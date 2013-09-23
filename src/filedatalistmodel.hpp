/*
 * filedatalistmodel.hpp
 *
 *  Created on: Sep 17, 2013
 *      Author: leoz
 */

#ifndef FILEDATALISTMODEL_HPP_
#define FILEDATALISTMODEL_HPP_

#include <QFileInfo>
#include <bb/cascades/QListDataModel>

class FileDataListModel : public bb::cascades::QListDataModel<QObject*>
{
    Q_OBJECT

public:
    FileDataListModel(bool load = true, QObject *parent = 0);

    Q_INVOKABLE void setDir(const QString& path);
    void addFile(const QUrl& url);
    void loadData();

private:
    bool m_load;
};

#endif /* FILEDATALISTMODEL_HPP_ */
