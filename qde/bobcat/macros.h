#ifndef __BOBCATMACRO_H
#define __BOBCATMACRO_H

#include <QtGlobal>

// borrowed from great Bespin style

#define registerPE(_FUNC_, _ELEM_) primitiveFunction[QStyle::_ELEM_] = &Style::_FUNC_
#define registerCE(_FUNC_, _ELEM_) controlFunction[QStyle::_ELEM_] = &Style::_FUNC_

#define ASSURE_OPTION(_VAR_, _TYPE_) \
const QStyleOption##_TYPE_ *_VAR_ = qstyleoption_cast<const QStyleOption##_TYPE_ *>(option);\
if (!_VAR_) return


#define DRAW_GRADIENT(_TOP_, _BOTTOM_, _TOPCOLOR_, _BOTTOMCOLOR_, _RECT_)\
        QLinearGradient gradient(_TOP_, _BOTTOM_);\
        gradient.setColorAt(0, _TOPCOLOR_);\
        gradient.setColorAt(1, _BOTTOMCOLOR_);\
        painter->fillRect(_RECT_, gradient)

#endif
