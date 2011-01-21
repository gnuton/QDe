#ifndef __TITLEBAR_H
#define __TITLEBAR_H

#include "defs.h"

#define TITLEBAR_HEIGHT  22
#define BORDER_WIDTH 0
#define DBL_BORDER_WIDTH 0

class Border : public QLabel
{
	Q_OBJECT

public:
	Border(QWidget *parent=0);
	~Border();

signals:
	void bMouseMove(QMouseEvent *);
	void bMouseLeftPress(QMouseEvent *);
	void bMouseLeftPress();
	void bMouseRightPress();
	void bMouseMidPress();
	void bMouseRelease();

protected:
	void mousePressEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);
};

class WinButton : public QLabel
{
	Q_OBJECT

public:

	enum ButtonState {
		normalState,
		activeState,
		inactiveState,
	};

	enum ButtonType {
		CloseButton,
		MinimizeButton,
		MaximizeButton,
		ExpandButton
	};

	WinButton(int buttonType, QWidget *parent = 0);
	~WinButton();

public slots:
	void changeState(int s);

signals:
	void wMouseLeftPress();
	void wMouseRightPress();
	void wMouseMidPress();

protected:
	void mousePressEvent(QMouseEvent *);
	virtual void enterEvent(QEvent *);
	virtual void leaveEvent(QEvent *);
	virtual void paintEvent(QPaintEvent *);

private:
	QPixmap *normal;
	QPixmap *active;
	QPixmap *inactive;
	int state, oldState, m_buttonType;
};

class GenericBar : public QWidget
{
	Q_OBJECT
public:
	enum BorderState {
		borderActive,
		borderInactive
	};

	enum BorderRole {
		normalWindow,
		dialogWindow,
		inspectorWindow,
	};
	
	enum CursorPos {
		NonePos,
		LeftPos,
		MiddlePos,
		RightPos,
	};

	GenericBar(QWidget *parent = 0);
	~GenericBar();
	virtual void changeState(int);
	
	int cursorPos;

signals:
        void tMouseRelease();
        void tMouseMove(QMouseEvent *);
        void tMouseLeftPress(QMouseEvent *);
        void tMouseRightPress();
        void tMouseMidPress();
	void tMouseDblClick();
	
protected:
        void mousePressEvent(QMouseEvent *);
        void mouseMoveEvent(QMouseEvent *);
        void mouseReleaseEvent(QMouseEvent *);
        virtual void paintEvent(QPaintEvent *);

protected:
	int borderState;
	bool changeCursor;
	bool posLocked;
        QColor top, bottom;
};

class Titlebar : public GenericBar
{
	Q_OBJECT
public:

	Titlebar(const QPixmap &, const QString &, int _role = 0, QWidget *parent = 0);
	~Titlebar();

	static int getPreferedHeight(int);
	virtual void changeState(int);
	void setTitle(const QString &);

	WinButton *closeBtn, *minBtn, *maxBtn;
	
//signals:
//      void tMouseRelease();
//      void tMouseMove(QMouseEvent *);
//      void tMouseLeftPress(QMouseEvent *);
//      void tMouseRightPress();
//      void tMouseMidPress();
//      void tMouseDblClick();
	
protected:
//      void mousePressEvent(QMouseEvent *);
//      void mouseMoveEvent(QMouseEvent *);
//      void mouseReleaseEvent(QMouseEvent *);
	void mouseDoubleClickEvent(QMouseEvent *);
        void paintEvent(QPaintEvent *);
	
protected:
	QPixmap icon;
	QString title;
	int role;
        QColor topline;

	//int borderState;
};

#endif
