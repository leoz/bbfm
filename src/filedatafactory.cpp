/*
 * filedatafactory.cpp
 *
 *  Created on: Sep 16, 2013
 *      Author: leoz
 */

#include "filedatafactory.hpp"
#include "imagedata.hpp"
#include "dirdata.hpp"

FileData* FileDataFactory::create(const QUrl& url)
{
	QFileInfo info(url.path());

	if (info.isDir()) {
		return new DirData(url);
	}
	else {
		if (isImage(url)) {
			return new ImageData(url);
		}

		return new FileData(url);
	}
}

bool FileDataFactory::isImage(const QUrl& url)
{
	QFileInfo info(url.path());

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

bool FileDataFactory::isSupportedImage(const QUrl& url)
{
	QFileInfo info(url.path());

    if (info.isDir()) {
    	return false;
    }

    setImageExts();

    QStringListIterator i(m_image_sup_list);

    QString ext(info.suffix());
    ext = ext.toLower();

    while(i.hasNext()) {
        if (ext == i.next()) {
        	qWarning() << info.fileName() << " - is supported image!!!";
        	return true;
        }
    }

	qWarning() << info.fileName() << " - is not supported image!!!";
    return false;
}

QStringList FileDataFactory::m_image_ext_list;
QStringList FileDataFactory::m_image_sys_list;
QStringList FileDataFactory::m_image_sup_list;
QStringList FileDataFactory::m_image_uns_list;

void FileDataFactory::setImageExts()
{
	if (m_image_ext_list.empty()) {

	    // System supported image formats
		m_image_sys_list << "png" << "jpeg" << "jpg" << "gif" << "bmp";

	    // Image formats supported by ImageMagick
		m_image_sup_list << "tiff" << "tif";
		m_image_sup_list << "miff" << "mif";
		m_image_sup_list << "svg" << "ico" << "xpm";

	    // Unsupported image formats
		m_image_uns_list << "raw" << "cr2";

		m_image_ext_list.append(m_image_sys_list);
		m_image_ext_list.append(m_image_sup_list);
		m_image_ext_list.append(m_image_uns_list);
	}
}
