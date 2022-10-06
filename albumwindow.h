#ifndef ALBUMWINDOW_H
#define ALBUMWINDOW_H

#include "ui_albumwindow.h"
#include <QFile>
#include <QIcon>
#include <QListWidgetItem>
#include <QString>
#include <QFileInfo>
#include <iostream>
#include <QDir>
#include <QObject>
#include <QList>
#include <QKeyEvent>
#include "imagewindow.h"
#include <QMap>
#include <map>
#include <albumdb.h>

typedef std::map<QString,QListWidgetItem>::iterator it;

class albumwindow : public QWidget, private Ui::albumwindow
{
    Q_OBJECT

public:
    explicit albumwindow(AlbumDb &album_db, QWidget *parent = nullptr);
    AlbumDb &db;
    std::map<QString,QListWidgetItem> image_widget_map;
    void addFolder(QString path);
    void addFromQuery(QSqlQuery query);
    void clearAll();
    void setAlbum_path(const QString &newAlbum_path);

private slots:
    void addImage(QFile &path);
    void removeImage(QString &image_path);
    void openImage(QListWidgetItem *item);
    void addColorFilter();
    void removeColorFilter(QListWidgetItem *color);
    void addTagFilter();
    void removeTagFilter(QListWidgetItem *tag);
    void on_search_button_clicked();
private :
    QString album_path;
    void updatePhotoView();
    QStringList getAllColorsFilter();
    QStringList getAllTagFilter();
};

#endif // ALBUMWINDOW_H
