#ifndef __WALLPAPER_H
#define __WALLPAPER_H

#include <QtGui>
#include "prefwidget.h"
#include "ui_wallpaper.h"

class Wallpaper : public PrefWidget
{
	Q_OBJECT
public:	
	Wallpaper(QWidget *parent = 0);
	~Wallpaper();

	void readDirectoryContent(QDir *dir);
	virtual void readSettings();
	virtual bool saveSettings();

public slots:
	void onSelectWallpaper(QListWidgetItem *);
	void onSelectFolder(QListWidgetItem *);
	void setCurrentWallpaper(const QString &path, bool notify = false);
	void selectDir();
	void addDir(const QString &directory);
	void removeDir();

protected:
	QList <QDir *> folders;
	QString currentPath; 

private:
	Ui::wallpaperFrm ui;
};

#endif

