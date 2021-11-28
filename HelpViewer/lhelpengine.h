#ifndef LHELPENGINE_H
#define LHELPENGINE_H

#include <QHelpEngine>

class LHelpEngine : public QHelpEngine
{
    Q_OBJECT

public:
    explicit LHelpEngine(const QString &collectionFile, QObject *parent = nullptr);

    bool isEmpty(const QUrl &url) const;
    QUrl fileUrl(const QString &searchFileName);
};

#endif // LHELPENGINE_H
