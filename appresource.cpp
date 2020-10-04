#include "appresource.h"

QStringList AppResource::levels;
QStringList AppResource::filenames;
QVector<QPixmap> AppResource::pixmaps;
QString AppResource::prefix;
QString AppResource::iconPrefix;
QIcon AppResource::logo;
QIcon AppResource::newIcon;
QIcon AppResource::stopIcon;
QIcon AppResource::exitIcon;
//QPixmap AppResource::background;
int AppResource::levelsCount;

const FieldArgs AppResource::Low{9, 9, 10};
const FieldArgs AppResource::Medium{16, 16, 40};
const FieldArgs AppResource::High{30, 16, 99};

const int AppResource::defaultPixmapSize = 30;
QSize AppResource::defaultSceneSize;

int AppResource::hborder = 0;
int AppResource::vborder = 0;


const QString AppResource::helpUrl = "https://baike.baidu.com/item/%E6%89%AB%E9%9B%B7/12543?fr=aladdin";
const QString AppResource::copyright = "Copyright © 2020 Longyu Xie. All rights reserved.\n\nGithub: https://github.com/xielove/QtMineSweeper";
void AppResource::initAppRes()
{
    levels << "简单" << "中等" << "困难" << "自定义";
    levelsCount = levels.size();
    prefix = ":/res/img30";
    filenames << "blank.png" << "1.png" << "2.png" << "3.png"
              << "4.png" << "5.png" << "6.png"
              << "7.png" << "8.png"
              << "explode.png" << "initial.png" << "question.png" << "flag.png";
    for(int i = CellDisplay::Zero; i <= CellDisplay::Flag; i++){
        pixmaps.append(QPixmap(prefix+"/"+filenames[i]));
    }

//    background = QPixmap(":/res/stoping.jpg");
    iconPrefix = ":/res/icons";
    exitIcon = QIcon(iconPrefix + "/" + "exit.png");
    stopIcon = QIcon(iconPrefix + "/" + "stop.png");
    newIcon = QIcon(iconPrefix + "/" + "add.png");
    logo    = QIcon(iconPrefix + "/" + "logo.png");

    defaultSceneSize = QSize(Low.cols*defaultPixmapSize, Low.rows*defaultPixmapSize);

}
