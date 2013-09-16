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

#include "imageloader.hpp"

#include "imageprocessor.hpp"

#include <bb/ImageData>
#include <bb/cascades/Image>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QUrl>
#include <QDebug>

using namespace bb::cascades;

/**
 *  This class implements a image loader which will initialize a network request in asynchronous manner.
 *  After receiving response from the network, it creates a QImage object in its own thread.
 *  Then it signals the interested parties about the result.
 */

/**
 *  Constructor
 */
ImageLoader::ImageLoader(const QFileInfo& fileInfo, QObject* parent)
: QObject(parent)
, m_loading(false)
, m_fileInfo(fileInfo)
{}

/**
 * Destructor
 */
//! [1]
ImageLoader::~ImageLoader() { }
//! [1]

/**
 * ImageLoader::load()
 *
 * Instruct the QNetworkAccessManager to initialize a network request in asynchronous manner.
 *
 * Also, setup the signal handler to receive the event indicating the network response.
 */
//! [2]
void ImageLoader::load()
{
	setDefaultImage();

	if(m_fileInfo.isFile()) {

		m_loading = true;
		emit loadingChanged();

		QNetworkAccessManager* netManager = new QNetworkAccessManager(this);

		const QUrl url(m_fileInfo.filePath());

		QNetworkRequest request(url);

		QNetworkReply* reply = netManager->get(request);
		bool ok = connect(reply, SIGNAL(finished()), this, SLOT(onReplyFinished()));
		Q_ASSERT(ok);
		Q_UNUSED(ok);
	}
}
//! [2]

/**
 * ImageLoader::onReplyFinished()
 *
 * Handler for the signal indicating the response for the previous network request.
 *
 * If the result was a success, it will start the thread of constructing the QImage object.
 */
//! [3]
void ImageLoader::onReplyFinished()
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
            m_label = tr("Error: %1 status: %2").arg(reply->errorString(), reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString());
            emit labelChanged();

            m_loading = false;
            emit loadingChanged();
        }

        reply->deleteLater();
    } else {
        m_label = tr("Download failed. Check internet connection");
        emit labelChanged();

        m_loading = false;
        emit loadingChanged();
    }
}
//! [3]

/**
 * ImageLoader::onImageProcessingFinished()
 *
 * Handler for the signal indicating the result of the image processing.
 */
//! [4]
void ImageLoader::onImageProcessingFinished()
{
    const QImage swappedImage = m_watcher.future().result().rgbSwapped();

    if(swappedImage.isNull()) {
    	setDefaultImage();
    }
    else {
		const bb::ImageData imageData = bb::ImageData::fromPixels(swappedImage.bits(), bb::PixelFormat::RGBX, swappedImage.width(), swappedImage.height(), swappedImage.bytesPerLine());
	    m_image = bb::cascades::Image(imageData);
    }

    emit imageChanged();

    m_label.clear();
    emit labelChanged();

    m_loading = false;
    emit loadingChanged();
}
//! [4]

QVariant ImageLoader::image() const
{
    return QVariant::fromValue(m_image);
}

QString ImageLoader::label() const
{
    return m_label;
}

bool ImageLoader::loading() const
{
    return m_loading;
}

QString ImageLoader::name() const
{
	return m_fileInfo.fileName();
}

QString ImageLoader::date() const
{
	return m_fileInfo.created().toString("dd.MM.yyyy hh:mm");
}

QString ImageLoader::size() const
{
	if (m_fileInfo.isDir()) {
		return getDirSize();
	}
	return getFileSize();
}

QString ImageLoader::path() const
{
    return m_fileInfo.filePath();
}

QString ImageLoader::getDirSize() const
{
	if (m_fileInfo.fileName() == QString("..")) {
		return QString("<DIR>");

	}
	QDir dir(m_fileInfo.filePath());
	uint count = dir.count();
	QString text("items");
	if(count == 1) {
		text = QString("item");
	}
	return QString(QString::number(count) + " " + text);
}

QString ImageLoader::getFileSize() const
{
     float num = m_fileInfo.size();
     QStringList list;
     list << "KB" << "MB" << "GB" << "TB";

     QStringListIterator i(list);
     QString unit("bytes");

     while(num >= 1024.0 && i.hasNext()) {
         unit = i.next();
         num /= 1024.0;
     }

     return QString().setNum(num,'f',0)+" "+unit;
}

void ImageLoader::setDefaultImage()
{
	if (m_fileInfo.isDir()) {
		if (m_fileInfo.isReadable() && m_fileInfo.isExecutable()) {
			m_image = Image(QUrl("asset:///images/folder.png"));
		}
		else {
			m_image = Image(QUrl("asset:///images/folder-red.png"));
		}
	}
	else {
		m_image = Image(QUrl("asset:///images/unknown.png"));
	}
}
