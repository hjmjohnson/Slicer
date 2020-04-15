
#ifndef _qSlicerQListQVariantMapMetaType_h
#define _qSlicerQListQVariantMapMetaType_h

#include <QMetaType>
#include <QVariant>

template <typename T> class QList;
template <class Key, class T> class QMap;
using QVariantMap = QMap<QString, QVariant>;

Q_DECLARE_METATYPE(QList<QVariantMap>);

#endif
