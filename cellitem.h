#ifndef CELLITEM_H
#define CELLITEM_H

//#include "commondef.h"
#include "appresource.h"

class CellItem
{
private:
    CellDisplay m_display = CellDisplay::Init;
    bool m_hasMine = false;
    int m_digit = 0;
public:
    // getter and setter
    inline bool hasMine() const {return m_hasMine;}
    inline int dight() const {return m_digit;}
    inline CellDisplay display(){return m_display;}

    // 是否是初始状态
    inline bool isInitialed() const {
        return m_display == CellDisplay::Init;
    }
    // 是否探测该方格
    inline bool isRevealed() const {
        return m_display >= CellDisplay::Zero && m_display <= CellDisplay::Explode;
    }
    // 是否使用旗子标记
    inline bool isFlagged() const {
        return m_display == CellDisplay::Flag;
    }
    // 是否使用问好标记
    inline bool isQuestioned() const {
        return m_display == CellDisplay::Question;
    }

    inline void setHasMine(bool hasMine){this->m_hasMine = hasMine;}
    inline void setDight(int d){this->m_digit = d;}

public:
    CellItem();
    virtual ~CellItem();
    virtual void reset();
    // 标记区域
    virtual void mark();
    virtual void forceMark();
    // 探测区域
    virtual void forceReveal();
    virtual void reveal();
};

#endif /* CELLITEM_H */
