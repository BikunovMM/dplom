#ifndef SHOWHIDEBTN_H
#define SHOWHIDEBTN_H

#include <array>

#include <QLineEdit>
#include <QPushButton>
#include <QPainter>
#include <QPainterPath>

constexpr float LINE_OFFSET_PERCENT = 0.1f;
constexpr float PADDING_OFFSET_PERCENT = 0.2f;

class ShowHideBtn : public QPushButton
{
    Q_OBJECT

public:
    ShowHideBtn(QWidget *parent = nullptr);
    ~ShowHideBtn();

signals:
    void showHide(bool hidden);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    bool is_hidden;
    std::array<QPainterPath, 3> off_paths;
    std::array<QPainterPath, 2> on_paths;
};

#endif /* SHOWHIDEBTN_H */
