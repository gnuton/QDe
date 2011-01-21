#ifndef __GENERIC_BUTTON_H
#define __GENERIC_BUTTON_H

#include "defs.h"

class Panel;

class GenericButton : public QToolButton
{
	Q_OBJECT
public:

	enum ButtonType {APixmap, AText};

	explicit GenericButton(Panel *p, QWidget *parent=0);
	virtual ~GenericButton(void);

	void setImages(const QString &normal, const QString &active);
	void setImages(const QPixmap &normal, const QPixmap &active);

	// various helper routines for poor Antico WM integration
	virtual bool canActivated();
	virtual void activate();
	virtual void deactivate();

public slots:
	void showMenu();
	
protected:
	virtual void paintEvent(QPaintEvent *);
	virtual void enterEvent(QEvent *);
	virtual void leaveEvent(QEvent *);
	virtual void mousePressEvent(QMouseEvent *);
	
	void paintButton();
	void paintText();
	void paintIcon();

	bool m_Focus, m_forceFocus;
	bool can_Activated, activated;
	ButtonType m_type;
	QPixmap focusPixmap;
	
	Panel *panel; 

};

#endif
