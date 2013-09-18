/*
 * imagedata.hpp
 *
 *  Created on: Sep 16, 2013
 *      Author: leoz
 */

#ifndef IMAGEDATA_HPP_
#define IMAGEDATA_HPP_

#include "filedata.hpp"
#include "imagesize.hpp"

class ImageData : public FileData
{
    Q_OBJECT

public:
    ImageData(const QFileInfo& info = QFileInfo());
    ~ImageData();

    Q_INVOKABLE virtual void load();
    Q_INVOKABLE virtual void reset();
    Q_INVOKABLE virtual void setSize(int size);

private Q_SLOTS:
	void onReplyFinished();
    void onImageProcessingFinished();

private:
    virtual QVariant icon() const;
    virtual bool loading() const;
    virtual QString error() const;

    QFutureWatcher<QImage> m_watcher;

    bb::cascades::Image m_image;
    bool m_loading;
    QString m_error;
    ImageSize m_size;
};


#endif /* IMAGEDATA_HPP_ */
