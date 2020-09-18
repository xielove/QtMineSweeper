#ifndef COMMONDEF_H
#define COMMONDEF_H

#include <utility>
#include <string>
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

namespace MinesState
{
    enum CellState {Initial, Opened, Flagged};
    enum CellDisplay{
        Zero, One, Two, Three, Four, Five, Six, Seven, Eight, Explode,
        Init, Question, Flag
    };
}

class GameResource
{

};


#endif // COMMONDEF_H
