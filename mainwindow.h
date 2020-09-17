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
#include "scene.h"

#include "commondef.h"

class XMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    XMainWindow();

    XMinesScene *m_scene = nullptr;
    XMinesView *m_view = nullptr;
private Q_SLOTS:
    void onNewGame();
    void onGamePauseed(bool);
    void onDifficultyChanged(int);
    void onMinesCountChanged(int);
    void advanceTime();
    void onGameOver(bool);

private:
    void sizeFitoLevel();

private:
    QStatusBar *m_status;
    QLabel *m_mineLabel;
    QLabel *m_timeLabel;
    QComboBox *m_levelCbx;
    QAction* m_newGameAction;
    QAction* m_PauseAction;
    QTimer *m_timer;
    int m_recordTime = 0;

    int m_level = 0;
    void setMenu();
    void setActions();
    void setStatusBar();
};
#endif
