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
    FileDataListModel(QObject *parent = 0);

    void setDir(const QFileInfo& info);

private:
    void addFile(const QFileInfo& info);
    void loadData();
};

#endif /* FILEDATALISTMODEL_HPP_ */
