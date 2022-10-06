#include "homewindow.h"
#include "albumwindow.h"
#include "albumdb.h"
#include <QApplication>
#include <QFile>
#include <QFileInfo>
#include <QCoreApplication>
#include <QDebug>
#include "folderselector.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //PROJECT_PATH chemin du projet
    AlbumDb db(QString(PROJECT_PATH) +"/data.db");
    db.addAlbum(QString(PROJECT_PATH) + "chatons");
    homewindow w(db);

//    albumwindow album(db);
//    album.show();

//    folderSelector fd;
//    fd.show();

    w.show();

    return a.exec();
}
