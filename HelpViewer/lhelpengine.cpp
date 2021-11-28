#include "lhelpengine.h"



LHelpEngine::LHelpEngine(const QString &collectionFile, QObject *parent) : QHelpEngine(collectionFile, parent)
{
}



bool LHelpEngine::isEmpty(const QUrl &url) const
{
    return fileData(url).isEmpty();
}



QUrl LHelpEngine::fileUrl(const QString &searchFileName)
{
    const QStringList documentations = registeredDocumentations();
    QUrl firstFileUrl;

    for (const QString &documentation : qAsConst(documentations)) {
        const QList<QUrl> fileUrls = files(documentation, QString());

        for (const QUrl &fileUrl : qAsConst(fileUrls)) {
            const QString fileName = fileUrl.fileName();

            if (!fileName.section(QLatin1Char('.'), -1).startsWith(QLatin1String("htm")))
                continue;

            if (firstFileUrl.isEmpty() && !isEmpty(fileUrl)) {
                if (fileName.startsWith(searchFileName))
                    return fileUrl;
                firstFileUrl = fileUrl;
            }

            if (fileName.startsWith(searchFileName)) {
                if (!isEmpty(fileUrl))
                    return fileUrl;
                if (!firstFileUrl.isEmpty())
                    return firstFileUrl;
            }
        }
    }

    return firstFileUrl;
}
