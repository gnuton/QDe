/***************************************************************************
 *   Copyright (C) 2006-2008 by Tomasz Ziobrowski                          *
 *   http://www.3electrons.com                                             *
 *   e-mail: t.ziobrowski@3electrons.com                                   *
 *                                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

///////////////////////////////////////////////////////////////////////////////
//
// Some modification over original code - removed all unneccessary code for the
// need of AnticoDeluxe project
//
// Cvetoslav Ludmiloff 
// <ludmiloff@gmail.com>
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __WALLCLOCK_H
#define __WALLCLOCK_H
#include <QtGui>
#include <QDateTime> 
#include <QTimer> 
 
class WallClock : public QWidget 
    {
        Q_OBJECT
public:
	WallClock(QWidget *parent = 0);

	QFont digitFont() const    	{ return m_digitFont;}
  	QFont timeFont () const    	{ return m_timeFont; }
	QFont dateFont () const    	{ return m_dateFont; }
	QFont dayFont  () const    	{ return m_dayFont;  }

	void setDigitFont(QFont f) 	{ m_digitFont = f; update();}
	void setTimeFont (QFont f) 	{ m_timeFont  = f; update();}
	void setDateFont (QFont f) 	{ m_dateFont  = f; update();}
	void setDayFont  (QFont f) 	{ m_dayFont   = f; update();}

	int digitOffset () const 	{ return m_digitOffset; }
	int dateOffset () const 	{ return m_dateOffset;  }
	int timeOffset () const 	{ return m_timeOffset;  }
	int dayOffset  () const 	{ return m_dayOffset;   }
	void setDigitOffset(int i)  { m_digitOffset = i; update();}
	void setDateOffset (int i)  { m_dateOffset  = i; update();}
	void setTimeOffset (int i)  { m_timeOffset  = i; update();}
	void setDayOffset  (int i)  { m_dayOffset   = i; update();}

	QColor digitColor() const  	{ return m_digitColor;   }
	QColor dateColor() const   	{ return m_dateColor;    }
	QColor timeColor() const   	{ return m_timeColor;    }
	QColor dayColor()  const   	{ return m_dayColor;     }
	QDateTime dateTime() const  { return m_dateTime;     }
    QDate date()      const   	{ return m_dateTime.date(); }
    QTime time()      const   	{ return m_dateTime.time(); } 
    bool showCurrentDateTime() const { return m_showCurrentDateTime; } 
        
	void setDigitColor(QColor c){ m_digitColor = c; update();}
	void setDateColor(QColor c) { m_dateColor = c;  update();}
	void setTimeColor(QColor c) { m_timeColor = c;  update();}
	void setDayColor (QColor c) { m_dayColor  = c;  update();}
        
public slots:
	void setTime ( const QTime & );
	void setDate ( const QDate & );
	void setDateTime( const QDateTime &);
	void setShowCurrentDateTime(bool showCurrentDateTime);

protected slots:
	void updateTime(); 

signals:
	void minutePassed(QTime);
        
protected:
	void paintEvent(QPaintEvent *event);
	void paintBackground(QPainter & painter);
	void initCoordinateSystem(QPainter & painter);
	 	
	int resetDigitOffset() const { return 75; }
	int resetDateOffset()  const { return 0;  }
	int resetTimeOffset()  const { return -12;}
	int resetDayOffset()   const { return 29; }

	QFont m_digitFont;
	QFont m_dateFont;
	QFont m_timeFont;
	QFont m_dayFont;

	int m_digitOffset;
	int m_dateOffset;
	int   m_timeOffset;
	int   m_dayOffset;

	QColor m_digitColor;
	QColor m_dateColor;
	QColor m_timeColor;
	QColor m_dayColor;
	QDateTime m_dateTime; 
	bool m_showCurrentDateTime; 
	QTimer * m_timer; 
	int seconds;
};
#endif // __WALLCLOCK_H

