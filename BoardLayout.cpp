#include "BoardLayout.h"
#include <QDebug>

BoardLayout::BoardLayout(QWidget *parent, int margin, int spacing)
    : QLayout(parent)
{
    setMargin(margin);
    setSpacing(spacing);
}

BoardLayout::BoardLayout(int spacing)
{
    setSpacing(spacing);
}


BoardLayout::~BoardLayout()
{
    QLayoutItem *l;
    while ((l = takeAt(0)))
        delete l;
}

void BoardLayout::addItem(QLayoutItem *item)
{
    add(item, Board);
}

void BoardLayout::addWidget(QWidget *widget, Position position)
{
    add(new QWidgetItem(widget), position);
}

Qt::Orientations BoardLayout::expandingDirections() const
{
    return Qt::Horizontal | Qt::Vertical;
}

bool BoardLayout::hasHeightForWidth() const
{
    return false;
}

int BoardLayout::count() const
{
    return list.size();
}

QLayoutItem *BoardLayout::itemAt(int index) const
{
    ItemWrapper *wrapper = list.value(index);
    if (wrapper)
        return wrapper->item;
    else
        return 0;
}

QSize BoardLayout::minimumSize() const
{
    return calculateSize(MinimumSize);
}


int cntt = 0;
void BoardLayout::setGeometry(const QRect &rect)
{
    qDebug() << "layout setGeometry" << endl;
    QLayout::setGeometry(rect);
    int d = qMin(rect.width() - 175, rect.height() - 100);
    for (auto e : list) {
        if (e->position == Board) {
            e->item->setGeometry({50, 50, d, d});
        }
        else if (e->position == Bar) {
            e->item->setGeometry({50 + d + 25, 50, 60, d});
        }
    }
}

QSize BoardLayout::sizeHint() const
{
    return calculateSize(SizeHint);
}

QLayoutItem *BoardLayout::takeAt(int index)
{
    if (index >= 0 && index < list.size()) {
        ItemWrapper *layoutStruct = list.takeAt(index);
        return layoutStruct->item;
    }
    return 0;
}

void BoardLayout::add(QLayoutItem *item, Position position)
{
    list.append(new ItemWrapper(item, position));
}

QSize BoardLayout::calculateSize(SizeType sizeType) const
{
    //qDebug() << "calcsize " << (sizeType == MinimumSize) << ": " << endl;
    int d = 0;
    for (int i = 0; i < list.size(); ++i) {
        ItemWrapper *wrapper = list.at(i);
        Position position = wrapper->position;
        QLayoutItem *item = wrapper->item;

        QSize item_size = (sizeType == MinimumSize ? item->minimumSize() : item->sizeHint());

        if (position == Board) {
            d = qMax(d, qMax(item_size.width(),
                             item_size.height()));
        }
        else {
            d = qMax(d, item_size.height());
        }
    }

    //qDebug() << QSize({50 + d + 25 + 50 + 50, 50 + d + 50}) << endl;;

    return {50 + d + 25 + 50 + 50, 50 + d + 50};
}
