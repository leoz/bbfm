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

#include "app.hpp"

#include <bb/cascades/AbstractPane>
#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <QDebug>

using namespace bb::cascades;

App::App(QObject *parent)
    : QObject(parent)
    , m_model(new QListDataModel<QObject*>())
{
    // Register custom type to QML
    qmlRegisterType<ImageLoader>();

    m_model->setParent(this);

    // Create the UI
    QmlDocument* qml = QmlDocument::create("asset:///main.qml").parent(this);
    qml->setContextProperty("_app", this);

    AbstractPane *root = qml->createRootObject<AbstractPane>();
    Application::instance()->setScene(root);

    // Fill the model with data
	QString path = "/accounts/1000/removable/sdcard/photos/file_types/jpeg";
    readDir(path);
}

bool App::readDir(const QString& path)
{
    qWarning() << "##### App::readDir : " << path;

    QFileInfo info(path);

    if (info.fileName() == QString("..")) {
        qWarning() << "##### App::readDir : shall be normalized!";
        info = info.canonicalFilePath();
        qWarning() << "##### App::readDir : normalized path: " << info.filePath();
    }

    if (info.isDir() && info.isReadable() && info.isExecutable()) {
		QDir dir(info.filePath());
		QFileInfoList files = dir.entryInfoList();

		m_model->clear();

		bool remove_root = info.isRoot();

		QString nm("");
		QString root_nm("..");
		QString dir_nm(".");


		foreach(const QFileInfo &fi, files) {
			nm = fi.fileName();
			// Do not include ".." for root
			if (!(remove_root && nm == root_nm)) {
				// Do not include "."
				if (!(nm == dir_nm)) {
					m_model->append(new ImageLoader(fi, this));
				}
			}
			else {
				remove_root = false;
			}
		}

		loadImages();

		return true;
    }

    return false;
}

QString App::getPath(const QString& path)
{
    QFileInfo info(path);

    if (info.fileName() == QString("..")) {
        info = info.canonicalFilePath();
    }

    return info.filePath();
}

QString App::getTitle(const QString& path)
{
    QFileInfo info(path);

    if (info.fileName() == QString("..")) {
        info = info.canonicalFilePath();
    }

    // Fix the root name
    if (info.isRoot()) {
    	return QString("/");
    }

    return info.fileName();
}

void App::loadImages()
{
    // Call the load() method for each ImageLoader instance inside the model
    for (int row = 0; row < m_model->size(); ++row) {
        qobject_cast<ImageLoader*>(m_model->value(row))->load();
    }
}

bb::cascades::DataModel* App::model() const
{
    return m_model;
}
