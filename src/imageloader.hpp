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

#ifndef IMAGELOADER_HPP
#define IMAGELOADER_HPP

#include <QtGui/QImage>
#include <QByteArray>

#include <bb/cascades/Image>

class ImageLoader : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVariant image READ image NOTIFY imageChanged)
    Q_PROPERTY(QString label READ label NOTIFY labelChanged)

    Q_PROPERTY(bool loading READ loading NOTIFY loadingChanged)

    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString date READ date NOTIFY dateChanged)
    Q_PROPERTY(QString size READ size NOTIFY sizeChanged)
    Q_PROPERTY(QString path READ path NOTIFY pathChanged)

public:
    /*
     * Creates a new image loader.
     *
     * @param imageUrl The url to load the image from.
     */
    ImageLoader(const QFileInfo& fileInfo, QObject* parent = 0);

    /*
     * Destroys the image loader.
     */
    ~ImageLoader();

    /*
     * Loads the image.
     */
    void load();

Q_SIGNALS:
    // The change notification signals of the properties
    void imageChanged();
    void labelChanged();
    void loadingChanged();
    void nameChanged();
    void dateChanged();
    void sizeChanged();
    void pathChanged();

private Q_SLOTS:
    /*
     * Response handler for the network operation.
     */
    void onReplyFinished();

    /*
     * Response handler for the image process operation.
     */
    void onImageProcessingFinished();

private:
    // The accessor methods of the properties
    QVariant image() const;
    QString label() const;
    bool loading() const;

    // New access methods for the properties
    QString name() const;
    QString date() const;
    QString size() const;
    QString path() const;

    QString getDirSize() const;
    QString getFileSize() const;

    void setDefaultImage();

    // The property values
    bb::cascades::Image m_image;
    QString m_label;
    bool m_loading;

    // File associated with URL
    QFileInfo m_fileInfo;

    // The thread status watcher
    QFutureWatcher<QImage> m_watcher;
};

#endif
