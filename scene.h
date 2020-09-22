#ifndef MINESCREEN_H
#define MINESCREEN_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include "fielditem.h"
#include "minesweeper.h"


class XMinesScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit XMinesScene(QObject *parent);
    void resizeScene(int width, int height);
    void startNewGame(FieldArgs args);
    void restartGame();
    void setGamePaused(bool paused);
    QSize fieldSize();

    void centerField();
    int totalMines(){
        auto it = m_mineSweeper->fieldItem();
        return it->minesCount();
    }
        bool isGameOver() const {return m_mineSweeper->isGameOver();}
Q_SIGNALS:
    void minesCountChanged(int );
    void gameOver(bool);
    void firstClickDone();

private Q_SLOTS:
    void onGameOver(bool);

private:
    XMineSweeper *m_mineSweeper = nullptr;
    QGraphicsPixmapItem *m_stoping = nullptr;
    QGraphicsTextItem *m_pausedText = nullptr;
//    CellItem *item = nullptr;
    void updateBackground();
};

class XMinesView : public QGraphicsView
{
    Q_OBJECT
public:
    XMinesView(XMinesScene* scene, QWidget *parent);
private:
    void resizeEvent( QResizeEvent *ev ) override;
    XMinesScene* m_scene = nullptr;
};

#endif // MINESCREEN_H
