#include "cellitem.h"

CellItem::CellItem()
{

}

CellItem::~CellItem()
{

}

void CellItem::reset()
{
    this->m_display = CellDisplay::Init;
    this->m_digit   = 0;
    this->m_hasMine = false;
}
void CellItem::mark()
{
    if(isFlagged()){
        this->m_display = CellDisplay::Question;
    }else if(isQuestioned()){
        this->m_display = CellDisplay::Init;
    }else if(isInitialed()){
        this->m_display = CellDisplay::Flag;
    }
}
void CellItem::forceMark()
{
    this->m_display = CellDisplay::Flag;
}

void CellItem::forceReveal()
{
    this->m_display = CellDisplay::Explode;
}
void CellItem::reveal()
{
    if(!isRevealed() && !isFlagged()){
        this->m_display = (hasMine()) ? CellDisplay::Explode : (CellDisplay)m_digit;
    }
}
