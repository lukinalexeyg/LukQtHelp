#ifndef HELPENGINE_H
#define HELPENGINE_H

#include <QHelpEngine>

class HelpEngine : public QHelpEngine
{
    Q_OBJECT

public:
    explicit HelpEngine(const QString &collectionFile, QObject *parent = nullptr);

    bool isEmpty(const QUrl &url) const;
    QUrl fileUrl(const QString &searchFileName);
};

#endif // HELPENGINE_H
