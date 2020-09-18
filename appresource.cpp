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

}
