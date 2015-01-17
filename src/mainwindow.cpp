/*
 *  Copyright 2015  Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 *  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 *  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 *  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 *  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 *  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "mainwindow.h"
#include "boxmanager.h"

#include <KMessageBox>
#include <QDebug>
#include <QIcon>
#include <QQmlContext>
#include <QQuickItem>
#include <QQuickWidget>
#include <QStandardPaths>

MainWindow::MainWindow()
    : m_widget(new QQuickWidget)
    , m_boxManager(new BoxManager)
{
    setWindowIcon(QIcon::fromTheme("system"));
    setWindowTitle(qAppName());

    // register box manager globally in QML Context
    m_widget->rootContext()->setContextProperty("boxManager", m_boxManager);

    m_widget->resize(QSize(800, 600));
    m_widget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    setCentralWidget(m_widget);
}

MainWindow::~MainWindow()
{
    m_boxManager->deleteLater();
}

bool MainWindow::queryClose()
{
    int result = KMessageBox::warningYesNoCancel(0, "Do you really want to close?");
    switch(result) {
    case KMessageBox::Yes: return true;
    case KMessageBox::No: return true;
    default: return false;
    }
}
