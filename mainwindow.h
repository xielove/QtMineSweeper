#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QPointer>
#include <QLabel>
#include <QMainWindow>
#include <QStatusBar>
#include <QMenu>
#include <QAction>
#include <QComboBox>
#include <QTimer>
#include <QActionGroup>
#include "scene.h"
#include "settingdialog.h"
#include "appresource.h"

class UI_XMainWindow{
public:
    QStatusBar *m_statusBar;
    QMenuBar *m_menuBar;
    QToolBar *m_toolBar;
    QWidget *m_centralWidget;

    QLabel *m_mineLabel;
    QLabel *m_timeLabel;
    QComboBox *m_levelCbx;
    QAction* m_newGameAction;
    QAction* m_pauseAction;
    QAction* m_exitAction;
    QAction* m_helpAction;
    QAction* m_aboutAction;
    QList<QAction* > levelAction;
    QActionGroup *actionGroup;

public:
    void setupUI(QMainWindow *mainWindow);
};

class XMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    XMainWindow();
    virtual ~XMainWindow();

    XMinesScene *m_scene = nullptr;
    XMinesView *m_view = nullptr;
    SettingDialog *m_setting = nullptr;

private Q_SLOTS:
    void onNewGame();
    void onNewGame(int level);
    void onGamePauseed(bool);

    void onActGroupTriggerd(QAction *);
    void onLevelChanged(int);
    void onMinesCountChanged(int);

    void advanceTime();
    void onGameOver(bool);

    void onHelpActionTriggerd();
    void onAboutActionTriggerd();
    bool onUserDefined();

private:
    void sizeFitoLevel();
    void setConnections();
    void updateChecked(int );

private:
    UI_XMainWindow *ui;
    QTimer *m_timer;
    int m_recordTime = 0;
    int m_level = 0;
    QMap<QAction*, int> act2level;
};
#endif
