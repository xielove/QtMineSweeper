#include "cellitem.h"

CellItem::CellItem()
{

}

CellItem::~CellItem()
{

}

void CellItem::reset()
{
    this->m_display = MinesState::Init;
    this->m_digit   = 0;
    this->m_hasMine = false;
}
void CellItem::mark()
{
    if(isFlagged()){
        this->m_display = MinesState::Question;
    }else if(isQuestioned()){
        this->m_display = MinesState::Init;
    }else if(isInitialed()){
        this->m_display = MinesState::Flag;
    }
}
void CellItem::forceMark()
{
    this->m_display = MinesState::Flag;
}

void CellItem::forceReveal()
{
    this->m_display = MinesState::Explode;
}
void CellItem::reveal()
{
    if(!isRevealed() && !isFlagged()){
        this->m_display = (hasMine()) ? MinesState::Explode : (MinesState::CellDisplay)m_digit;
    }
}
