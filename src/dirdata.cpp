/*
 * dirdata.cpp
 *
 *  Created on: Sep 16, 2013
 *      Author: leoz
 */

#include "dirdata.hpp"

///////////////////////////////////////////////////////////////////////////////
// Constructor/Destructor

DirData::DirData(const QUrl& url)
: FileData(url)
{
	QFileInfo info(m_url.path());

	if (info.isReadable() && info.isExecutable()) {
		if(info.isSymLink()) {
			m_type = FileDataTypeDirSymLink;
		}
		else {
			m_type = FileDataTypeDir;
		}
	}
	else {
		m_type = FileDataTypeDirNoAccess;
	}
}

DirData::~DirData()
{}

///////////////////////////////////////////////////////////////////////////////
// Access methods

QString DirData::size() const
{
	QFileInfo info(m_url.path());

	if (info.fileName() == QString("..")) {
		return QString("<DIR>");
	}

	QDir dir(info.filePath());
	uint count = dir.count();
	QString text("items");

	if(count == 1) {
		text = QString("item");
	}

	return QString(QString::number(count) + " " + text);
}

///////////////////////////////////////////////////////////////////////////////
