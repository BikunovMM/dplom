#ifndef SLIDERSTYLE_H
#define SLIDERSTYLE_H

#include <QProxyStyle>
#include <QStyle>
#include <QStyleOptionSlider>

class SliderStyle : public QProxyStyle {
    int styleHint(StyleHint hint,
                  const QStyleOption *opt = nullptr,
                  const QWidget *widget = nullptr,
                  QStyleHintReturn *rtdata = nullptr) const override;
};

#endif /* SLIDERSTYLE_H */
