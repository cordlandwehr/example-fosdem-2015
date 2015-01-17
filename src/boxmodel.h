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

#ifndef BOXMODEL_H
#define BOXMODEL_H

#include <QAbstractListModel>

class Box;
class BoxManager;
class QSignalMapper;

class BoxModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(BoxManager *boxManager READ boxManager WRITE setBoxManager)

public:
    enum boxRoles {
        TextRole = Qt::UserRole + 1,
        DataRole
    };

    explicit BoxModel(QObject *parent = Q_NULLPTR);
    virtual QHash<int,QByteArray> roleNames() const Q_DECL_OVERRIDE;
    void setBoxManager(BoxManager *boxManager);
    BoxManager * boxManager() const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

Q_SIGNALS:
    void boxChanged(int index);

private Q_SLOTS:
    void boxAboutToBeAdded(Box *box, int index);
    void onBoxAdded();
    void onBoxAboutToBeRemoved(int index);
    void emitBoxChanged(int row);

private:
    void updateMappings();
    BoxManager *m_boxManager;
    QSignalMapper *m_signalMapper;
};

#endif
