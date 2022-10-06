#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include "ui_homewindow.h"
#include "folderselector.h"
#include "albumdb.h"
#include "albumwindow.h"
#include <QStringList>
#include <QStringListModel>

class homewindow : public QMainWindow, private Ui::homewindow
{
    Q_OBJECT

public:
    explicit homewindow(AlbumDb &album_db, QWidget *parent = nullptr);
private:
    AlbumDb &db;
    QStringListModel *model;
    QStringList *album_path_list;
    albumwindow *album_window;
private slots:
    void on_import_button_clicked();
    void on_selectFolder_button_clicked();
    void on_exit_button_clicked();
    void on_open_album_button_clicked();
    void on_Path_textChanged();
    void on_remove_album_button_clicked();
    void on_album_selection_changed();
};

#endif // HOMEWINDOW_H
