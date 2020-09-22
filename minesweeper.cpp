#include "minesweeper.h"
#include "QDebug"
#include "appresource.h"
#include <QGraphicsSceneMouseEvent>


bool MineSweeper::checkWin()
{
    if(m_unRevealed == m_fieldItem->minesCount()){
        return true;
    }
    return false;
}
void MineSweeper::revealItem(FieldPos pos)
{
    if(isGameOver()){
        return;
    }
    if(m_firstClick){
        m_fieldAction->generateField(pos);
        m_firstClick = false;
    }
    auto it = m_fieldItem->itemAt(pos);
    if(it->isRevealed() || it->isFlagged()){
        return ;
    }
    m_unRevealed--;
    if(it->hasMine()){
        m_fieldAction->revealAllMines();
        m_gameOver = true;
        m_win = false;
        return;
    }else if(it->dight() == 0){
        it->reveal();
        int revealed = m_fieldAction->revealEmptySpace(pos);
        m_unRevealed -= revealed;
    }else{
        it->reveal();
    }
    if(checkWin()){
        m_gameOver = true;
        m_win = true;
        m_fieldAction->markAllMines();
    }
}

void MineSweeper::markItem(FieldPos pos)
{
    if(isGameOver()){
        return ;
    }
    auto item = m_fieldItem->itemAt(pos);
    if(item->isInitialed() && m_marked >= m_fieldItem->minesCount()){
        return;
    }
    if(item->isFlagged()){
        m_marked--;
    }else if(item->isInitialed()){
        m_marked++;
    }
    item->mark();
}



MineSweeper::MineSweeper()
{

}

MineSweeper::~MineSweeper()
{
    m_fieldAction->clearField();
}
/*
 * XFieldItem
 */

void XMineSweeper::mousePressEvent(QGraphicsSceneMouseEvent *ev)
{
    Q_UNUSED(ev);
}

void XMineSweeper::mouseReleaseEvent(QGraphicsSceneMouseEvent *ev)
{
//    qDebug() << ev->pos();
    int row = static_cast<int>(ev->pos().y()/cellSize());
    int col = static_cast<int>(ev->pos().x()/cellSize());
//    qDebug() << "row, col = " << row << ", " << col;

    if( row < 0 || row >= m_fieldItem->rowsCount() || col < 0 || col >= m_fieldItem->colsCount() )
    {
        qDebug() << "无效的点击范围";
        return;
    }
    // 左键点击
    if(ev->button() == Qt::LeftButton && (ev->buttons() & Qt::RightButton) == false)
    {
        this->revealItem(FieldPos(row, col));
    }
    else if(ev->button() == Qt::RightButton && (ev->buttons() & Qt::LeftButton) == false)
    {
        // 右键
        this->markItem(FieldPos(row, col));
    }
}

void XMineSweeper::mouseMoveEvent(QGraphicsSceneMouseEvent *ev)
{
    Q_UNUSED(ev);
}

XMineSweeper::XMineSweeper(/* args */)
    :m_cellSize(AppResource::defaultPixmapSize)
{
    m_fieldItem = new FieldItem();
    m_fieldAction = new FieldAction(m_fieldItem);
}

XMineSweeper::~XMineSweeper()
{

}


void XMineSweeper::initField(FieldArgs args)
{
    m_fieldAction->setFieldArgs(args);
    int cellsCount = m_fieldItem->cellItemsCount();
    m_unRevealed = cellsCount;
    m_marked = 0;
    m_fieldAction->clearField();
    std::vector<CellItem*> cellitems(cellsCount, nullptr);
    for (int i = 0; i < cellsCount; i++) {
        cellitems[i] = new XCellItem(this);
    }
    m_fieldAction->setCellItems(cellitems);
    m_fieldAction->resetAllCells();
    adjustCellPos();
}

void XMineSweeper::paint(QPainter *painter, const QStyleOptionGraphicsItem *opt, QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(opt);
    Q_UNUSED(widget);
}

QRectF XMineSweeper::boundingRect() const
{
    return QRectF(0, 0, cellSize()* m_fieldItem->colsCount(), cellSize()* m_fieldItem->rowsCount());
}
void XMineSweeper::adjustCellPos()
{
    for(int row = 0; row < m_fieldItem->rowsCount(); ++row)
        for(int col=0; col < m_fieldItem->colsCount(); ++col)
        {
            XCellItem *it = static_cast<XCellItem *>(m_fieldItem->itemAt(row, col));
            it->setPos(col*cellSize(), row*cellSize());
        }
}

void XMineSweeper::revealItem(FieldPos pos)
{
    if(isGameOver()){
        return;
    }
    MineSweeper::revealItem(pos);
    if(this->isWin()){
        emit gameOver(true);
    }else if(this->isLose()){
        emit gameOver(false);
    }
}

void XMineSweeper::markItem(FieldPos pos)
{
    int oldMarkedCount = m_marked;
    MineSweeper::markItem(pos);
    if(oldMarkedCount != m_marked){
        emit flaggedMinesCountChanged(m_marked);
    }
}

void XMineSweeper::startNewGame(FieldArgs args)
{
    m_gameOver = false;
    m_win = false;
    m_firstClick = true;
    initField(args);
}
