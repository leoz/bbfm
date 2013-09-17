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
QStringList FileDataFactory::m_image_sys_list;
QStringList FileDataFactory::m_image_imk_list;
QStringList FileDataFactory::m_image_uns_list;

void FileDataFactory::setImageExts()
{
	if (m_image_ext_list.empty()) {

	    // System supported image formats
		m_image_sys_list << "png" << "jpeg" << "jpg" << "gif" << "bmp";

	    // Image formats supported by ImageMagick
		m_image_imk_list << "tiff" << "tif";
		m_image_imk_list << "miff" << "mif";
		m_image_imk_list << "svg" << "ico" << "xpm";

	    // Unsupported image formats
		m_image_uns_list << "raw" << "cr2";

		m_image_ext_list.append(m_image_sys_list);
		m_image_ext_list.append(m_image_imk_list);
		m_image_ext_list.append(m_image_uns_list);
	}
}
