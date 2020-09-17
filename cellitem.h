#ifndef CELLITEM_H
#define CELLITEM_H

#include "commondef.h"

class CellItem
{
private:
    XMinesState::CellDisplay m_display = XMinesState::Init;
    bool m_hasMine = false;
    int m_digit = 0;
public:
    // getter and setter
    inline bool hasMine(){return m_hasMine;}
    inline int dight(){return m_digit;}
    inline void setHasMine(bool hasMine){this->m_hasMine = hasMine;}
    inline void setDight(int d){this->m_digit = d;}
    inline int display(){return m_display;}
    inline bool isInitialed(){
        return m_display == XMinesState::Init;
    }
    inline bool isRevealed(){
        return m_display >= XMinesState::Zero && m_display <= XMinesState::Explode;
    }
    inline bool isFlagged(){
        return m_display == XMinesState::Flag;
    }
    inline bool isQuestioned(){
        return m_display == XMinesState::Question;
    }

public:
    CellItem();
    virtual ~CellItem();
    virtual void reset();
    // 标记区域
    virtual void mark();
    void virtual forceMark();
    void virtual forceReveal();
    // 探测区域
    virtual void reveal();
    // 模板方法：将一部分动作延迟到子类实现
    virtual void updateDisplay() = 0;
};

#endif /* CELLITEM_H */
