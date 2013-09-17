/*
 * dirdata.hpp
 *
 *  Created on: Sep 16, 2013
 *      Author: leoz
 */

#ifndef DIRDATA_HPP_
#define DIRDATA_HPP_

#include "filedata.hpp"

class DirData : public FileData
{
    Q_OBJECT

public:
    DirData(const QFileInfo& info);
    ~DirData();

private:
    virtual QString size() const;
};

#endif /* DIRDATA_HPP_ */
