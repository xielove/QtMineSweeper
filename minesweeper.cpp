#include "minesweeper.h"
#include "QDebug"
#include "appresource.h"
#include <QGraphicsSceneMouseEvent>
const int defaultPixmapSize = 30;

bool MineSweeper::checkWin()
{
//    qDebug() << "m_unRevealed = " << m_unRevealed;
    if(m_unRevealed == m_fieldItem->minesCount()){
        return true;
    }
    return false;
}
void MineSweeper::revealItem(FieldPos pos)
{
//        qDebug("void MineSweeper::revealItem(FieldPos pos)");
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
//        qDebug("void MineSweeper::revealItem(FieldPos pos)");
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
    qDebug() << "mousePressEvent" ;
    // TODO: 增加对鼠标按下操作的处理
//    QGraphicsObject::mousePressEvent(ev);
}

void XMineSweeper::mouseReleaseEvent(QGraphicsSceneMouseEvent *ev)
{
    qDebug() << "mouseReleaseEvent";
    qDebug() << ev->pos();
    int row = static_cast<int>(ev->pos().y()/cellSize());
    int col = static_cast<int>(ev->pos().x()/cellSize());
    qDebug() << "row, col = " << row << ", " << col;

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
//    QGraphicsObject::mouseReleaseEvent(ev);
}

void XMineSweeper::mouseMoveEvent(QGraphicsSceneMouseEvent *ev)
{
    Q_UNUSED(ev);
//    qDebug() << "mouseMoveEvent" ;
//    QGraphicsObject::mouseMoveEvent(ev);
}

XMineSweeper::XMineSweeper(/* args */)
    :m_cellSize(defaultPixmapSize)
{
//    qDebug() << "构造XMineSweeper" ;
    m_fieldItem = new FieldItem();
    m_fieldAction = new FieldAction(m_fieldItem);
//    qDebug() << "成功构造XMineSweeper" ;
}

XMineSweeper::~XMineSweeper()
{

}


void XMineSweeper::initField(FieldArgs args)
{
//    qDebug("void XMineSweeper::initField(FieldArgs args)");
    m_fieldAction->setFieldArgs(args);
    int cellsCount = m_fieldItem->cellItemsCount();
    m_unRevealed = cellsCount;
    m_marked = 0;
    m_fieldAction->clearField();
    qDebug("create CellItem");
    std::vector<CellItem*> cellitems(cellsCount, nullptr);
    for (int i = 0; i < cellsCount; i++) {
        cellitems[i] = new XCellItem(this);
    }
    qDebug("create CellItem");
    m_fieldAction->setCellItems(cellitems);
    qDebug("reset all cells");
    m_fieldAction->resetAllCells();
    adjustCellPos();
//    qDebug("void XMineSweeper::initField(FieldArgs args)");
}

void XMineSweeper::paint(QPainter *painter, const QStyleOptionGraphicsItem *opt, QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(opt);
    Q_UNUSED(widget);
}

QRectF XMineSweeper::boundingRect() const
{
//    return QRectF(0, 0, m_cellSize*(m_numCols+2), m_cellSize*(m_numRows+2));
    return QRectF(0, 0, cellSize()* m_fieldItem->colsCount(), cellSize()* m_fieldItem->rowsCount());
}
void XMineSweeper::adjustCellPos()
{
//    Q_ASSERT( m_cells.size() == m_numRows*m_numCols );
//    qDebug() << "重新排列方格位置" ;
//    qDebug() << "m_rowsCount = " << m_rowsCount;
//    qDebug() << "m_colsCount = " << m_colsCount;
//    qDebug() << "void XFieldItem::adjustCellPos()" ;
    for(int row = 0; row < m_fieldItem->rowsCount(); ++row)
        for(int col=0; col < m_fieldItem->colsCount(); ++col)
        {
            XCellItem *it = static_cast<XCellItem *>(m_fieldItem->itemAt(row, col));
            it->setPos(col*cellSize(), row*cellSize());
        }
//    qDebug() << "确定雷区方格位置成功" ;
//    qDebug() << "void XFieldItem::adjustCellPos()" ;

}

void XMineSweeper::revealItem(FieldPos pos)
{
//    qDebug("void XMineSweeper::revealItem(FieldPos pos)");
    if(isGameOver()){
        return;
    }
    MineSweeper::revealItem(pos);
    if(this->isWin()){
        emit gameOver(true);
    }else if(this->isLose()){
        emit gameOver(false);
    }
//        qDebug("void XMineSweeper::revealItem(FieldPos pos)");
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
//    qDebug() << "void XMineSweeper::startNewGame(FieldArgs args)";
    m_gameOver = false;
    m_win = false;
    m_firstClick = true;
    initField(args);
//    qDebug() << "void XMineSweeper::startNewGame(FieldArgs args)";
}
