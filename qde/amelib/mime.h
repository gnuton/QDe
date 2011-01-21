/* 
 *  This file is taken from Quartica project (FreeDesktopMime).
 *
 *  Copyright (c) 2008 Matteo Bertozzi <theo.bertozzi@gmail.com>
 * 
 *	Some modifications made by me. <ludmiloff@gmail.com>
 *	- changed class name to fit Amelib namespace;
 *
 */

#ifndef _AMEMIME_H_
#define _AMEMIME_H_

#include <QDomElement>
#include <QFile>

class AmeMime : public QObject {
	Q_OBJECT

	public:
		AmeMime (QObject *parent = 0);
		~AmeMime();

		QString fromFileName (const QString& fileName);

		QString fromFile (const QString& fileName);
		QString fromFile (QFile *file);

		// Methods Information
		QString genericIconName (const QString& mimeType);
		QString expandedAcronym (const QString& mimeType);		
		QString description (const QString& mimeType);
		QString subClassOf (const QString& mimeType);
		QString acronym (const QString& mimeType);
		QString alias (const QString& mimeType);

		QString genericIconName (void) const;
		QString expandedAcronym (void) const;
		QString description (void) const;
		QString subClassOf (void) const;
		QString mimeType (void) const;
		QString acronym (void) const;
		QString alias (void) const;		

	protected:
		bool checkMatch (const QDomElement& matchNode, QFile *file);
		bool checkMagic (const QDomElement& magicNode, QFile *file);
		bool checkMagic (const QDomNodeList& matchList, QFile *file);
		bool checkSubMagic (const QDomNodeList& matchList, QFile *file);

		bool getMimeNode (const QString& mimeType);

	private:
		bool checkMagicString (const QString offset, const QByteArray value, const QByteArray mask, QFile *file);

	private:
		class Private;
		Private *d;
};

#endif // !_AMEMIME_H_

