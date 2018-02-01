#include "models/namespacebaritem.h"
#include "models/treeitem.h"


namespace duly_gui {
namespace models {

void NameSpaceBarItem::setPath(const QString &s)
{
    if (s == m_path)
        return;
    m_path = s;
    emit pathChanged(s);
}

void NameSpaceBarItem::setAsChild(bool b)
{
    if (b == m_asChild)
        return;
    m_asChild = b;
    emit asChildChanged(b);
}

void NameSpaceBarItem::setIdx(const QModelIndex &idx)
{
    if (m_idx == idx)
        return;
    m_idx = idx;
    emit idxChanged(idx);
}

void NameSpaceBarItem::setModel(duly_gui::models::TreeItem *model)
{
    m_model = model;
}
}
}
