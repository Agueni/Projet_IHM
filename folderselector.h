#ifndef FOLDERSELECTOR_H
#define FOLDERSELECTOR_H
#include <QFileSystemModel>
#include <visitpath.h>

#include "ui_folderselector.h"

class folderSelector : public QDialog, private Ui::folderSelector
{
    Q_OBJECT
public:
    explicit folderSelector(QWidget *parent = nullptr);
    bool updateCurrentPath(QString);
    void on_elementListView_doubleClicked(const QModelIndex &index);
    void openEditor();
    void openImage();
    void openDirectory();
    void on_lePath_returnPressed();
    QString getPath();
    //void showContextMenu(const QPoint &pos);
    QFileSystemModel *dirModel;
    QFileSystemModel *fileModel;
    visitpath *visitPath;
    QString actualFile;
    QString sPath;
    QString directory;
private slots:
    void on_Path_textChanged();
    void on_validate_folder_button_clicked();
    void on_treeViewrecent_clicked(const QModelIndex &index);
    void on_elementlistView_doubleClicked(const QModelIndex &index);
    void on_treeViewrecent_doubleClicked(const QModelIndex &index);
};

#endif // FOLDERSELECTOR_H
