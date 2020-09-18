#include <cstddef>
#include <cstdlib>
#include <cstdio>
#include <time.h>
#include <utility>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QMessageBox>
#include "fielditem.h"

// 预先定义的游戏等级
const FieldArgs FieldArgs::Low{9, 9, 10};
const FieldArgs FieldArgs::Medium{16, 16, 40};
const FieldArgs FieldArgs::High{30, 16, 99};


// 查找item的坐标
FieldPos FieldItem::itemPos(CellItem *item)
{
    for(size_t i = 0; i < m_cellitems.size(); i++){
        if(item == m_cellitems.at(i)){
            return FieldPos(i/colsCount(), i%colsCount());
        }
    }
    return FieldPos(-1, -1);
}

// 二维坐标到一维坐标的转换
int FieldItem::transToIndex(int row, int col)
{
    if(row < 0 || row >= rowsCount()  || col < 0 || col >= colsCount())
        return -1;
    return row * colsCount() + col;
}
int FieldItem::transToIndex(FieldPos pos)
{
    return transToIndex(pos.first, pos.second);
}


// 返回目标区域的CellItem对象
CellItem *FieldItem::itemAt(int row, int col)
{
    int pos = transToIndex(row, col);
//    qDebug() << "周围方块的位置为：" << pos;
    return (pos == -1) ? nullptr : m_cellitems.at(pos);
}
CellItem *FieldItem::itemAt(FieldPos pos)
{
    return itemAt(pos.first, pos.second);
}


// 返回目标位置pos周围的格子的对象
std::list<CellItem *> FieldItem::neighbors(FieldPos pos)
{
//    qDebug("std::list<CellItem *> FieldItem::neighbors(FieldPos pos)");
    std::list<FieldPos> neighbors = neighborsPos(pos);

    std::list<CellItem *> nei;
    for(auto pos : neighbors){
        nei.push_back(itemAt(pos));
    }
    return nei;
//    qDebug("std::list<CellItem *> FieldItem::neighbors(FieldPos pos)");
}

// 返回目标位置pos周围的格子的坐标
std::list<FieldPos> FieldItem::neighborsPos(FieldPos pos)
{
//    qDebug("std::list<FieldPos> FieldItem::neighborsPos(FieldPos pos)");
    std::list<FieldPos> nei;
    int row = pos.first;
    int col = pos.second;
//    qDebug("%d, %d", row, col);
    for(int i = row-1; i <= row+1; i++){
        for(int j = col-1; j <= col+1; j++){
            if(row == i && col == j)
                continue;
            CellItem *cell = itemAt(i, j);
            if(cell){
                nei.emplace_back(FieldPos(i, j));
            }
        }
    }
//    qDebug("周围的方格数量为：%ld", nei.size());
//    qDebug("std::list<FieldPos> FieldItem::neighborsPos(FieldPos pos)");
    return nei;
}

// 前置条件：游戏失败时，点击了埋了地雷的格子
// 显示所有的雷的位置
void FieldAction::revealAllMines()
{
    // 可能出现一些格子被标记了有雷，但是游戏失败时标记没有清除
    // 可以考虑扫描整个雷区
    for(int pos : m_field->m_randomPos){
        m_field->m_cellitems[pos]->forceReveal();
    }
}

// 前置条件：当游戏胜利时，当没有探测的格子数量等于雷的数量
// 将场景中的所有雷标记出来
void FieldAction::markAllMines()
{
    for(int pos : m_field->m_randomPos){
        auto it = m_field->m_cellitems[pos];
        if(!it->isFlagged()){
            it->forceMark();
        }
    }
}

// 如果目标区域p不是雷，而且周围格子没有雷，递归展开
// 返回值是一共展开的格子数量
int FieldAction::revealEmptySpace(FieldPos p)
{
    int count = 0;
    std::list<FieldPos> neighbor = m_field->neighborsPos(p);
    CellItem *item = nullptr;
    for(auto pos : neighbor){
        item = m_field->itemAt(pos);
        if(item->isRevealed() || item->isFlagged()){
            continue;
        }
        if(item->dight() == 0)
        {
            item->reveal();
            count += revealEmptySpace(pos)+1;
        }
        else
        {
            count++;
            item->reveal();
        }
    }
    return count;
}

void FieldAction::resetAllCells()
{
//    qDebug() << "void FieldItem::resetAllCells()" ;
    for(auto it : m_field->m_cellitems){
        it->reset();
    }
//    qDebug() << "void FieldItem::resetAllCells()" ;
}

// 在埋雷之后更新雷区中的数字
void FieldAction::updateCellsDigit()
{
//    qDebug("void FieldAction::updateCellsDigit()");
    for (auto pos : m_field->m_randomPos) {
//        qDebug() << "pos" << pos ;
        std::list<CellItem*> neighbor = m_field->neighbors(FieldPos(pos / m_field->colsCount(), pos % m_field->colsCount()));
        for (auto it : neighbor) {
            if(!it->hasMine()){
                it->setDight(it->dight() + 1);
            }
        }
    }
//    qDebug("void FieldAction::updateCellsDigit()");
}

void FieldAction::clearField()
{
//    qDebug("void FieldAction::clearField()");
    for (auto it : m_field->m_cellitems) {
        delete it;
    }
    m_field->m_cellitems.clear();
//    qDebug("void FieldAction::clearField()");
}

// 埋雷算法：对于坐标fp要求该方格不是雷，同时周围没有雷
void FieldAction::generateField(FieldPos fp)
{
//    qDebug("void FieldAction::generateField(FieldPos fp)");
    m_field->m_randomPos.resize(m_field->minesCount());
    CellItem* item = nullptr;
    int clickedPos = fp.first * m_field->colsCount() + fp.second;
    std::list<CellItem* > neiborItems = m_field->neighbors(fp);
    srand(time(nullptr));
    int i;
    for (i = 0; i < m_field->minesCount(); ) {
        int pos = rand() % m_field->cellItemsCount();
        item = m_field->m_cellitems[pos];
        if (!m_field->m_cellitems[pos]->hasMine()
            && clickedPos != pos
            && std::find(neiborItems.begin(), neiborItems.end(), item) == neiborItems.end())
        {
            m_field->m_cellitems[pos]->setHasMine(true);
            m_field->m_randomPos[i] = pos;
            i++;
        }
    }
//    qDebug() << m_field->m_randomPos;
//    qDebug() << m_field->m_randomPos.size();
    updateCellsDigit();
//        qDebug("void FieldAction::generateField(FieldPos fp)");
}

FieldAction::FieldAction(FieldItem *field)
    :m_field(field)
{
    if (m_field == nullptr) {
        qDebug() << "Error: init FieldAction with null FieldItem";
    }
}
