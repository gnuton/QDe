#include "wallpaper.h"
#include <Mime>
#include <Utils>
#include <QtDBus>

Wallpaper::Wallpaper(QWidget *parent)
	: PrefWidget(parent)
{
	ui.setupUi(this);
	moduleName = "Desktop Wallpaper";
	ui.content->setLayoutMode(QListView::Batched);
	//ui.content->setBatchSize(12);
	ui.content->setViewMode(QListView::IconMode);
	ui.content->setIconSize(QSize(80, 50));
	ui.content->setGridSize(QSize(85, 55));

	readSettings();
	connect(ui.addBtn, SIGNAL(clicked()), this, SLOT(selectDir()));
	connect(ui.removeBtn, SIGNAL(clicked()), this, SLOT(removeDir()));
	connect(ui.content, SIGNAL(itemActivated(QListWidgetItem *)), this, 
		SLOT(onSelectWallpaper(QListWidgetItem *))); 
	connect(ui.folders, SIGNAL(itemClicked(QListWidgetItem *)), this,
		SLOT(onSelectFolder(QListWidgetItem *)));

/*
     QDBusReply<QStringList> reply = QDBusConnection::sessionBus().interface()->registeredServiceNames();
     if (!reply.isValid()) {
         qDebug() << "Error:" << reply.error().message();
         exit(1);
     }
     foreach (QString name, reply.value())
         qDebug() << name;
*/
}

void Wallpaper::readDirectoryContent(QDir *dir)
{
	if (state == Initializing) return;
	ui.content->clear();
	dir->setFilter(QDir::Files | QDir::Readable | QDir::NoSymLinks);
	QStringList list = dir->entryList();
	AmeMime mime;
	QStringList ext;
	bool f;
	ext << "jpg" << "jpeg" << "png" << "gif" << "tiff" ;
	foreach (QFileInfo fi, dir->entryInfoList()) {
		// Very, very first - check for thumbnail
		QString fileName(fi.absoluteFilePath()); 
		QCryptographicHash hash(QCryptographicHash::Md5);
		hash.addData(fileName.toLatin1());
		hash.addData(fi.lastModified().toString().toLatin1());
		QString thumbName(AmeUtils::homeDir(QString()) + "/.thumbnails/" + hash.result().toHex()+"." + fi.suffix());
		
		bool makeThumb = true;
		QFileInfo thumbFile(thumbName);
		if (thumbFile.exists()) {
			if (thumbFile.lastModified() >= fi.lastModified()) {
				fileName = thumbName;
				makeThumb = false;
			} else
				makeThumb = true;
		}
		// First try to guess from file extention
		f = false;		
		if (ext.contains(fi.suffix())) {
			//qDebug() << "by extension";
			f = true;
		} else { // Try to find mime type
			QString mimeType(mime.fromFile(fi.absoluteFilePath()));
			//qDebug() << "by mime";
			if (mimeType.startsWith("image/")) f = true;
		}
		if (f) {
			QListWidgetItem *it = new QListWidgetItem();
			//QPixmap pix();
			if (makeThumb) {
				QPixmap pix = QPixmap(fileName).scaled(80, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation);
				pix.save(thumbName, 0, 80);
				qDebug() << "making thumbnail : " << thumbName;
				it->setIcon(QIcon(thumbName));
			} else {
				it->setIcon(QIcon(fileName));
			}
			it->setData(Qt::UserRole, fi.absoluteFilePath());
			ui.content->addItem(it);
		}
	}
}


void Wallpaper::selectDir()
{
	QString directory = QFileDialog::getExistingDirectory(this,
			tr("Select Wallpaper directory"), AmeUtils::homeDir(QString()));
	if (!directory.isEmpty())
		addDir(directory);
}

void Wallpaper::addDir(const QString &directory)
{
	if (directory.isEmpty())
		return;
	QDir dir(directory);
	QListWidgetItem *it = new QListWidgetItem(QIcon(":/icons/tool/folder.png"),
			dir.dirName());
	it->setData(Qt::UserRole, QVariant(directory));
	ui.folders->addItem(it);
	ui.folders->setCurrentItem(it);
	readDirectoryContent(&dir);
	saveSettings();
}

void Wallpaper::removeDir()
{
	int row = ui.folders->currentRow();
	QListWidgetItem *it = ui.folders->takeItem(row);
	if (row >= 1) {
		ui.folders->setCurrentRow(row-1);
		onSelectFolder(ui.folders->item(row-1));
	} else {
		ui.content->clear();
	}
	delete it;
	saveSettings();
}

void Wallpaper::onSelectWallpaper(QListWidgetItem *it)
{
	QVariant name = it->data(Qt::UserRole);
	setCurrentWallpaper(name.toString(), true);
	antico->beginGroup("Desktop");
	antico->setValue("wallpaper", name);
	antico->endGroup();
	antico->sync();
}

void Wallpaper::onSelectFolder(QListWidgetItem *it) {
	QDir dir(it->data(Qt::UserRole).toString());
	if (dir.exists()) readDirectoryContent(&dir);	
}

void Wallpaper::setCurrentWallpaper(const QString &path, bool notify)
{
	currentPath = path;
	QPixmap pix = QPixmap(currentPath).scaled(ui.current->width(), ui.current->height(),
		Qt::KeepAspectRatio, Qt::SmoothTransformation);
	if (!pix.isNull()) {
		ui.current->setPixmap(pix);
		QFileInfo fi(currentPath);
		ui.currentTitle->setText(fi.fileName());
		if (notify) {
			// Notify AnticoDeluxe WM for changing wallpaper
			QDBusInterface *iface = new QDBusInterface("org.freedesktop.AnticoDeluxe", 
				"/", "org.freedesktop.AnticoDeluxe.WMCtrl",
				QDBusConnection::sessionBus(), this);
			if (!iface->isValid())
				qDebug() << "NOT VALID INTERFACE" << qPrintable(QDBusConnection::sessionBus().lastError().message());
			else
				iface->call("changeWallpaper", path);
		}
	} else {
	}
}

void Wallpaper::readSettings()
{
	state = Initializing;
	QStringList dirs = stg->value("wallpapers_dirs", "").toStringList(); 
	int count = dirs.count();
	for (int i = 0; i < count; i++) {
		addDir(dirs.at(i));
	} 
	if (count > 0) {
		ui.folders->setCurrentRow(0);
		QDir dir(dirs.at(0));
		state = Normal;
		readDirectoryContent(&dir);
		state = Initializing;
	}	
	// read AnticoDeluxe settings
	antico->beginGroup("Desktop");
	setCurrentWallpaper(antico->value("wallpaper", "").toString());
	antico->endGroup();
	state = Normal;
}


bool Wallpaper::saveSettings()
{
	QStringList dirs;
	for (int row = 0; row < ui.folders->count(); row++) {
		dirs << ui.folders->item(row)->data(Qt::UserRole).toString();
	}
	stg->setValue("wallpapers_dirs", dirs);
	stg->sync();
	return true;
}

Wallpaper::~Wallpaper()
{
}

