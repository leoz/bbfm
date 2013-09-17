/*
 * filedatafactory.cpp
 *
 *  Created on: Sep 16, 2013
 *      Author: leoz
 */

#include "filedatafactory.hpp"
#include "imagedata.hpp"
#include "dirdata.hpp"

FileData* FileDataFactory::create(const QFileInfo& info)
{
	if (info.isDir()) {
		return new DirData(info);
	}
	else {
		if (isImage(info)) {
			return new ImageData(info);
		}

		return new FileData(info);
	}
}

bool FileDataFactory::isImage(const QFileInfo& info)
{
    if (info.isDir()) {
    	return false;
    }

    setImageExts();

    QStringListIterator i(m_image_ext_list);

    QString ext(info.suffix());
    ext = ext.toLower();

    while(i.hasNext()) {
        if (ext == i.next()) {
        	qWarning() << info.fileName() << " - IS IMAGE!!!";
        	return true;
        }
    }

	qWarning() << info.fileName() << " - IS NOT IMAGE!!!";
    return false;
}

QStringList FileDataFactory::m_image_ext_list;

void FileDataFactory::setImageExts()
{
	if (m_image_ext_list.empty()) {

	    // Supported natively
		m_image_ext_list << "png" << "jpeg" << "jpg" << "gif" << "bmp";

		// Supported extra formats
		m_image_ext_list << "svg" << "ico";

	    // Supported by ImageMagick
		m_image_ext_list << "xpm" << "tiff" << "tif" << "raw" << "miff" << "mif" << "cr2";
	}
}
