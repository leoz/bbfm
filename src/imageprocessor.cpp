/* Copyright (c) 2012, 2013  BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "imageprocessor.hpp"

#include <QDebug>
#include <Qt/qcolor.h>

#include <string>
#include <Magick++.h>

using namespace Magick;
using namespace std;

ImageProcessor::ImageProcessor(const QByteArray &imageData, QObject *parent)
    : QObject(parent)
    , m_data(imageData)
{
}

QImage ImageProcessor::start()
{
    QImage image;

    bool result = image.loadFromData(m_data);

//    int img_w = 768;
//    int img_h = 500;

    int img_w = 128;
    int img_h = 128;

    if(result) {
        image = image.scaled(img_w, img_h, Qt::KeepAspectRatioByExpanding);
    }
    else {
    	// Try ImageMagick
    	try {
        	Blob blob(m_data.constData(), m_data.size());
        	Image img(blob);

        	image = convert(img);
    	}
    	catch( exception &error_ ) {
    		qWarning() << "### ImageMagick failed";
    		qWarning() << "Caught exception: " << error_.what();
    	}
    }

    // Image processing goes here, example could be adding water mark to the downloaded image

    return image;
}

QImage ImageProcessor::convert(const Magick::Image &image)
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
