#include "mmodel.h"
MModel::MModel()
{

}

MModel::MModel(int rows, int columns, QObject *parent):QStandardItemModel(rows, columns, parent)
{

}
#if 1
QVariant MModel::data(const QModelIndex &index, int role) const
{
#if 0
    if(!index.isValid())
    {
        return QVariant();
    }
    if(role == Qt::BackgroundColorRole)
    {
       // return QColor(Qt::black);
        return QColor(Qt::white);
    }else if(role == Qt::TextColorRole)
    {
       // return QColor(Qt::white);
        return QColor(Qt::black);
    }
    else{
        return QStandardItemModel::data(index,role);
    }
#endif
    return QStandardItemModel::data(index,role);
}
#endif
