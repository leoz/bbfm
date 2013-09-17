/*
 * dirdata.cpp
 *
 *  Created on: Sep 16, 2013
 *      Author: leoz
 */

#include "dirdata.hpp"

///////////////////////////////////////////////////////////////////////////////
// Constructor/Destructor

DirData::DirData(const QFileInfo& info)
: FileData(info)
{
	if (m_info.isReadable() && m_info.isExecutable()) {
		m_type = FileDataTypeDir;
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
	if (m_info.fileName() == QString("..")) {
		return QString("<DIR>");
	}

	QDir dir(m_info.filePath());
	uint count = dir.count();
	QString text("items");

	if(count == 1) {
		text = QString("item");
	}

	return QString(QString::number(count) + " " + text);
}

///////////////////////////////////////////////////////////////////////////////
