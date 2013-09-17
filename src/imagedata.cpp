/*
 * imagedata.cpp
 *
 *  Created on: Sep 16, 2013
 *      Author: leoz
 */

#include "imagedata.hpp"
#include "imageprocessor.hpp"

///////////////////////////////////////////////////////////////////////////////
// Constructor/Destructor

ImageData::ImageData(const QFileInfo& info)
: FileData(info)
, m_image()
, m_loading(false)
, m_error()
{
	m_type = FileDataTypeImage;
}

ImageData::~ImageData()
{}

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

void ImageData::loadData()
{
	m_loading = true;
	emit loadingChanged();

	QNetworkAccessManager* netManager = new QNetworkAccessManager(this);

	QString path = "file://" + m_info.filePath();

	const QUrl url(path);

	QNetworkRequest request(url);

	QNetworkReply* reply = netManager->get(request);
	bool ok = connect(reply, SIGNAL(finished()), this, SLOT(onReplyFinished()));
	Q_ASSERT(ok);
	Q_UNUSED(ok);
}

void ImageData::onReplyFinished()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    QString response;
    if (reply) {
        if (reply->error() == QNetworkReply::NoError) {
            const int available = reply->bytesAvailable();
            if (available > 0) {
                const QByteArray data(reply->readAll());

                // Setup the image processing thread
                ImageProcessor *imageProcessor = new ImageProcessor(data);

                QFuture<QImage> future = QtConcurrent::run(imageProcessor, &ImageProcessor::start);

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
}

void ImageData::onImageProcessingFinished()
{
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
}

///////////////////////////////////////////////////////////////////////////////
