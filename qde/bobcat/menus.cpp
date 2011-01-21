#include <QtGui>
#include "bobcat.h"
#include "macros.h"

static const char * const qt_cleanlooks_menuitem_checkbox_checked[] = {
    "8 7 6 1",
    " 	g None",
    ".	g #959595",
    "+	g #676767",
    "@	g #454545",
    "#	g #1D1D1D",
    "0	g #101010",
    "      ..",
    "     .+ ",
    "    .+  ",
    "0  .@   ",
    "@#++.   ",
    "  @#    ",
    "   .    "};

void Style::drawMenuBar(const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
        QRect rect = option->rect;

        painter->save();
        QLinearGradient gradient(QPointF(0, rect.top()), QPointF(0, rect.top()+widget->height()));
        gradient.setColorAt(0, QColor(0xb3, 0xb3, 0xb3));
        gradient.setColorAt(1, QColor(0xa2, 0xa2, 0xa2));
        painter->fillRect(rect, gradient);
        painter->restore();
}

void Style::drawMenuBarItem(const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
        ASSURE_OPTION(mbi, MenuItem);

        painter->save();

        QStyleOptionMenuItem it = *mbi;
        it.rect = mbi->rect.adjusted(0, 3, 0, -1);

        drawMenuBar(option, painter, widget);

        bool act = mbi->state & State_Selected && mbi->state & State_Sunken;
        bool dis = !(mbi->state & State_Enabled);

        QPalette::ColorRole textRole = act ? QPalette::HighlightedText : QPalette::Text;

        if (act) {
                QColor highlight = option->palette.highlight().color();
                QColor outline = highlight.lighter(115);

                QRect r = option->rect;

                // border
                painter->setPen(QPen(outline, 0));
                painter->drawLine(QPoint(r.left(), r.top()+1), QPoint(r.left(), r.bottom()));
                painter->drawLine(QPoint(r.right(), r.top()+1), QPoint(r.right(), r.bottom()));
                painter->drawLine(QPoint(r.left()+1, r.top()), QPoint(r.right()-1, r.top()));

                // selection
                QLinearGradient gradient(QPointF(0, r.top()), QPointF(0, r.top()+widget->height()));
                gradient.setColorAt(0, highlight.lighter(135));
                gradient.setColorAt(1, highlight);
                painter->fillRect(r.adjusted(1, 1, -1, 0), gradient);

                // text color
                textRole = dis ? QPalette::Text : QPalette::HighlightedText;
        }

        uint alignment = Qt::AlignCenter | Qt::TextShowMnemonic | Qt::TextDontClip | Qt::TextSingleLine;
        if (!styleHint(SH_UnderlineShortcut, mbi, widget))
                alignment |= Qt::TextHideMnemonic;

        drawItemText(painter, it.rect, alignment, mbi->palette, mbi->state & State_Enabled, mbi->text, textRole);

        painter->restore();
}

