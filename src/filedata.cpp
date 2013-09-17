/*
 * filedata.cpp
 *
 *  Created on: Sep 16, 2013
 *      Author: leoz
 */

#include "filedata.hpp"

///////////////////////////////////////////////////////////////////////////////
// Constructor/Destructor and static data

FileData::FileData(const QFileInfo& info)
: m_type(FileDataTypeUnknown)
, m_info(info)
{
	FileData::setSizes();
}

FileData::~FileData()
{}

const QString FileData::m_date_format("dd.MM.yyyy hh:mm");

QStringList FileData::m_size_list;

///////////////////////////////////////////////////////////////////////////////
// Access methods

QVariant FileData::icon() const
{
    return QVariant::fromValue(FileDataIcon::m_icons[m_type]);
}

QString FileData::size() const
{
     float num = m_info.size();

     QStringListIterator i(m_size_list);
     QString unit("bytes");

     while(num >= 1024.0 && i.hasNext()) {
         unit = i.next();
         num /= 1024.0;
     }

     return QString().setNum(num,'f', 0) + " " + unit;
}

QString FileData::name() const
{
	return m_info.fileName();
}

QString FileData::date() const
{
	return m_info.created().toString(m_date_format);
}

QString FileData::path() const
{
    return m_info.filePath();
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

