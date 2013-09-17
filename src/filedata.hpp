/*
 * filedata.hpp
 *
 *  Created on: Sep 16, 2013
 *      Author: leoz
 */

#ifndef FILEDATA_HPP_
#define FILEDATA_HPP_

#include <QVariant>
#include <QFileInfo>

#include "filedataicon.hpp"

class FileData : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVariant icon READ icon NOTIFY iconChanged)
    Q_PROPERTY(bool loading READ loading NOTIFY loadingChanged)
    Q_PROPERTY(QString error READ error NOTIFY errorChanged)

    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString date READ date NOTIFY dateChanged)
    Q_PROPERTY(QString size READ size NOTIFY sizeChanged)
    Q_PROPERTY(QString path READ path NOTIFY pathChanged)

public:
    FileData(const QFileInfo& info);
    ~FileData();

    virtual void loadData() {}

Q_SIGNALS:
    // The change notification signals of the properties
    void iconChanged();
    void loadingChanged();
    void errorChanged();
    void nameChanged();
    void dateChanged();
    void sizeChanged();
    void pathChanged();

protected:
    FileDataType m_type;
    QFileInfo m_info;

private:
    // The access methods for the properties
    virtual QVariant icon() const;
    virtual bool loading() const { return false; }
    virtual QString error() const {  return QString(); }
    virtual QString size() const;
    QString name() const;
    QString date() const;
    QString path() const;

    static void setSizes();

    static const QString m_date_format;
    static QStringList m_size_list;
};

#endif /* FILEDATA_HPP_ */
