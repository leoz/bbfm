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
#include "imagedata.hpp"
#include "filedatalistmodel.hpp"

#include <bb/cascades/AbstractPane>
#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bps/navigator_invoke.h>
#include <QDebug>

using namespace bb::cascades;

App::App(QObject *parent)
: QObject(parent)
, m_model(new FileDataListModel(this))
, m_dev_path("accounts/1000/shared")
, m_def_path(QDir::rootPath () + m_dev_path)
{
    // Register custom type to QML
    qmlRegisterType<ImageData>        ("com.leoz", 1, 0, "ImageData");
    qmlRegisterType<FileDataListModel>("com.leoz", 1, 0, "FileDataListModel");

    // Create the UI
    QmlDocument* qml = QmlDocument::create("asset:///main.qml").parent(this);
    qml->setContextProperty("_app", this);

    AbstractPane *root = qml->createRootObject<AbstractPane>();
    Application::instance()->setScene(root);

    // Load file icons
    FileDataIcon::loadIcons();
}

App::~App()
{
    // Free file icons
    FileDataIcon::freeIcons();
}

QFileInfo App::normalizeInfo(QFileInfo& info)
{
    if (info.fileName() == QString("..")) {
        qWarning() << "App::normalizeInfo : shall be normalized!";
        info = info.canonicalFilePath();
        qWarning() << "App::normalizeInfo : normalized path: " << info.filePath();
    }

    return info;
}

bool App::showFileList(const QString& path)
{
    qWarning() << "App::showFileList" << path;

    QFileInfo info(path);

    info = normalizeInfo(info);

    bool result(false);

    if (info.isDir()) {
    	if (info.isReadable() && info.isExecutable()) {
    	    result = true;
    	}
    }
    else {
    	if(!FileDataFactory::isSupportedImage(info)) {
        	invokeFile(info);
    	}
    }

    return result;
}

bool App::showImageView(const QString& path)
{
    QFileInfo info(path);

    info = normalizeInfo(info);

    return FileDataFactory::isSupportedImage(info);
}

void App::invokeFile(const QFileInfo& info)
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

