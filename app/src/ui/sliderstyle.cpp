#include "sliderstyle.h"

int SliderStyle::styleHint(StyleHint hint,
                           const QStyleOption *opt,
                           const QWidget *widget,
                           QStyleHintReturn *rtdata) const
{
    return hint == QStyle::SH_Slider_AbsoluteSetButtons ?
        Qt::LeftButton :
        QProxyStyle::styleHint(hint, opt, widget, rtdata);
}
