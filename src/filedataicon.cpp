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
	m_icons[FileDataTypeUnknown    ] = Image(QUrl("qrc:/assets/images/unknown.png"));
	m_icons[FileDataTypeDir        ] = Image(QUrl("qrc:/assets/images/folder.png"));
	m_icons[FileDataTypeDirNoAccess] = Image(QUrl("qrc:/assets/images/folder-system.png"));
	m_icons[FileDataTypeDirSymLink ] = Image(QUrl("qrc:/assets/images/folder-symlink.png"));
	m_icons[FileDataTypeImage      ] = Image(QUrl("qrc:/assets/images/image-x-generic.png"));
	m_icons[FileDataTypeImageRemote] = Image(QUrl("qrc:/assets/images/image-remote.png"));
}

void FileDataIcon::freeIcons()
{
	m_icons.clear();
}

