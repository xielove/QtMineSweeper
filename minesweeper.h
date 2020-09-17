#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#include <utility>
#include <vector>
#include <list>
#include <QGraphicsObject>

#include "cellitem.h"
#include "commondef.h"
#include "fielditem.h"

class MineSweeper
{
protected:
    bool m_firstClick = false;
    bool m_win = false;
    bool m_gameOver = false;
    FieldAction *m_fieldAction = nullptr;
    FieldItem *m_fieldItem = nullptr;
    int  m_unRevealed;
    int  m_marked;
    bool checkWin();

public:
    MineSweeper();
    virtual ~MineSweeper();
    // 在子类中实现代码
    virtual void initField(FieldArgs args) = 0;
    virtual void startNewGame(FieldArgs args) = 0;
    virtual void revealItem(FieldPos pos);
    virtual void markItem(FieldPos pos);

    // 用于查询雷区参数
    const FieldItem* fieldItem() const{ return this->m_fieldItem; }
    inline bool isGameOver() const {return m_gameOver;}
    inline bool isWin() const {return m_gameOver && m_win;}
    inline bool isLose() const {return m_gameOver && !m_win;}
};


class XMineSweeper : public QGraphicsObject, public MineSweeper
{
    Q_OBJECT
private:
    int m_cellSize;
    void mousePressEvent( QGraphicsSceneMouseEvent * ev) override;
    void mouseReleaseEvent( QGraphicsSceneMouseEvent * ev) override;
    void mouseMoveEvent( QGraphicsSceneMouseEvent * ev) override;
    void adjustCellPos();

Q_SIGNALS:
    void flaggedMinesCountChanged(int);
    void gameOver(bool win);

public:
    XMineSweeper();
    virtual ~XMineSweeper();
    void initField(FieldArgs) override;
    void revealItem(FieldPos pos) override;
    void markItem(FieldPos pos) override;
    void startNewGame(FieldArgs args) override;

    int cellSize() const { return m_cellSize; }

    // 必须要重写这两个虚函数
    void paint( QPainter * painter, const QStyleOptionGraphicsItem*, QWidget * widget = nullptr ) override;
    QRectF boundingRect() const override;
};
#endif /* MINESWEEPER_H */
