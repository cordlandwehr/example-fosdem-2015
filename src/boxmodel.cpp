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

#include "boxmodel.h"
#include "box.h"
#include "boxmanager.h"

#include <QSignalMapper>


BoxModel::BoxModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_boxManager(Q_NULLPTR)
    , m_signalMapper(new QSignalMapper(this))
{
    connect(m_signalMapper, SIGNAL(mapped(int)), SLOT(emitBoxChanged(int)));
}

QHash< int, QByteArray > BoxModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[DataRole] = "dataRole";

    return roles;
}

void BoxModel::setBoxManager(BoxManager *boxManager)
{
    if (m_boxManager == boxManager) {
        return;
    }

    beginResetModel();

    if (m_boxManager) {
        m_boxManager->disconnect(this);
        for (auto box : m_boxManager->boxes()) {
            box->disconnect(this);
        }
    }

    m_boxManager = boxManager;
    if (m_boxManager) {
        // initial setting of signal mappings
        connect(m_boxManager, &BoxManager::boxAboutToBeAdded, this, &BoxModel::boxAboutToBeAdded);
        connect(m_boxManager, &BoxManager::boxAdded, this, &BoxModel::onBoxAdded);
        connect(m_boxManager, &BoxManager::boxAboutToBeRemoved, this, &BoxModel::onBoxAboutToBeRemoved);

        // insert and connect all already existing profiles
        int boxes = m_boxManager->boxes().count();
        for (int i = 0; i < boxes; ++i) {
            connect(m_boxManager->boxes().at(i), &Box::textChanged, m_signalMapper, static_cast<void (QSignalMapper::*)(void)>(&QSignalMapper::map));
            connect(m_boxManager->boxes().at(i), &Box::positionChanged, m_signalMapper, static_cast<void (QSignalMapper::*)(void)>(&QSignalMapper::map));
        }
        updateMappings();
    }
    endResetModel();
}

BoxManager * BoxModel::boxManager() const
{
    return m_boxManager;
}

QVariant BoxModel::data(const QModelIndex &index, int role) const
{
    Q_ASSERT(m_boxManager);

    if (!index.isValid()) {
        return QVariant();
    }

    if (index.row() >= m_boxManager->boxes().count()) {
        return QVariant();
    }

    Box * const box = m_boxManager->boxes().at(index.row());

    switch(role)
    {
    case Qt::DisplayRole:
        return !box->text().isEmpty()?
                QVariant(box->text()) : QVariant("unknown");
    case Qt::ToolTipRole:
        return QVariant(box->text());
    case DataRole:
        return QVariant::fromValue<QObject*>(box);
    default:
        return QVariant();
    }
}

int BoxModel::rowCount(const QModelIndex &parent) const
{
    if (!m_boxManager) {
        return 0;
    }

    if (parent.isValid()) {
        return 0;
    }
    return m_boxManager->boxes().count();
}

void BoxModel::boxAboutToBeAdded(Box *box, int index)
{
    connect(box, &Box::textChanged, m_signalMapper, static_cast<void (QSignalMapper::*)(void)>(&QSignalMapper::map));
    beginInsertRows(QModelIndex(), index, index);
    updateMappings();
}

void BoxModel::onBoxAdded()
{
    endInsertRows();
}

void BoxModel::onBoxAboutToBeRemoved(int index)
{
    beginRemoveRows(QModelIndex(), index, index);
    endRemoveRows();
}

void BoxModel::emitBoxChanged(int row)
{
    emit dataChanged(index(row, 0), index(row, 0));
}

QVariant BoxModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole) {
        return QVariant();
    }
    if (orientation == Qt::Vertical) {
        return QVariant(section + 1);
    }
    return QVariant("Box");
}

void BoxModel::updateMappings()
{
    if (!m_boxManager) {
        return;
    }
    int boxes = m_boxManager->boxes().count();
    for (int i = 0; i < boxes; ++i) {
        m_signalMapper->setMapping(m_boxManager->boxes().at(i), i);
    }
}
