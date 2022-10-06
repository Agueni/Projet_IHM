#ifndef ALBUMDB_H
#define ALBUMDB_H
#include <QtSql/QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QDateTime>
#include <QSqlError>
#include <QSqlRecord>
#include <QDir>
#include <QList>

class AlbumDb
{
public:
    AlbumDb(const QString& path);
    bool resetAlbums();
    void addAlbum(QString album_path);
    bool addImage(QString img_path, QString album_path);
    bool removeImage(QString img_path);
    QSqlQuery getImages(QString album_path, QString viewOut = "image_view");
    QList<QString> getAlbums();
    QSqlQuery getAllImages();
    bool editImageProperties(QString img_path, QString namePropertie, QString value);
    QSqlQuery filterByName(QString name, QString viewIn = "images", QString viewOut = "images_view");

    //gestion des tags
    bool addTag(QString tag);
    bool addImageTag(QString img_path, QString tag);
    bool removeImageTag(QString img_path, QString tag);
    QSqlQuery getTagsOfImage(QString img_path);
    QSqlQuery getImagesByTag(QString tag);

    void test();
    void testTags();
    void affichageQuery(QSqlQuery query);
    void setAlbum_path(const QString &newAlbum_path);

private:
    QSqlDatabase db;
};

#endif // ALBUMDB_H
