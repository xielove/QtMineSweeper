#ifndef APPRESOURCE_H
#define APPRESOURCE_H

#include <QString>
#include <QPixmap>
#include <QVector>
#include <QStringList>
#include <QIcon>
#include <utility>

typedef std::pair<int, int> FieldPos;

struct FieldArgs
{
    int rows;
    int cols;
    int mines;

    static const FieldArgs Low;
    static const FieldArgs Medium;
    static const FieldArgs High;
};

enum CellDisplay{
    Zero, One, Two, Three, Four, Five, Six, Seven, Eight, Explode,
    Init, Question, Flag
};

class AppResource
{
public:
    static void initAppRes();
    static QStringList levels;
    static int levelsCount;
    static QStringList filenames;
    static QVector<QPixmap> pixmaps;
    static QString prefix;
//    static QPixmap background;
    static QString iconPrefix;
    static QIcon logo;
    static QIcon newIcon;
    static QIcon stopIcon;
    static QIcon exitIcon;

};

#endif // APPRESOURCE_H
