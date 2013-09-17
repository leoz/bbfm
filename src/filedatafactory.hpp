/*
 * filedatafactory.hpp
 *
 *  Created on: Sep 16, 2013
 *      Author: leoz
 */

#ifndef FILEDATAFACTORY_HPP_
#define FILEDATAFACTORY_HPP_

#include <QFileInfo>

class FileData;

struct FileDataFactory {
    static FileData* create(const QFileInfo& info);
    static bool isImage(const QFileInfo& info);

private:
    static void setImageExts();

    static QStringList m_image_ext_list;
};


#endif /* FILEDATAFACTORY_HPP_ */
