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
#include <QActionGroup>
#include <QLayout>

XMainWindow::XMainWindow()
{
    ui = new UI_XMainWindow();
    ui->setupUI(this);

    m_setting = new SettingDialog(this);

    m_timer = new QTimer(this);
    m_timer->setInterval(1000);

    m_scene = new XMinesScene(this);
    m_view = new XMinesView( m_scene, this );
    m_view->setCacheMode( QGraphicsView::CacheBackground );
    m_view->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    m_view->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    m_view->setFrameStyle(QFrame::NoFrame);
    setCentralWidget(m_view);

    setConnections();
    onNewGame();
}

// Qt中QObject的子类对象，按照对象树析构
XMainWindow::~XMainWindow()
{
    delete ui;
}

void XMainWindow::setConnections()
{
    connect(m_timer, &QTimer::timeout, this, &XMainWindow::advanceTime);
    connect(m_scene, &XMinesScene::minesCountChanged, this, &XMainWindow::onMinesCountChanged);
    connect(m_scene, &XMinesScene::gameOver, this, &XMainWindow::onGameOver);

    connect(ui->m_newGameAction, &QAction::triggered, this, &XMainWindow::onNewGame);
    connect(ui->m_exitAction, &QAction::triggered, this, &XMainWindow::close);
    connect(ui->m_pauseAction, &QAction::toggled, this, &XMainWindow::onGamePauseed);
    connect(ui->m_levelCbx, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &XMainWindow::onLevelChanged);
    connect(ui->actionGroup, &QActionGroup::triggered, this, &XMainWindow::onActGroupTriggerd);
}

void XMainWindow::updateChecked()
{
    auto newact = ui->levelAction[m_level];
    auto newindex = ui->m_levelCbx;
    if(!newact->isChecked())
        ui->levelAction[m_level]->setChecked(true);
    if(newindex->currentIndex() != m_level)
        ui->m_levelCbx->setCurrentIndex(m_level);
}

bool XMainWindow::onNewGame()
{
    qDebug() << "m_level = " << m_level;
    m_timer->stop();
    switch (m_level) {
        case 0:
            m_scene->startNewGame(AppResource::Low);
        break;
        case 1:
            m_scene->startNewGame(AppResource::Medium);
        break;
        case 2:
            m_scene->startNewGame(AppResource::High);
        break;
        case 3:
            m_setting->exec();
            if(m_setting->Success()){
                qDebug() << m_setting->Success();
                m_scene->startNewGame(m_setting->getArgs());
            }else{
                m_timer->start();
                return false;
            }
        break;
    }

    qDebug() << "创建了一个新游戏";

    if( ui->m_pauseAction->isChecked() )
    {
        m_scene->setGamePaused(false);
        ui->m_pauseAction->setChecked(false);
    }
    m_recordTime = 0;
    advanceTime();
    m_timer->start();
    onMinesCountChanged(0);
    sizeFitoLevel();

    // 更新难度选择器的当前选项
    updateChecked();
    return true;
}

void XMainWindow::onGamePauseed(bool paused)
{
    m_scene->setGamePaused(paused);
    // 游戏结束时，确保游戏时间不会改变
    if(m_scene->isGameOver()){
        return;
    }
    if( paused )
        m_timer->stop();
    else
        m_timer->start();
}

void XMainWindow::onActGroupTriggerd(QAction * act)
{
    if(act2level.size() == 0){
        for(int i = 0; i < ui->levelAction.size(); i++){
            act2level.insert(ui->levelAction[i], i);
        }
    }
    onLevelChanged((act2level[act]));
}

void XMainWindow::onLevelChanged(int index)
{
    if(m_level == index){
        return ;
    }
    int oldlevel = m_level;
    this->m_level = index;
    if(!onNewGame()){
        qDebug() << "取消自定义";
        m_level = oldlevel;
        updateChecked();
    }
}

void XMainWindow::onMinesCountChanged(int mines)
{
    ui->m_mineLabel->setText(QString("Mines: %1/%2").arg(mines).arg(m_scene->totalMines()));
}

void XMainWindow::advanceTime()
{
    this->m_recordTime++;
    int second = m_recordTime % 60;
    int minute = m_recordTime / 60;
    QString timeStr = "Time: %1:%2";
    ui->m_timeLabel->setText(timeStr.arg(minute, 2, 10, QChar('0')).arg(second, 2, 10, QChar('0')));
}

void XMainWindow::onGameOver(bool win)
{
    Q_UNUSED(win);
    m_timer->stop();
}

void XMainWindow::sizeFitoLevel()
{
    int h = AppResource::vborder + m_scene->fieldSize().height();
    int w = AppResource::hborder + m_scene->fieldSize().width();
    this->resize(w, h);
    m_scene->centerField();
}

void UI_XMainWindow::setupUI(QMainWindow *mw)
{
//    mw->resize(AppResource::defaultSceneSize);
    // 初始化窗口
    mw->setFont(QFont("Microsoft YaHei"));
    mw->setWindowIcon(AppResource::logo);
    mw->setWindowTitle("扫雷");


    // 初始化Action
    m_newGameAction = new QAction(AppResource::newIcon, "新游戏", mw);
    m_pauseAction   = new QAction(AppResource::stopIcon, "暂停", mw);
    m_pauseAction->setCheckable(true);
    m_exitAction = new QAction(AppResource::exitIcon, "退出", mw);
    m_helpAction  = new QAction("帮助", mw);
    m_aboutAction = new QAction("关于", mw);
    actionGroup = new QActionGroup(mw);
    for(int i = 0; i < AppResource::levelsCount; i++){
        levelAction.append(new QAction(AppResource::levels[i], mw));
        levelAction[i]->setCheckable(true);
        actionGroup->addAction(levelAction[i]);
    }
    levelAction[0]->setChecked(true);

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
    setting->addActions(levelAction);
    help->addAction(m_helpAction);
    help->addAction(m_aboutAction);

    // 添加到主窗口
    mw->setStatusBar(m_statusBar);
    mw->addToolBar(m_toolBar);
    mw->setMenuBar(m_menuBar);
    mw->show();
}
