/*
 * filedataicon.hpp
 *
 *  Created on: Sep 16, 2013
 *      Author: leoz
 */

#ifndef FILEDATAICON_HPP_
#define FILEDATAICON_HPP_

#include <bb/cascades/Image>

enum FileDataType {
	FileDataTypeUnknown,
	FileDataTypeDir,
	FileDataTypeDirNoAccess,
	FileDataTypeImage
};

struct FileDataIcon {

	static void loadIcons();
	static void freeIcons();

	static QMap<FileDataType, bb::cascades::Image> m_icons;
};


#endif /* FILEDATAICON_HPP_ */
