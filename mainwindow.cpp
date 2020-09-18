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

    this->setFont(QFont("Microsoft YaHei"));

    setActions();
    setStatusBar();
    setMenu();

    m_scene = new XMinesScene(this);

//    qDebug() << "创建场景成功";
    m_view = new XMinesView( m_scene, this );
    m_view->setCacheMode( QGraphicsView::CacheBackground );
    m_view->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    m_view->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    m_view->setFrameStyle(QFrame::NoFrame);

    m_timer = new QTimer(this);
    m_timer->start(1000);
    onMinesCountChanged(0);

    setWindowIcon(QIcon(":/res/icons/logo.png"));
    setWindowTitle("扫雷");
    setCentralWidget(m_view);

    connect(m_timer, &QTimer::timeout, this, &XMainWindow::advanceTime);
    connect(m_scene, &XMinesScene::minesCountChanged, this, &XMainWindow::onMinesCountChanged);
    connect(m_scene, &XMinesScene::gameOver, this, &XMainWindow::onGameOver);

//    qDebug() << "创建主窗口";

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
    auto menubar = this->menuBar();
    auto m_about = new QMenu("帮助");
    auto m_setting = new QMenu("设置");
    auto m_file = new QMenu("文件");

    menubar->addMenu(m_file);
    menubar->addMenu(m_setting);
    menubar->addMenu(m_about);

    m_file->addAction(m_newGameAction);
    auto m_exit = m_file->addAction("退出");
    connect(m_exit, &QAction::triggered, [=](){
        this->close();
    });
    m_exit->setIcon(AppResource::exitIcon);
}

void XMainWindow::setActions()
{
    m_newGameAction = new QAction(this);
    m_newGameAction->setIcon(QIcon(":/res/icons/add.png"));
    m_newGameAction->setIconText("新游戏");
    m_PauseAction = new QAction(this);
    m_PauseAction->setIcon(QIcon(":/res/icons/stop.png"));
    m_PauseAction->setIconText("暂停");
    QToolBar *toolbar = new QToolBar(this);

    toolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toolbar->addAction(m_newGameAction);
    toolbar->addAction(m_PauseAction);
    this->addToolBar(Qt::TopToolBarArea, toolbar);
    toolbar->setMovable(false);
    m_PauseAction->setCheckable(true);
    connect(m_newGameAction, &QAction::triggered, this, &XMainWindow::onNewGame);
    connect(m_PauseAction, &QAction::toggled, this, &XMainWindow::onGamePauseed);
}

#include <QSpacerItem>
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
//    statusBar()->insertPermanentWidget( 0, m_mineLabel );
//    statusBar()->insertPermanentWidget( 1, m_timeLabel );
//    statusBar()->insertPermanentWidget( 2, m_levelCbx );
//    QSpacerItem sp(20, statusBar()->height());

    statusBar()->addWidget(m_timeLabel);
    statusBar()->addPermanentWidget(m_mineLabel);
    statusBar()->addPermanentWidget(m_levelCbx);
    statusBar()->setSizeGripEnabled(false);
    connect(m_levelCbx, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &XMainWindow::onDifficultyChanged);

}

void UI_XMainWindow::setupUI(QMainWindow *mw)
{
    // 状态栏设置
    m_statusBar = new QStatusBar(mw);
    m_timeLabel = new QLabel(m_statusBar);
    m_mineLabel = new QLabel(m_statusBar);
    m_levelCbx  = new QComboBox(m_statusBar);

    m_levelCbx->addItems(AppResource::levels);
    m_mineLabel->setText("Mines: 0/0");
    m_timeLabel->setText("Time: 00:00");

    m_statusBar->addWidget(m_timeLabel );
    m_statusBar->addPermanentWidget(m_mineLabel );
    m_statusBar->addPermanentWidget(m_levelCbx );
    m_statusBar->setSizeGripEnabled(false);

    // 工具栏设置
    m_toolBar = new QToolBar(mw);
    m_newGameAction = new QAction(AppResource::newIcon, "新游戏", mw);
    m_pauseAction   = new QAction(AppResource::stopIcon, "暂停", mw);
    m_toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_toolBar->addAction(m_newGameAction);
    m_toolBar->addAction(m_pauseAction);
    m_toolBar->setMovable(false);
    m_pauseAction->setCheckable(true);

    // 菜单栏设置
    m_menuBar = new QMenuBar(mw);
    auto file = m_menuBar->addMenu("文件");
    auto setting = m_menuBar->addMenu("设置");
    auto help = m_menuBar->addMenu("帮助");

    file->addAction(m_newGameAction);
    file->addSeparator();
    file->addAction(m_exitAction);

    for(int i = 0; i < AppResource::levelsCount; i++){
        levelAction.append(new QAction(AppResource::levels[i], mw));
        levelAction[i]->setCheckable(true);
    }
    setting->addActions(levelAction);

    m_helpAction  = new QAction("帮助", help);
    m_aboutAction = new QAction("关于", help);
    help->addAction(m_helpAction);
    help->addAction(m_aboutAction);
}
