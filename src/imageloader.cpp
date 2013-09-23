/*
 * imageloader.cpp
 *
 *  Created on: Sep 21, 2013
 *      Author: leoz
 */

#include <QFile>
#include <QDebug>
#include <QColor>

#include "imageloader.hpp"

#include <string>
#include <Magick++.h>

using namespace Magick;
using namespace std;

ImageLoader::ImageLoader(const QByteArray &imageData, const ImageSize& size)
: m_data(imageData)
, m_path()
, m_size(size)
{}

ImageLoader::ImageLoader(const QString &path, const ImageSize& size)
: m_data()
, m_path(path)
, m_size(size)
{}

QImage ImageLoader::start()
{
    QImage image;

    bool result = image.loadFromData(m_data);

    int size = 82;
    int size_w = size;
    int size_h = size;

    if(result) {
    	// Scale image just when it is needed
       	if(m_size != ImageSizeFull) {
       		image = image.scaled(size_w, size_h, Qt::KeepAspectRatioByExpanding);
       	}
    }
    else {
    	// Try ImageMagick
    	try {
        	Blob blob(m_data.constData(), m_data.size());
        	Image img(blob);

        	// Scale image just when it is needed
        	if(m_size != ImageSizeFull) {
				Geometry geom(size_w,size_h);
				img.scale(geom);
        	}

        	image = convert(img);
    	}
    	catch( exception &error_ ) {
    		qWarning() << "### ImageMagick failed";
    		qWarning() << "Caught exception: " << error_.what();
    	}
    }

    return image;
}

QImage ImageLoader::start2()
{
    QFile file(m_path);

    file.open(QIODevice::ReadOnly);

    m_data = file.readAll();

    file.close();

    return ImageLoader::start();
}

QImage ImageLoader::convert(const Magick::Image &image)
{
    qDebug() << "convert:" << image.columns() << image.rows();

    QImage img(image.columns(), image.rows(), QImage::Format_RGB32);

	try {
		const Magick::PixelPacket *pixels;
		Magick::ColorRGB rgb;
		for (int y = 0; y < img.height(); y++) {
			pixels = image.getConstPixels(0, y, img.width(), 1);
			for (int x = 0; x < img.width(); x++) {
				rgb = (*(pixels + x));
				img.setPixel(x, y, QColor((int) (255 * rgb.red())
												 , (int) (255 * rgb.green())
												 , (int) (255 * rgb.blue())).rgb());
			}
		}
	}
	catch( exception &error_ ) {
		qWarning() << "convert";
		qWarning() << "Caught exception: " << error_.what();
		img = QImage();
	}

	return img;
}
