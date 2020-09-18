#include "xcellitem.h"
#include "appresource.h"


XCellItem::~XCellItem()
{

}


XCellItem::XCellItem(QGraphicsItem *parent)
   :QGraphicsPixmapItem(parent)
{

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
    this->setPixmap(AppResource::pixmaps[display()]);
}
