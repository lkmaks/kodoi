#include "BoardLayout.h"
#include <QDebug>

BoardLayout::BoardLayout(QWidget *parent, Config *config, int margin, int spacing)
    : QLayout(parent), config_(config)
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
    return list_.size();
}

QLayoutItem *BoardLayout::itemAt(int index) const
{
    ItemWrapper *wrapper = list_.value(index);
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

    int dhor = rect.width();
    dhor -= config_->board_layout_bar_width;
    dhor -= config_->board_layout_hpad1;
    dhor -= config_->board_layout_hpad2;
    dhor -= config_->board_layout_info_widget_width;

    int dvert = rect.height();
    dvert -= 2 * config_->board_layout_vpad;

    int d = qMin(dhor, dvert);
    int rest_w = (dhor - d);

    for (auto e : list_) {
        if (e->position == Bar) {
            e->item->setGeometry({rest_w / 2, config_->board_layout_vpad,
                                  config_->board_layout_bar_width, d});
        }
        if (e->position == Board) {
            int x = rest_w / 2 + config_->board_layout_bar_width + config_->board_layout_hpad1;
            e->item->setGeometry({x, config_->board_layout_vpad,
                                  d, d});
        }
        else if (e->position == InfoWidget) {
            int x = rest_w / 2 +
                    config_->board_layout_bar_width + config_->board_layout_hpad1 +
                    d + config_->board_layout_hpad2;
            e->item->setGeometry({x, config_->board_layout_vpad,
                                  config_->board_layout_info_widget_width, d});
        }
    }
}

QSize BoardLayout::sizeHint() const
{
    return calculateSize(SizeHint);
}

QLayoutItem *BoardLayout::takeAt(int index)
{
    if (index >= 0 && index < list_.size()) {
        ItemWrapper *layoutStruct = list_.takeAt(index);
        return layoutStruct->item;
    }
    return 0;
}

void BoardLayout::add(QLayoutItem *item, Position position)
{
    list_.append(new ItemWrapper(item, position));
}

QSize BoardLayout::calculateSize(SizeType sizeType) const
{
    int d;
    if (sizeType == MinimumSize) {
        d = min_d_;
    }
    else {
        d = want_d_;
    }

    int w = 0;
    w += config_->board_layout_bar_width;
    w += config_->board_layout_hpad1;
    w += d;
    w += config_->board_layout_hpad2;
    w += config_->board_layout_info_widget_width;
    w += config_->board_layout_min_hpad;

    int h = 0;
    h += config_->board_layout_vpad * 2;
    h += d;

    return {w, h};
}
