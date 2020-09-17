#include "cellitem.h"

// 模板方法：将一部分动作延迟到子类实现
// 也可以写成纯虚函数，作为一个接口
//void CellItem::updateDisplay()
//{

//}

CellItem::CellItem()
{
//    reset();
}

CellItem::~CellItem()
{

}

void CellItem:: reset()
{
    this->m_display = XMinesState::Init;
    this->m_digit   = 0;
    this->m_hasMine = false;
    updateDisplay();
}
// 标记区域
void CellItem::mark()
{
    if(isFlagged()){
        this->m_display = XMinesState::Question;
    }else if(isQuestioned()){
        this->m_display = XMinesState::Init;
    }else if(isInitialed()){
        this->m_display = XMinesState::Flag;
    }
    updateDisplay();
}
void CellItem::forceMark()
{
    this->m_display = XMinesState::Flag;
    updateDisplay();
}

void CellItem::forceReveal()
{
    this->m_display = XMinesState::Explode;
    updateDisplay();
}
// 探测区域
void CellItem::reveal()
{
    if(!isRevealed() && !isFlagged()){
        this->m_display = (hasMine()) ? XMinesState::Explode : (XMinesState::CellDisplay)m_digit;
    }
    updateDisplay();
}