void Style::drawMenuItem(const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
        // Copy / Paste / Modified from QCleanlooks style

        // Draws one item in a popup menu.
        ASSURE_OPTION(mi, MenuItem);

        painter->save();

        QColor menuBackground = option->palette.background().color().lighter(140);
        QRect r = mi->rect;

        if (mi->menuItemType == QStyleOptionMenuItem::Separator) {
                painter->fillRect(r, menuBackground);
                int w = 0;
                if (!mi->text.isEmpty()) {
                    painter->setFont(mi->font);
                    drawItemText(painter, r.adjusted(5, 0, -5, 0), Qt::AlignLeft | Qt::AlignVCenter,
                                 mi->palette, mi->state & State_Enabled, mi->text,
                                 QPalette::Text);
                    w = mi->fontMetrics.width(mi->text) + 5;
                }
                painter->setPen(option->palette.base().color().darker(180));
                bool reverse = mi->direction == Qt::RightToLeft;
                painter->drawLine(r.left() + 5 + (reverse ? 0 : w), r.center().y(),
                                  r.right() - 5 - (reverse ? w : 0), r.center().y());
                painter->restore();
                return;
        }

        bool selected = mi->state & State_Selected && mi->state & State_Enabled;
        if (selected) {
                QRect r1 = option->rect;
                QColor highlight = option->palette.highlight().color();
                QColor outline = highlight.lighter(115);

                DRAW_GRADIENT(QPointF(0, r1.top()), QPointF(0, r1.bottom()),
                              highlight.lighter(135), highlight, r1);

                //r1 = r1.adjusted(-1, 0, 1, 0);
                painter->setPen(QPen(outline, 0));
                painter->drawLine(QPoint(r1.left(), r1.bottom()), QPoint(r1.right(), r1.bottom()));
                painter->drawLine(QPoint(r1.left(), r1.top()), QPoint(r1.right(), r1.top()));
        } else {
                painter->fillRect(option->rect, menuBackground);
        }

        bool checkable = mi->checkType != QStyleOptionMenuItem::NotCheckable;
        bool checked = mi->checked;
        bool sunken = mi->state & State_Sunken;
        bool enabled = mi->state & State_Enabled;

        bool ignoreCheckMark = false;
        int checkcol = qMax(mi->maxIconWidth, 20);

#ifndef QT_NO_COMBOBOX
        if (qobject_cast<const QComboBox*>(widget))
                ignoreCheckMark = true; //ignore the checkmarks provided by the QComboMenuDelegate
#endif

        if (!ignoreCheckMark) {
                // Check
                QRect checkRect(option->rect.left() + 7, option->rect.center().y() - 6, 13, 13);
                checkRect = visualRect(mi->direction, r, checkRect);
                if (checkable) {
                        if (mi->checkType & QStyleOptionMenuItem::Exclusive) {
                        // Radio button
                                if (checked || sunken) {
                                        painter->setRenderHint(QPainter::Antialiasing);
                                        painter->setPen(Qt::NoPen);

                                        QPalette::ColorRole textRole = !enabled ? QPalette::Text:
                                                        selected ? QPalette::HighlightedText : QPalette::ButtonText;
                                        painter->setBrush(option->palette.brush( option->palette.currentColorGroup(), textRole));
                                        painter->drawEllipse(checkRect.adjusted(4, 4, -4, -4));
                                }
                } else {
                        // Check box
                        if (mi->icon.isNull()) {
                                if (checked || sunken) {
                                QImage image(qt_cleanlooks_menuitem_checkbox_checked);
                                if (enabled && (mi->state & State_Selected)) {
                                    image.setColor(1, 0x55ffffff);
                                    image.setColor(2, 0xAAffffff);
                                    image.setColor(3, 0xBBffffff);
                                    image.setColor(4, 0xFFffffff);
                                    image.setColor(5, 0x33ffffff);
                                } else {
                                    image.setColor(1, 0x55000000);
                                    image.setColor(2, 0xAA000000);
                                    image.setColor(3, 0xBB000000);
                                    image.setColor(4, 0xFF000000);
                                    image.setColor(5, 0x33000000);
                                }
                                painter->drawImage(QPoint(checkRect.center().x() - image.width() / 2,
                                                        checkRect.center().y() - image.height() / 2), image);
                            }
                        }
                    }
                }
        } else { //ignore checkmark
                if (mi->icon.isNull())
                    checkcol = 0;
                else
                    checkcol = mi->maxIconWidth;
            }

        // Text and icon, ripped from windows style
        bool dis = !(mi->state & State_Enabled);
        bool act = mi->state & State_Selected;
        const QStyleOption *opt = option;
        const QStyleOptionMenuItem *menuitem = mi;

        QPainter *p = painter;
        QRect vCheckRect = visualRect(opt->direction, menuitem->rect,
                                          QRect(menuitem->rect.x(), menuitem->rect.y(),
                                                checkcol, menuitem->rect.height()));
        if (!mi->icon.isNull()) {
                QIcon::Mode mode = dis ? QIcon::Disabled : QIcon::Normal;
                if (act && !dis)
                    mode = QIcon::Active;
                QPixmap pixmap;

                int smallIconSize = pixelMetric(PM_SmallIconSize, option, widget);
                QSize iconSize(smallIconSize, smallIconSize);
#ifndef QT_NO_COMBOBOX
                if (const QComboBox *combo = qobject_cast<const QComboBox*>(widget))
                    iconSize = combo->iconSize();
#endif // QT_NO_COMBOBOX
                if (checked)
                    pixmap = mi->icon.pixmap(iconSize, mode, QIcon::On);
                else
                    pixmap = mi->icon.pixmap(iconSize, mode);

                int pixw = pixmap.width();
                int pixh = pixmap.height();

                QRect pmr(0, 0, pixw, pixh);
                pmr.moveCenter(vCheckRect.center());
                painter->setPen(mi->palette.text().color());
                if (checkable && checked) {
                    QStyleOption opt = *option;
                    if (act) {
                        QColor activeColor = mergeColors(option->palette.background().color(),
                                                        option->palette.highlight().color());
                        opt.palette.setBrush(QPalette::Button, activeColor);
                    }
                    opt.state |= State_Sunken;
                    opt.rect = vCheckRect;
                    drawPrimitive(PE_PanelButtonCommand, &opt, painter, widget);
                }
                painter->drawPixmap(pmr.topLeft(), pixmap);
            }
        if (selected) {
                painter->setPen(mi->palette.highlightedText().color());
        } else {
                painter->setPen(mi->palette.text().color());
        }

        int x, y, w, h;
        menuitem->rect.getRect(&x, &y, &w, &h);
        int tab = menuitem->tabWidth;
        QColor discol;
        if (dis) {
                discol = menuitem->palette.text().color();
                p->setPen(discol);
            }
        int xm = windowsItemFrame + checkcol + windowsItemHMargin;
        int xpos = menuitem->rect.x() + xm;

        QRect textRect(xpos, y + windowsItemVMargin, w - xm - windowsRightBorder - tab + 1, h - 2 * windowsItemVMargin);
        QRect vTextRect = visualRect(opt->direction, menuitem->rect, textRect);
        QString s = menuitem->text;
        if (!s.isEmpty()) {                     // draw text
                p->save();
                int t = s.indexOf(QLatin1Char('\t'));
                int text_flags = Qt::AlignVCenter | Qt::TextShowMnemonic | Qt::TextDontClip | Qt::TextSingleLine;
                if (!styleHint(SH_UnderlineShortcut, menuitem, widget))
                    text_flags |= Qt::TextHideMnemonic;
                text_flags |= Qt::AlignLeft;
                if (t >= 0) {
                    QRect vShortcutRect = visualRect(opt->direction, menuitem->rect,
                                                     QRect(textRect.topRight(), QPoint(menuitem->rect.right(), textRect.bottom())));
                    if (dis && !act && styleHint(SH_EtchDisabledText, option, widget)) {
                        p->setPen(menuitem->palette.light().color());
                        p->drawText(vShortcutRect.adjusted(1, 1, 1, 1), text_flags, s.mid(t + 1));
                        p->setPen(discol);
                    }
                    p->drawText(vShortcutRect, text_flags, s.mid(t + 1));
                    s = s.left(t);
                }
                QFont font = menuitem->font;

                if (menuitem->menuItemType == QStyleOptionMenuItem::DefaultItem)
                    font.setBold(true);

                p->setFont(font);
                if (dis && !act && styleHint(SH_EtchDisabledText, option, widget)) {
                    p->setPen(menuitem->palette.light().color());
                    p->drawText(vTextRect.adjusted(1, 1, 1, 1), text_flags, s.left(t));
                    p->setPen(discol);
                }
                p->drawText(vTextRect, text_flags, s.left(t));
                p->restore();
        }

        // Arrow
        if (mi->menuItemType == QStyleOptionMenuItem::SubMenu) {// draw sub menu arrow
                int dim = (mi->rect.height() - 4) / 2;
                PrimitiveElement arrow;
                arrow = QApplication::isRightToLeft() ? PE_IndicatorArrowLeft : PE_IndicatorArrowRight;
                int xpos = mi->rect.left() + mi->rect.width() - 3 - dim;
                QRect  vSubMenuRect = visualRect(option->direction, mi->rect,
                                                 QRect(xpos, mi->rect.top() + mi->rect.height() / 2 - dim / 2, dim, dim));
                QStyleOptionMenuItem newMI = *mi;
                newMI.rect = vSubMenuRect;
                newMI.state = !enabled ? State_None : State_Enabled;
                if (selected)
                    newMI.palette.setColor(QPalette::ButtonText,
                                           newMI.palette.highlightedText().color());
                drawPrimitive(arrow, &newMI, painter, widget);
        }
        painter->restore();
}
