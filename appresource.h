#ifndef APPRESOURCE_H
#define APPRESOURCE_H

#include <QString>
#include <QPixmap>
#include <QVector>
#include <QStringList>
#include <QIcon>
#include <utility>

typedef std::pair<int, int> FieldPos;


// 雷区参数
struct FieldArgs
{
    int rows;
    int cols;
    int mines;
};

// 方格的虚拟显示值
enum CellDisplay{
    Zero, One, Two, Three, Four, Five, Six, Seven, Eight, Explode,
    Init, Question, Flag
};

// 游戏资源
class AppResource
{
public:
    static void initAppRes();

    // 难度字符串：简单
    static QStringList levels;
    static int levelsCount;

    // 方格图片资源
    static QStringList filenames;
    static QVector<QPixmap> pixmaps;
    static QString prefix;
//    static QPixmap background;

    // 图标资源
    static QString iconPrefix;
    static QIcon logo;
    static QIcon newIcon;
    static QIcon stopIcon;
    static QIcon exitIcon;


    // 默认大小
    static QSize defaultSceneSize;
    static const int defaultPixmapSize;


    static const FieldArgs Low;
    static const FieldArgs Medium;
    static const FieldArgs High;

    static int hborder;
    static int vborder;

    static const QString helpUrl;
    static const QString copyright;

};

#endif // APPRESOURCE_H
