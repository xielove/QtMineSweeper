#include "xcellitem.h"
#include "QDebug"

bool XCellItem::isInitRes = false;
QVector<QPixmap> XCellItem::pixs;

XCellItem::~XCellItem()
{

}


XCellItem::XCellItem(QGraphicsItem *parent)
   :QGraphicsPixmapItem(parent)
{
   qDebug() << "XCellItem::XCellItem(QGraphicsItem *parent)";
   if(!isInitRes){
       QString prefix = ":/imgs/img30";
       QVector<QString> fileNames;
       fileNames.append("blank.png");
       fileNames.append("1.png");
       fileNames.append("2.png");
       fileNames.append("3.png");
       fileNames.append("4.png");
       fileNames.append("5.png");
       fileNames.append("6.png");
       fileNames.append("7.png");
       fileNames.append("8.png");
       fileNames.append("explode.png");    // 9
       fileNames.append("initial.png");    // 10
       fileNames.append("question.png");   // 11
       fileNames.append("flag.png");       // 12
       for(int i = MinesState::Zero; i <= MinesState::Flag; i++){
           pixs.push_back(QPixmap(prefix+"/"+fileNames[i]));
       }
       isInitRes = true;
   }
   qDebug() << "XCellItem::XCellItem(QGraphicsItem *parent)";
}

void XCellItem::reset()
{
    CellItem::reset();
    updateDisplay();
}
void XCellItem::mark()
{
    CellItem::mark();
    updateDisplay();
}
void XCellItem::forceMark()
{
    CellItem::forceMark();
    updateDisplay();
}

void XCellItem::forceReveal()
{
    CellItem::forceReveal();
    updateDisplay();
}
void XCellItem::reveal()
{
    CellItem::reveal();
    updateDisplay();
}

void XCellItem::updateDisplay()
{
    this->setPixmap(pixs[display()]);
}
