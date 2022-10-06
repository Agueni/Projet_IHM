#ifndef VISITPATH_H
#define VISITPATH_H
#include <QString>
#include<QVector>
class visitpath
{
public:
    visitpath(QString currentPath);
    void addPath(QString);
    QString back();
    QString forward();

public:
    QString currentPath;
    QVector<QString> precedingPath;
    QVector<QString> followingPath;
};

#endif // VISITPATH_H
