#include "mainwindow.h"
#include "scene.h"
#include "cellitem.h"
#include <QDebug>
#include <QSpacerItem>
#include <QList>
#include <QStringList>
#include <QToolBar>
#include <QMenu>
#include <QMenuBar>

XMainWindow::XMainWindow()
{
    setActions();
    setStatusBar();
    setMenu();

    m_scene = new XMinesScene(this);
//    m_scene->setBackgroundBrush()

    qDebug() << "创建场景成功";
    m_view = new XMinesView( m_scene, this );
    m_view->setCacheMode( QGraphicsView::CacheBackground );
    m_view->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    m_view->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    m_view->setFrameStyle(QFrame::NoFrame);

    m_timer = new QTimer(this);
    m_timer->start(1000);
    onMinesCountChanged(0);

    setWindowIcon(QIcon(":/icons/icons/logo.png"));
    setWindowTitle("扫雷");
    setCentralWidget(m_view);

    connect(m_timer, &QTimer::timeout, this, &XMainWindow::advanceTime);
    connect(m_scene, &XMinesScene::minesCountChanged, this, &XMainWindow::onMinesCountChanged);
    connect(m_scene, &XMinesScene::gameOver, this, &XMainWindow::onGameOver);

    qDebug() << "创建主窗口";

//    this->onNewGame();
}

void XMainWindow::onNewGame()
{
    m_timer->stop();
    this->m_recordTime = 0;
    advanceTime();
    onMinesCountChanged(0);
    m_timer->start();

    if( m_PauseAction->isChecked() )
    {
            m_scene->setGamePaused(false);
            m_PauseAction->setChecked(false);
    }
//    qDebug() << "新建一个游戏";
    switch (m_level) {
        case 0:
            m_scene->startNewGame(FieldArgs::Low);
        break;
        case 1:
            m_scene->startNewGame(FieldArgs::Medium);
        break;
        case 2:
            m_scene->startNewGame(FieldArgs::High);
        break;
    }
    sizeFitoLevel();
    this->onMinesCountChanged(0);

//    qDebug() << m_scene->fieldSize();
//    this->resize(m_scene->fieldSize());
//    m_view->resize(m_scene->fieldSize());
//    centralWidget()->resize(m_scene->fieldSize());
//    qDebug() << centralWidget()->size();
//    qDebug() << this->size();
}

void XMainWindow::onGamePauseed(bool paused)
{
    m_scene->setGamePaused(paused);
    if(m_scene->isGameOver()){
        return;
    }
    if( paused )
        m_timer->stop();
    else
        m_timer->start();
}

void XMainWindow::onDifficultyChanged(int index)
{
    this->m_level = index;
    qDebug() << m_level;
    onNewGame();
}

void XMainWindow::onMinesCountChanged(int mines)
{
    m_mineLabel->setText(QString("Mines: %1/%2").arg(mines).arg(m_scene->totalMines()));
}

void XMainWindow::advanceTime()
{
    this->m_recordTime++;
    int second = m_recordTime % 60;
    int minute = m_recordTime / 60;
    QString timeStr = "Time: %1:%2";
    m_timeLabel->setText(timeStr.arg(minute, 2, 10, QChar('0')).arg(second, 2, 10, QChar('0')));
}

void XMainWindow::onGameOver(bool win)
{
    Q_UNUSED(win);
    m_timer->stop();
}

void XMainWindow::sizeFitoLevel()
{
    // 小窗口变大窗口，大窗口不会自动变成小窗口
    auto centralSize = centralWidget()->size();
    auto fieldSize = m_scene->fieldSize();
    if(centralSize.width() < fieldSize.width() || centralSize.height() < fieldSize.height()){
        int hborder = this->size().height()-centralWidget()->size().height();
        int wborder = this->size().width()-centralWidget()->size().width();
        int h = hborder + m_scene->fieldSize().height();
        int w = wborder + m_scene->fieldSize().width();
        this->resize(w, h);
    }
    m_scene->centerField();
}

void XMainWindow::setMenu()
{
//    auto menubar = menuBar();
//    QMenu* file = new QMenu("文件", this);
//    file->addAction(m_newGameAction);
//    menubar->addMenu(file);
}

void XMainWindow::setActions()
{
    m_newGameAction = new QAction(this);
    m_newGameAction->setIcon(QIcon(":/icons/icons/new.jpg"));
    m_newGameAction->setIconText("新游戏");
    m_PauseAction = new QAction(this);
    m_PauseAction->setIcon(QIcon(":/icons/icons/stop.jpg"));
    m_PauseAction->setIconText("暂停");
//    m_newGameAction->set
    QToolBar *toolbar = new QToolBar(this);
//    toolbar->setFloatable(false);
    toolbar->setMovable(false);
    toolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toolbar->addAction(m_newGameAction);
    toolbar->addAction(m_PauseAction);
    this->addToolBar(Qt::TopToolBarArea, toolbar);
    m_PauseAction->setCheckable(true);
    connect(m_newGameAction, &QAction::triggered, this, &XMainWindow::onNewGame);
    connect(m_PauseAction, &QAction::toggled, this, &XMainWindow::onGamePauseed);
}

void XMainWindow::setStatusBar()
{
//    m_status = new QStatusBar(this);
    m_timeLabel = new QLabel();
    m_mineLabel = new QLabel();
    m_levelCbx = new QComboBox();
    QStringList ranks;
    ranks << "简单" << "中等" << "困难" << "自定义";
    m_levelCbx->addItems(ranks);
    m_mineLabel->setText("Mines: 0/0");
    m_timeLabel->setText("Time: 00:00");
    statusBar()->insertPermanentWidget( 0, m_mineLabel );
    statusBar()->insertPermanentWidget( 1, m_timeLabel );
    statusBar()->insertPermanentWidget( 2, m_levelCbx );
    statusBar()->setSizeGripEnabled(false);
    connect(m_levelCbx, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &XMainWindow::onDifficultyChanged);

}
