#include <cstddef>
#include <cstdlib>
#include <cstdio>
#include <time.h>
#include <utility>
#include "QDebug"
#include "QGraphicsSceneMouseEvent"
#include <QMessageBox>
#include "fielditem.h"

const FieldArgs FieldArgs::Low{9, 9, 10};
const FieldArgs FieldArgs::Medium{16, 16, 40};
const FieldArgs FieldArgs::High{30, 16, 99};

FieldPos FieldItem::itemPos(CellItem *item)
{
    for(size_t i = 0; i < m_cellitems.size(); i++){
        if(item == m_cellitems.at(i)){
            return FieldPos(i/colsCount(), i%colsCount());
        }
    }
    return FieldPos(-1, -1);
}
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

//#include <QDebug>
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


std::list<CellItem *> FieldItem::neighbors(FieldPos pos)
{
    qDebug("std::list<CellItem *> FieldItem::neighbors(FieldPos pos)");
    std::list<FieldPos> neighbors = neighborsPos(pos);

    std::list<CellItem *> nei;
    for(auto pos : neighbors){
        nei.push_back(itemAt(pos));
    }
    return nei;
    qDebug("std::list<CellItem *> FieldItem::neighbors(FieldPos pos)");
}

std::list<FieldPos> FieldItem::neighborsPos(FieldPos pos)
{
    qDebug("std::list<FieldPos> FieldItem::neighborsPos(FieldPos pos)");
    std::list<FieldPos> nei;
    int row = pos.first;
    int col = pos.second;
    qDebug("%d, %d", row, col);
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
    qDebug("std::list<FieldPos> FieldItem::neighborsPos(FieldPos pos)");
    return nei;
}

void FieldAction::revealAllMines()
{
    for(int pos : m_field->m_randomPos){
        m_field->m_cellitems[pos]->forceReveal();
    }
}

void FieldAction::markAllMines()
{
    for(int pos : m_field->m_randomPos){
        auto it = m_field->m_cellitems[pos];
        if(!it->isFlagged()){
            it->forceMark();
        }
    }
}
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
    qDebug() << "void FieldItem::resetAllCells()" ;
    for(auto it : m_field->m_cellitems){
        it->reset();
    }
    qDebug() << "void FieldItem::resetAllCells()" ;
}



void FieldAction::updateCellsDigit()
{
    qDebug("void FieldAction::updateCellsDigit()");
    for (auto pos : m_field->m_randomPos) {
//        qDebug() << "pos" << pos ;
        std::list<CellItem*> neighbor = m_field->neighbors(FieldPos(pos / m_field->colsCount(), pos % m_field->colsCount()));
        for (auto it : neighbor) {
            it->setDight(it->dight() + 1);
        }
    }
    qDebug("void FieldAction::updateCellsDigit()");
}

void FieldAction::clearField()
{
    qDebug("void FieldAction::clearField()");
    for (auto it : m_field->m_cellitems) {
        delete it;
    }
    m_field->m_cellitems.clear();
    qDebug("void FieldAction::clearField()");
}

void FieldAction::generateField(FieldPos fp)
{
    qDebug("void FieldAction::generateField(FieldPos fp)");
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
    qDebug() << m_field->m_randomPos;
    qDebug() << m_field->m_randomPos.size();
    updateCellsDigit();
        qDebug("void FieldAction::generateField(FieldPos fp)");
}

//void MineSweeper::initField(FieldArgs args)
//{
//    m_args = args;
//    m_unRevealed = cellItemsCount();
//    m_marked = 0;
//    m_firstClick = true;
//    m_gameOver = false;
//    m_win = false;
//
//    int oldSize = m_cellitems.size();
//    int newSize = cellItemsCount();
//
//    if(oldSize > newSize){
//        for(int i = oldSize-1; i >= newSize; i--){
//            delete m_cellitems[i];
//            m_cellitems.pop_back();
//        }
//    }
//    int j = oldSize;
//    while(j < newSize){
//        auto it = new CellItem();
//        m_cellitems.push_back(it);
//        j++;
//    }
//    resetAllCells();
//}

FieldAction::FieldAction(FieldItem *field)
    :m_field(field)
{
    if (m_field == nullptr) {
        qDebug() << "Error: init FieldAction with null FieldItem";
    }
}


