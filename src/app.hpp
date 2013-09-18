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

#ifndef APP_HPP
#define APP_HPP

#include <QtCore/QObject>
#include <QtCore/QVector>

#include <bb/cascades/QListDataModel>

class FileDataListModel;

class App : public QObject
{
    Q_OBJECT

public:
    App(QObject *parent = 0);
    ~App();

    Q_INVOKABLE bool showFileList(const QString& path);
    Q_INVOKABLE bool showImageView(const QString& path);
    Q_INVOKABLE QString getDefPath();
    Q_INVOKABLE QString getDefTitle();
    Q_INVOKABLE QString getPath(const QString& path);
    Q_INVOKABLE QString getTitle(const QString& path);

private:
    void invokeFile(const QFileInfo& info);
    QFileInfo normalizeInfo(QFileInfo& info);

    // The model that contains the progress and image data
    FileDataListModel* m_model;

    // Default path
    QString m_dev_path;
    QString m_def_path;
};

#endif
