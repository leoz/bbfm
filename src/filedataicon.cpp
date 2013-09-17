/*
 * filedataicon.cpp
 *
 *  Created on: Sep 16, 2013
 *      Author: leoz
 */

#include "filedataicon.hpp"

using namespace bb::cascades;

QMap<FileDataType, bb::cascades::Image> FileDataIcon::m_icons;

void FileDataIcon::loadIcons()
{
	m_icons[FileDataTypeUnknown    ] = Image(QUrl("asset:///images/unknown.png"));
	m_icons[FileDataTypeDir        ] = Image(QUrl("asset:///images/folder.png"));
	m_icons[FileDataTypeDirNoAccess] = Image(QUrl("asset:///images/folder-red.png"));
	m_icons[FileDataTypeImage      ] = Image(QUrl("asset:///images/image-x-generic.png"));
}

void FileDataIcon::freeIcons()
{
	m_icons.clear();
}

