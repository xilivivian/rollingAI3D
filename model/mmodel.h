#ifndef MMODEL_H
#define MMODEL_H

#include <QObject>
#include <QStandardItemModel>
#include <QVariant>

class MModel : public QStandardItemModel
{
public:
    MModel();


   MModel(int rows, int columns, QObject *parent = Q_NULLPTR);

    QVariant data(const QModelIndex &index,int role = Qt::DisplayRole)const;
};

#endif // MMODEL_H
