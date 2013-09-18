/*
 * filedatalistmodel.cpp
 *
 *  Created on: Sep 17, 2013
 *      Author: leoz
 */

#include "filedatalistmodel.hpp"
#include "filedatafactory.hpp"
#include "filedata.hpp"

///////////////////////////////////////////////////////////////////////////////
// Constructor/Destructor

FileDataListModel::FileDataListModel(QObject *parent)
: QListDataModel<QObject*>()
{
	setParent(parent);
}

///////////////////////////////////////////////////////////////////////////////

void FileDataListModel::setDir(const QFileInfo& info)
{
	QDir dir(info.filePath());
	QFileInfoList files = dir.entryInfoList();

	clear();

	bool remove_root = info.isRoot();

	QString nm("");
	QString root_nm("..");
	QString dir_nm(".");

	QString cur_name("");

	foreach(const QFileInfo &fi, files) {
		nm = fi.fileName();
		// Do not include ".." for root
		if (!(remove_root && nm == root_nm)) {
			// Do not include "."
			if (!(nm == dir_nm)) {
				// Fix duplicate entries
				if (nm != cur_name) {
					cur_name = nm;
					addFile(fi);
				}
			}
		}
		else {
			remove_root = false;
		}
	}

	loadData();
}

void FileDataListModel::addFile(const QFileInfo& info)
{
	append(FileDataFactory::create(info));
}

void FileDataListModel::loadData()
{
    // Call the load() method for each FileData instance inside the model
    for (int row = 0; row < size(); ++row) {
        qobject_cast<FileData*>(value(row))->load();
    }
}

///////////////////////////////////////////////////////////////////////////////
