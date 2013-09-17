/*
 * imagedata.hpp
 *
 *  Created on: Sep 16, 2013
 *      Author: leoz
 */

#ifndef IMAGEDATA_HPP_
#define IMAGEDATA_HPP_

#include "filedata.hpp"

class ImageData : public FileData
{
    Q_OBJECT

public:
    ImageData(const QFileInfo& info);
    ~ImageData();

    virtual void loadData();

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
};


#endif /* IMAGEDATA_HPP_ */
