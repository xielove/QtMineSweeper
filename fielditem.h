#ifndef MINEFIELD_H
#define MINEFIELD_H

#include <utility>
#include <vector>
#include <list>
#include <QGraphicsObject>

#include "cellitem.h"
#include "xcellitem.h"
#include "commondef.h"

class FieldItem
{
    friend class FieldAction;
private:
    FieldArgs m_args;
    std::vector<CellItem *> m_cellitems;
    std::vector<int> m_randomPos;

    int transToIndex(int row, int col);
    int transToIndex(FieldPos pos);

    std::list<CellItem*> neighbors(FieldPos pos);
    std::list<FieldPos>  neighborsPos(FieldPos pos);

public:
    inline int colsCount() const {return m_args.rows;}
    inline int rowsCount() const {return m_args.cols;}
    inline int minesCount() const {return m_args.mines;}
    inline FieldArgs fieldArgs() const {return m_args;}
    inline int cellItemsCount() const {return colsCount()*rowsCount();}

    FieldPos itemPos(CellItem *item);
    CellItem *itemAt(int row, int col);
    CellItem *itemAt(FieldPos pos);

};

class FieldAction
{
private:
    FieldItem *m_field = nullptr;

public:
    FieldAction(FieldItem *field);

    void generateField(FieldPos fp);
    void revealAllMines();
    void markAllMines();
    int revealEmptySpace(FieldPos pos);
    void updateCellsDigit();
    void resetAllCells();
    void setFieldArgs(FieldArgs args) {
        m_field->m_args = args;
    }
    void setCellItems(std::vector<CellItem*> cellitems) {
        this->m_field->m_cellitems = cellitems;
    }
    void clearField();
};
#endif /* MINEFIELD_H */
