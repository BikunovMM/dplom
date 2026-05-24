#include "showhidebtn.h"

ShowHideBtn::ShowHideBtn(QWidget *parent)
    : QPushButton(parent),
      is_hidden(true)
{

}

ShowHideBtn::~ShowHideBtn()
{

}

void ShowHideBtn::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (is_hidden) {
        QPen pen(Qt::black, 2);

        painter.setBrush(Qt::lightGray);
        painter.setPen(pen);
        painter.drawPath(on_paths[0]);

        painter.fillPath(on_paths[1], Qt::black);
    }
    else {
        QPen pen(Qt::black, 2);

        painter.setBrush(Qt::lightGray);
        painter.setPen(pen);

        painter.drawPath(off_paths[0]);

        painter.setBrush(Qt::NoBrush);
        painter.drawPath(off_paths[1]);

        painter.fillPath(off_paths[2], Qt::black);
    }
}

void ShowHideBtn::mouseReleaseEvent(QMouseEvent *event)
{
    is_hidden = !is_hidden;

    update();

    emit showHide(is_hidden);

    QPushButton::mouseReleaseEvent(event);
}

void ShowHideBtn::resizeEvent(QResizeEvent *event)
{
    (void)event;

    off_paths[0].clear();
    off_paths[1].clear();
    off_paths[2].clear();

    on_paths[0].clear();
    on_paths[1].clear();

    int w = width();
    int h = height();
    float hw = w/2;
    float hh = h/2;

    //
    //  OFF_PATHS
    //

    off_paths[0].moveTo(0, hh);
    off_paths[0].quadTo(hw, 0, w, hh);
    off_paths[0].quadTo(hw, h, 0, hh);

    off_paths[1].moveTo(0, hh);
    off_paths[1].quadTo(hw, h/1.5f, w, hh);

    float off_lw = w * LINE_OFFSET_PERCENT;
    float off_lh = h * LINE_OFFSET_PERCENT;
    float off_bw = w * PADDING_OFFSET_PERCENT;
    float off_bh = h * PADDING_OFFSET_PERCENT;

    off_paths[2].moveTo(off_bw, off_lh);
    off_paths[2].lineTo(off_lw, off_bh);
    off_paths[2].lineTo(w - off_bw, h - off_lh);
    off_paths[2].lineTo(w - off_lw, h - off_bh);
    off_paths[2].closeSubpath();

    //
    //  ON_PATHS
    //

    on_paths[0].moveTo(0, hh);
    on_paths[0].quadTo(hw, 0, w, hh);
    on_paths[0].quadTo(hw, h, 0, hh);

    on_paths[1].addEllipse(hw - w/5.6f, hh - h/5.6f, w/2.8f, h/2.8f);
}
