#ifndef SLICEDBTN_H
#define SLICEDBTN_H

#include <iostream>

#include <QPushButton>
#include <QPainter>
#include <QMouseEvent>

constexpr int X_OFFSET = 8; // full_offset = 16

class SlicedBtn : public QPushButton
{
    Q_OBJECT

public:
    explicit SlicedBtn(QString ltext, QString rtext,
                       QWidget *parent = nullptr);
    ~SlicedBtn();

    void setText(const QString& left_text,
                 const QString& right_text);

signals:
    void leftClicked();
    void rightClicked();

protected:
    void paintEvent(QPaintEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;    

private:
    QString ltext;
    QString rtext;
    bool is_lhovered;
    bool is_rhovered;
    bool last_lhovered;
    bool last_rhovered;
};

#endif /* SLICEDBTN_H */
//234 234 234
//229 229 229
//206 206 206
