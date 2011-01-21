////////////////////////////////////////
//  File      : defs.h                //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////
#ifndef __DEFS_H
#define __DEFS_H

#include <QApplication>
#include <QPushButton>
#include <QMap>
#include <QLabel>
#include <QPainter>
#include <QPalette>
#include <QCheckBox>
#include <QVariant>
#include <QCursor>
#include <QTextStream>
#include <QFile>
#include <QFileInfo>
#include <QColor>
#include <QBitmap>
#include <QImage>
#include <QPixmap>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QHash>
#include <QSizePolicy>
#include <QAction>
#include <QVariant>
#include <QMenu>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QIcon>
#include <QWidget>
#include <QSpinBox>
#include <QByteArray>
#include <QBrush>
#include <QFrame>
#include <QX11Info>
#include <QDesktopWidget>
#include <QDebug>
#include <QProcess>
#include <QDir>
#include <QSettings>
#include <QTabWidget>
#include <QDirModel>
#include <QTreeView> 
#include <QMessageBox>
#include <QStringList>
#include <QListWidget>
#include <QSignalMapper>
#include <QFileDialog>
#include <QColorDialog>
#include <QGroupBox>
#include <QSpacerItem>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QTimer>
#include <QDateTime>
#include <QStyleFactory>
#include <QToolBox>
#include <QToolButton>
#include <QShortcut>
#include <QX11EmbedContainer>

#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/X.h>
#include <X11/Xutil.h>
#include <X11/extensions/shape.h>
#include <X11/keysym.h>

#define display() QX11Info::display()
#define rootWindow() QX11Info::appRootWindow()

#endif
