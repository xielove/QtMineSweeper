#ifndef XCELLITEM_H
#define XCELLITEM_H

#include <QGraphicsPixmapItem>
#include "cellitem.h"

class XCellItem : public CellItem, public QGraphicsPixmapItem
{
public:
    XCellItem(QGraphicsItem *parent);
    virtual ~XCellItem();
    void updateDisplay();

    // 重载函数，在进行操作后需要更新显示的图片
    void reset() override;
    void mark() override;
    void forceMark() override;
    void forceReveal() override;
    void reveal() override;
};

#endif // XCELLITEM_H
