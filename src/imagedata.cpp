/*
 * imagedata.cpp
 *
 *  Created on: Sep 16, 2013
 *      Author: leoz
 */

#include "imagedata.hpp"
#include "imageloader.hpp"

#include <bb/ImageData>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QDebug>

///////////////////////////////////////////////////////////////////////////////
// Constructor/Destructor

ImageData::ImageData(const QUrl& url)
: FileData(url)
, m_image()
, m_loading(false)
, m_error()
, m_size(ImageSizeIcon)
{
	if(m_url.isLocalFile()) {
		m_type = FileDataTypeImage;
	}
	else {
		m_type = FileDataTypeImageRemote;
	}
}

ImageData::~ImageData()
{}

void ImageData::reset()
{
	qWarning() << "ImageData::reset";

	FileData::reset();

	m_image = bb::cascades::Image();
	m_loading = false;
	m_error = QString();
	m_size = ImageSizeIcon;

	m_watcher.cancel();
}

void ImageData::setSize(int size)
{
	m_size = static_cast<ImageSize>(size);
}

///////////////////////////////////////////////////////////////////////////////
// Access methods

QVariant ImageData::icon() const
{
    if (m_image.isNull()) {
    	return QVariant::fromValue(FileDataIcon::m_icons[m_type]);
    }
    else {
       	return QVariant::fromValue(m_image);
    }
}

bool ImageData::loading() const
{
	return m_loading;
}

QString ImageData::error() const
{
	return m_error;
}

///////////////////////////////////////////////////////////////////////////////

void ImageData::load()
{
	if (m_url.isLocalFile()) {
		load_local();
	}
	else {
		load_remote();
	}
}

void ImageData::load_local()
{
    // Setup the image loader thread
	ImageLoader* loader = new ImageLoader(path(), m_size);

    QFuture<QImage> future = QtConcurrent::run(loader, &ImageLoader::start2);

    // Invoke our onProcessingFinished slot after the processing has finished.
    bool ok = connect(&m_watcher, SIGNAL(finished()), this, SLOT(onImageProcessingFinished()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);

    // starts watching the given future
    m_watcher.setFuture(future);
}

void ImageData::load_remote()
{
	qWarning() << "@@@ mageData::load_remote - BEGIN - " << name();

	m_loading = true;
	emit loadingChanged();

	QNetworkAccessManager* netManager = new QNetworkAccessManager(this);

	QNetworkRequest request(m_url);

	QNetworkReply* reply = netManager->get(request);

	qWarning() << "+++ mageData::load - reply is: " << reply;

	bool ok = connect(reply, SIGNAL(finished()), this, SLOT(onReplyFinished()));

	qWarning() << "+++ mageData::load - result is: " << ok;

	Q_ASSERT(ok);
	Q_UNUSED(ok);

	qWarning() << "@@@ mageData::load_remote - END - " << name();
}

void ImageData::onReplyFinished()
{
	qWarning() << "@@@ mageData::onReplyFinished - BEGIN - " << name();

	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    QString response;
    if (reply) {
        if (reply->error() == QNetworkReply::NoError) {
            const int available = reply->bytesAvailable();
            if (available > 0) {
                const QByteArray data(reply->readAll());

                // Setup the image loader thread
                ImageLoader* loader = new ImageLoader(data, m_size);

                QFuture<QImage> future = QtConcurrent::run(loader, &ImageLoader::start);

                // Invoke our onProcessingFinished slot after the processing has finished.
                bool ok = connect(&m_watcher, SIGNAL(finished()), this, SLOT(onImageProcessingFinished()));
                Q_ASSERT(ok);
                Q_UNUSED(ok);

                // starts watching the given future
                m_watcher.setFuture(future);

            }
        } else {
        	m_error = tr("Error: %1 status: %2").arg(reply->errorString(), reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString());
            emit errorChanged();

            m_loading = false;
            emit loadingChanged();
        }

        reply->deleteLater();
    } else {
    	m_error = tr("Download failed. Check internet connection");
        emit errorChanged();

        m_loading = false;
        emit loadingChanged();
    }

	qWarning() << "@@@ mageData::onReplyFinished - END - " << name();
}

void ImageData::onImageProcessingFinished()
{
	qWarning() << "@@@ mageData::onImageProcessingFinished - BEGIN - " << name();

    const QImage swappedImage = m_watcher.future().result().rgbSwapped();

    if(!swappedImage.isNull()) {
		const bb::ImageData imageData = bb::ImageData::fromPixels(swappedImage.bits(), bb::PixelFormat::RGBX, swappedImage.width(), swappedImage.height(), swappedImage.bytesPerLine());
	    m_image = bb::cascades::Image(imageData);
    }

    emit iconChanged();

    m_error.clear();
    emit errorChanged();

    m_loading = false;
    emit loadingChanged();

	qWarning() << "@@@ mageData::onImageProcessingFinished - END - " << name();
}

///////////////////////////////////////////////////////////////////////////////
