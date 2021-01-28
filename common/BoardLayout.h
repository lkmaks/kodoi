#ifndef BOARDLAYOUT_H
#define BOARDLAYOUT_H

#include <QLayout>
#include <QRect>
#include "Config.h"

class BoardLayout : public QLayout
{
public:
    enum Position { Board, Bar, InfoWidget, LeaveWidget };

    explicit BoardLayout(QWidget *parent, Config *config, int margin = 0, int spacing = -1);
    BoardLayout(int spacing = -1);
    ~BoardLayout();

    void addItem(QLayoutItem *item) override;
    void addWidget(QWidget *widget, Position position);
    Qt::Orientations expandingDirections() const override;
    bool hasHeightForWidth() const override;
    int count() const override;
    QLayoutItem *itemAt(int index) const override;
    QSize minimumSize() const override;
    void setGeometry(const QRect &rect) override;
    QSize sizeHint() const override;
    QLayoutItem *takeAt(int index) override;

    void add(QLayoutItem *item, Position position);

private:
    struct ItemWrapper
    {
        ItemWrapper(QLayoutItem *i, Position p) {
            item = i;
            position = p;
        }

        QLayoutItem *item;
        Position position;
    };

    enum SizeType { MinimumSize, SizeHint };
    QSize calculateSize(SizeType sizeType) const;

    Config *config_;
    int min_d_ = 100;
    int want_d_ = 100;
    QList<ItemWrapper *> list_;
};

#endif // BOARDLAYOUT_H
