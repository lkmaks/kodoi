#ifndef BOARDHANDLER_H
#define BOARDHANDLER_H

#include <vector>
#include <QGraphicsItem>
using std::vector, std::pair;

class BoardHandler
{
private:
    vector<pair<int, int> > position;
    vector<QGraphicsItem*> position_stones;
public:
    BoardHandler();
};

#endif // BOARDHANDLER_H
