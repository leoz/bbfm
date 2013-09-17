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
#include "filedata.hpp"
#include "filedataicon.hpp"
#include "filedatafactory.hpp"

#include <bb/cascades/AbstractPane>
#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bps/navigator_invoke.h>
#include <QDebug>

using namespace bb::cascades;

App::App(QObject *parent)
: QObject(parent)
, m_model(new QListDataModel<QObject*>())
, m_dev_path("accounts/1000/shared")
, m_def_path(QDir::rootPath () + m_dev_path)
{
    m_model->setParent(this);

    // Create the UI
    QmlDocument* qml = QmlDocument::create("asset:///main.qml").parent(this);
    qml->setContextProperty("_app", this);

    AbstractPane *root = qml->createRootObject<AbstractPane>();
    Application::instance()->setScene(root);

    // Load file icons
    FileDataIcon::loadIcons();

    // Fill the model with data
    action(m_def_path);
}

App::~App()
{
    // Free file icons
    FileDataIcon::freeIcons();
}

bool App::action(const QString& path)
{
    qWarning() << "App::action" << path;

    QFileInfo info(path);

    if (info.fileName() == QString("..")) {
        qWarning() << "App::action : shall be normalized!";
        info = info.canonicalFilePath();
        qWarning() << "App::action : normalized path: " << info.filePath();
    }

    bool result(false);

    if (info.isDir()) {
    	if (info.isReadable() && info.isExecutable()) {
    	    result = true;
    	    dirAction(info);
    	}
    }
    else {
	    fileAction(info);
    }

    return result;
}

void App::dirAction(const QFileInfo& info)
{
	QDir dir(info.filePath());
	QFileInfoList files = dir.entryInfoList();

	m_model->clear();

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
					m_model->append(FileDataFactory::create(fi));
				}
			}
		}
		else {
			remove_root = false;
		}
	}

	loadImages();
}

void App::fileAction(const QFileInfo& info)
{
	navigator_invoke_invocation_t *invoke = NULL;
	navigator_invoke_invocation_create(&invoke);

	QUrl u(QUrl::fromLocalFile(info.filePath()));
	QString url(u.toString());
	QByteArray bytes = url.toUtf8();
	navigator_invoke_invocation_set_uri(invoke, bytes.constData());

	navigator_invoke_invocation_send(invoke);
	navigator_invoke_invocation_destroy(invoke);
}

QString App::getDefPath()
{
	return getPath(m_def_path);
}

QString App::getDefTitle()
{
	return getTitle(m_def_path);
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
    // Call the load() method for each FileData instance inside the model
    for (int row = 0; row < m_model->size(); ++row) {
        qobject_cast<FileData*>(m_model->value(row))->loadData();
    }
}

bb::cascades::DataModel* App::model() const
{
    return m_model;
}
