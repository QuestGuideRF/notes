#include "taglistmodel.h"
#include "tagpool.h"
#include <QDebug>
TagListModel::TagListModel(QObject *parent) : QAbstractListModel(parent), m_tagPool{ nullptr } { }
void TagListModel::setTagPool(TagPool *tagPool)
{
    beginResetModel();
    m_tagPool = tagPool;
    connect(tagPool, &TagPool::dataReset, this, [this] {
        beginResetModel();
        updateTagData();
        endResetModel();
    });
    endResetModel();
}
void TagListModel::setModelData(const QSet<int> &data)
{
    if (m_tagPool == nullptr) {
        qDebug() << __FUNCTION__ << "Пул тегов еще не инициализирован";
        return;
    }
    beginResetModel();
    m_ids = data;
    updateTagData();
    endResetModel();
}
void TagListModel::addTag(int tagId)
{
    if (m_tagPool == nullptr) {
        qDebug() << __FUNCTION__ << "Пул тегов еще не инициализирован";
        return;
    }
    if (m_tagPool->contains(tagId)) {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        m_data.append(m_tagPool->getTag(tagId));
        endInsertRows();
    } else {
        qDebug() << __FUNCTION__ << "Тега нет в пуле:" << tagId;
    }
}
int TagListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_data.count();
}
QVariant TagListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return {};
    }
    const TagData &tag = m_data[index.row()];
    switch (static_cast<TagListRole>(role)) {
    case TagListRole::IdRole:
        return tag.id();
    case TagListRole::NameRole:
        return tag.name();
    case TagListRole::ColorRole:
        return tag.color();
    }
    return {};
}
void TagListModel::updateTagData()
{
    m_data.clear();
    for (const auto &id : std::as_const(m_ids)) {
        if (m_tagPool->contains(id)) {
            m_data.append(m_tagPool->getTag(id));
        } else {
            qDebug() << __FUNCTION__ << "Тега нет в пуле:" << id;
        }
    }
}