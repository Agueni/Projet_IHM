#include "homewindow.h"

#include <QListWidget>
#include <QDebug>

homewindow::homewindow(AlbumDb &album_db, QWidget *parent):
    QMainWindow(parent), db(album_db)
{
    setupUi(this);
    this->setStyleSheet("QWidget{background-color: rgb(73,81,92);}"
                          "QPushButton{background-color:rgb(204,255,255);}"
                          "QListView{background-color: rgb(224, 224, 224);}");

    album_list->setEditTriggers(QAbstractItemView::NoEditTriggers);
    model = new QStringListModel(this);
    album_list->setModel(model);
    album_path_list = new QStringList();
    *album_path_list << db.getAlbums();
    model->setStringList(*album_path_list);
    open_album_button->setDisabled(true);
    remove_album_button->setDisabled(true);
    connect(album_list->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                     this, SLOT(on_album_selection_changed()));
}

void homewindow::on_selectFolder_button_clicked()
{
    folderSelector f;
    if(f.exec()){
        Path->setText(f.getPath());
    }
}

void homewindow::on_exit_button_clicked()
{
    exit(0);
}

void homewindow::on_open_album_button_clicked()
{
    QModelIndex localCurrentIndex = album_list->currentIndex();
    QVariant localData = localCurrentIndex.data();
    album_window = new albumwindow(db);
    album_window->addFolder(localData.toString());
    album_window->setAlbum_path(localData.toString());
    album_window->show();
    qDebug() << "open "<<localData.toString();

}
void homewindow::on_Path_textChanged()
{
    QFileInfo dir(Path->text());
    if (dir.exists()) import_button->setDisabled(false);
    else import_button->setDisabled(true);
}

void homewindow::on_remove_album_button_clicked()
{
    QModelIndex localCurrentIndex = album_list->currentIndex();
    QVariant localData = localCurrentIndex.data();
    int i = album_path_list->indexOf(localData.toString());
    album_path_list->removeAt(i);
    model->setStringList(*album_path_list);
    //TODO enlever album de la bdd
}

void homewindow::on_album_selection_changed()
{
    open_album_button->setDisabled(!album_list->currentIndex().isValid());
    remove_album_button->setDisabled(!album_list->currentIndex().isValid());
}

void homewindow::on_import_button_clicked(){
    if (!album_path_list->contains(Path->text())){
        *album_path_list << Path->text();
        model->setStringList(*album_path_list);
        db.addAlbum(Path->text());
    }
}
