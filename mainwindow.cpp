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
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>

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

    connect(ui->m_newGameAction, &QAction::triggered, this, static_cast<void(XMainWindow::*)()>(&XMainWindow::onNewGame));
    connect(ui->m_exitAction, &QAction::triggered, this, &XMainWindow::close);
    connect(ui->m_pauseAction, &QAction::toggled, this, &XMainWindow::onGamePauseed);
    connect(ui->m_levelCbx, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &XMainWindow::onLevelChanged);
    connect(ui->actionGroup, &QActionGroup::triggered, this, &XMainWindow::onActGroupTriggerd);

    connect(ui->m_helpAction, &QAction::triggered, this, &XMainWindow::onHelpActionTriggerd);
    connect(ui->m_aboutAction, &QAction::triggered, this, &XMainWindow::onAboutActionTriggerd);

}

void XMainWindow::updateChecked(int level)
{
    // 只更新数据，不触发信号
    disconnect(ui->m_levelCbx, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &XMainWindow::onLevelChanged);
    disconnect(ui->actionGroup, &QActionGroup::triggered, this, &XMainWindow::onActGroupTriggerd);

    auto newact = ui->levelAction[level];
    auto newindex = ui->m_levelCbx;
    if(!newact->isChecked())
        ui->levelAction[level]->setChecked(true);
    if(newindex->currentIndex() != level)
        ui->m_levelCbx->setCurrentIndex(level);

    connect(ui->m_levelCbx, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &XMainWindow::onLevelChanged);
    connect(ui->actionGroup, &QActionGroup::triggered, this, &XMainWindow::onActGroupTriggerd);
}
void XMainWindow::onNewGame()
{
    m_timer->stop();
    m_recordTime = 0;
    advanceTime();
    // 按照原来的配置重新开始游戏，默认是9*9-10
    m_scene->restartGame();
    m_timer->start();
    onMinesCountChanged(0);
    sizeFitoLevel();
}

void XMainWindow::onNewGame(int level)
{
    m_timer->stop();
    switch (level) {
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
           if(!this->onUserDefined()){
                updateChecked(m_level);
                m_timer->start();
                return;
           }
        break;
        default:
            qDebug() << "invalid difficulty options" << endl;
            m_timer->start();
            return;
        break;
    }

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
    m_level = level;
    updateChecked(level);
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
    this->onNewGame(act2level[act]);
}

void XMainWindow::onLevelChanged(int index)
{
    this->onNewGame(index);
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

void XMainWindow::onHelpActionTriggerd()
{
    QDesktopServices::openUrl(QUrl(AppResource::helpUrl));
}

void XMainWindow::onAboutActionTriggerd()
{
    QMessageBox::about(this, "about", AppResource::copyright);
}

bool XMainWindow::onUserDefined()
{
    m_setting->exec();
    if(!m_setting->Success()){
        return false;
    }
    m_scene->startNewGame(m_setting->getArgs());
    return true;
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
//    mw->setFixedSize();


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
