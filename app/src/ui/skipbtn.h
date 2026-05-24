#ifndef SKIPBTN_H
#define SKIPBTN_H

#include <QPushButton>
#include <QPainter>
#include <QMouseEvent>
#include <QPainterPath>

#include "roundbtn.h"

constexpr int ARROWS_OFFSET = 10;//12
constexpr int R_ARROW_X_OFFSET = 2;
constexpr int L_ARROW_X_OFFSET = 2 * R_ARROW_X_OFFSET;

class SkipBtn : public RoundBtn
{
    Q_OBJECT

public:
    explicit SkipBtn(bool isrigth, QWidget *parent = nullptr);
    ~SkipBtn();

protected:
    void drawContent(QPainter& painter) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    bool is_right;
    QPainterPath arrows_ph0;
};

#endif /* SKIPBTN_H */
