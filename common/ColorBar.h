#ifndef COLORBAR_H
#define COLORBAR_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsWidget>
#include <Config.h>
#include "EngineWrapper.h"

class QGraphicsRectWidget : public QGraphicsWidget
{
public:
    QGraphicsRectWidget(QColor color);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *,
               QWidget *) override;
private:
    QColor color_;
};


class ColorBar : public QWidget
{
    Q_OBJECT
public:
    explicit ColorBar(Config *config, QWidget *parent = nullptr);

    virtual void resizeEvent(QResizeEvent *event);

    void SmoothSetProportionLevel(qreal p);
    void RapidSetProportionLevel(qreal p);
    void SetTopBlackText(const QString &text);
    void SetBotWhiteText(const QString &text);

    static void PutTextIntoWidth(QGraphicsTextItem *txt, int w);
private:
    Config *config_;

    int bar_w;
    int bar_half_h;
    int level_;

    QGraphicsScene *scene_;
    QGraphicsView *view_;

    QGraphicsRectItem *top_container_;
    QGraphicsRectItem *bot_container_;
    QGraphicsRectItem *mid_rect_;
    QGraphicsRectWidget *black_;
    QGraphicsRectWidget *white_;
    QGraphicsTextItem *top_black_text_;
    QGraphicsTextItem *bot_white_text_;
};

#endif // COLORBAR_H
