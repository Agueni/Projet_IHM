#include "folderselector.h"
#include<QFileSystemModel>
#include <QStandardPaths>
#include <iostream>
#include <visitpath.h>
#include <QDebug>
#include <QUrl>
#include <imagewindow.h>
#include <albumwindow.h>

folderSelector::folderSelector(QWidget *parent) :QDialog(parent){
    setupUi(this);
    /*styleSheet of foderselector interface  */
    this->setStyleSheet("QDialog{background-color: rgb(73,81,92);}"
                     "QPushButton{background-color:rgb(204,255,255);}"
                     "QTreeView{background-color: rgb(224, 224, 224);}");

    /*  Accéder au dossiers contenant des image dans la machine de l'utilisateur */
 sPath =QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
 dirModel = new QFileSystemModel(this);
 dirModel->setRootPath(sPath);
 dirModel->setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
 treeViewrecent->setModel(dirModel);
 for (int i = 1; i < dirModel->columnCount(); ++i)
        treeViewrecent->hideColumn(i);
 fileModel = new QFileSystemModel();
 fileModel->setRootPath(sPath);
 fileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
 QStringList sDriveFilters;
 sDriveFilters << "*.png" << "*.jpeg" << "*.jpg";
 fileModel->setNameFilterDisables(false);
 fileModel->setNameFilters(sDriveFilters);
  elementlistView->setModel(fileModel);
 for (int i = 1; i < fileModel->columnCount(); ++i)
        elementlistView->hideColumn(i);



  visitPath = new visitpath(sPath);
  updateCurrentPath(sPath);


}

/* méthode pour mettre à jour le chemain du dossier selectioné   */

bool folderSelector::updateCurrentPath(QString path) {


    elementlistView->setRootIndex(fileModel->setRootPath(path));

    treeViewrecent->setExpanded(dirModel->setRootPath(path), true);
    treeViewrecent->setCurrentIndex(dirModel->setRootPath(path));

    Path->setText(path);

    visitPath->addPath(path);

    return true;
}

/* récupérer le chemain du dossier choisi  */
 QString directory;
void folderSelector::on_treeViewrecent_clicked(const QModelIndex &index)
{
    QString dirPath = dirModel->fileInfo(index).absoluteFilePath();
    directory = dirPath;
    updateCurrentPath(dirPath);

}

/* slots pour l'ouverture des dossiers et pour la validation du choix effectuer dans l'arboresence*/
void folderSelector::on_treeViewrecent_doubleClicked(const QModelIndex &index)
{
    openEditor();
}

void folderSelector::on_elementlistView_doubleClicked(const QModelIndex &index)
{
    QString dirPath = dirModel->fileInfo(index).absoluteFilePath();
    actualFile = dirPath;

    QDir pathDir(dirPath);
    if (pathDir.exists()) {
        openDirectory();
    } else {
        openImage();

    }
}
void folderSelector::openImage()
{
//    Imagewindow *w = new Imagewindow();
//    w->setImage(actualFile);

//    w->show();

}
void folderSelector::openEditor()
{
    //albumwindow *w = new albumwindow();
    //w->addFolder(directory);
    //w->show();

}
void folderSelector::openDirectory()
{
    updateCurrentPath(actualFile);


}

QString folderSelector::getPath()
{
 return QString(Path->text());
}

void folderSelector::on_Path_textChanged()
{
    QFileInfo dir(Path->text());
    if (dir.exists()) validate_folder_button->setDisabled(false);
    else validate_folder_button->setDisabled(true);
}

void folderSelector::on_validate_folder_button_clicked()
{
    this->accept();
}
