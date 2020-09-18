#include "scene.h"
#include <QResizeEvent>
#include "fielditem.h"
#include "QDebug"
#include <QMessageBox>

XMinesScene::XMinesScene(QObject *parent)
    :QGraphicsScene(parent)
{
//    qDebug() << "创建雷区，并添加到场景中";
    setItemIndexMethod( NoIndex );
    m_stoping = new QGraphicsPixmapItem();
    m_stoping->setPixmap(QPixmap(":/res/stoping.jpg"));
    addItem(m_stoping);

    m_mineSweeper = new XMineSweeper();
    addItem(m_mineSweeper);

    resizeScene(m_mineSweeper->boundingRect().width(), m_mineSweeper->boundingRect().height());

    connect(m_mineSweeper, &XMineSweeper::flaggedMinesCountChanged, this, &XMinesScene::minesCountChanged);
    connect(m_mineSweeper, &XMineSweeper::gameOver, this, &XMinesScene::gameOver);
    connect(m_mineSweeper, &XMineSweeper::gameOver, this, &XMinesScene::onGameOver);

//    qDebug() << "启动一个新游戏";
//    m_mineSweeper->startNewGame(FieldArgs::Low);
    m_mineSweeper->startNewGame(FieldArgs::Low);
    int w = m_mineSweeper->boundingRect().width();
    int h = m_mineSweeper->boundingRect().height();
    resizeScene(w, h);
//    qDebug() << "成功创建场景类";
}

void XMinesScene::resizeScene(int width, int height)
{
    setSceneRect(0, 0, width, height);
    centerField();
    updateBackground();
}


void XMinesScene::startNewGame(FieldArgs args)
{
    m_mineSweeper->startNewGame(args);
//    QRectF rect = m_mineSweeper->boundingRect();
//    resizeScene(rect.width(), rect.height());
}

void XMinesScene::setGamePaused(bool paused)
{
    m_mineSweeper->setVisible(!paused);
}

QSize XMinesScene::fieldSize()
{
    auto rect = m_mineSweeper->boundingRect();
    QSize size(rect.width(), rect.height());
    return size;
}

void XMinesScene::centerField()
{
    m_mineSweeper->setPos( sceneRect().width()/2 - m_mineSweeper->boundingRect().width()/2,
                     sceneRect().height()/2 - m_mineSweeper->boundingRect().height()/2 );
}

void XMinesScene::onGameOver(bool win)
{
    if(!win){
        QMessageBox::question(nullptr, "消息", "抱歉，你失败了");
    }else{
        QMessageBox::information(nullptr, "消息", "恭喜，你赢了");
    }
}

void XMinesScene::updateBackground()
{
    QTransform trans;
    qreal qx, qy;
    auto rect = sceneRect();
    qx = 1.0 * rect.width() / m_stoping->pixmap().width();
    qy = 1.0 * rect.height() / m_stoping->pixmap().height();
    trans.scale(qx, qy);
    m_stoping->setTransform(trans);
}

XMinesView::XMinesView(XMinesScene *scene, QWidget *parent)
    : QGraphicsView(scene, parent), m_scene(scene)
{

}

void XMinesView::resizeEvent(QResizeEvent *ev)
{
    m_scene->resizeScene(ev->size().width(), ev->size().height());
}
