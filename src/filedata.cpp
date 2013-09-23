/*
 * filedata.cpp
 *
 *  Created on: Sep 16, 2013
 *      Author: leoz
 */

#include "filedata.hpp"

///////////////////////////////////////////////////////////////////////////////
// Constructor/Destructor and static data

FileData::FileData(const QUrl& url)
: m_type(FileDataTypeUnknown)
, m_url(url)
{
	FileData::setSizes();
}

FileData::~FileData()
{}

const QString FileData::m_date_format("MMM dd, yyyy h:mm A");

QStringList FileData::m_size_list;

void FileData::reset()
{
	qWarning() << "FileData::reset";

	m_type = FileDataTypeUnknown;
	m_url = QUrl();
}

///////////////////////////////////////////////////////////////////////////////
// Access methods

QVariant FileData::icon() const
{
    return QVariant::fromValue(FileDataIcon::m_icons[m_type]);
}

QString FileData::size() const
{
     float num = QFileInfo(m_url.path()).size();

     QStringListIterator i(m_size_list);
     QString unit("bytes");

     while(num >= 1024.0 && i.hasNext()) {
         unit = i.next();
         num /= 1024.0;
     }

     return QString().setNum(num,'f', 0) + " " + unit;
}

void FileData::setPath(const QString& path)
{
	reset();
	m_url = QUrl(path);
}

QString FileData::name() const
{
	return QFileInfo(m_url.path()).fileName();
}

QString FileData::date() const
{
	return QFileInfo(m_url.path()).created().toString(m_date_format);
}

QString FileData::path() const
{
    return m_url.path();
}

///////////////////////////////////////////////////////////////////////////////
// Utility methods

void FileData::setSizes()
{
	if (m_size_list.empty()) {
		m_size_list << "KB" << "MB" << "GB" << "TB";
	}
}

///////////////////////////////////////////////////////////////////////////////

