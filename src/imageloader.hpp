/*
 * imageloader.hpp
 *
 *  Created on: Sep 21, 2013
 *      Author: leoz
 */

#ifndef IMAGELOADER_HPP_
#define IMAGELOADER_HPP_

#include "imagesize.hpp"

#include <QByteArray>
#include <QObject>
#include <QImage>

namespace Magick {
	class Image;
}

class ImageLoader : public QObject
{
    Q_OBJECT

public:
    ImageLoader(const QByteArray &imageData, const ImageSize& size);
    ImageLoader(const QString &path, const ImageSize& size);

public Q_SLOTS:
    QImage start();
    QImage start2();

private:
    QImage convert(const Magick::Image &image);

    QByteArray m_data;
    QString m_path;
    ImageSize m_size;
};

#endif /* IMAGELOADER_HPP_ */
