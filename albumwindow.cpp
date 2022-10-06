#include "albumwindow.h"


albumwindow::albumwindow(AlbumDb &album_db, QWidget *parent) :
    QWidget(parent), db(album_db)
{
    setupUi(this);
    this->setStyleSheet("QWidget{background-color: rgb(73,81,92);}"
                        "QPushButton{background-color:rgb(204,255,255);}"
                        "QTreeView{background-color: rgb(224, 224, 224);}"
                        "QMenu{background-color: rgb(255, 255, 255);}"
                        "QComboBox{background-color: rgb(255, 255, 255);}");
    connect(photolist_listWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this ,SLOT(openImage(QListWidgetItem*)));
    connect(colorfilter_textEdit, SIGNAL(textChanged()),this,SLOT(addColorFilter()));
    connect(colorfilter_listWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this ,SLOT(removeColorFilter(QListWidgetItem*)));
    connect(tagfilter_textEdit, SIGNAL(textChanged()),this,SLOT(addTagFilter()));
    connect(tagfilter_listWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this ,SLOT(removeTagFilter(QListWidgetItem*)));
    connect(search_lineEdit,SIGNAL(returnPressed()),this,SLOT(on_search_button_clicked()));

    photolist_listWidget->setViewMode(QListWidget::IconMode);
    photolist_listWidget->setIconSize  (QSize(200,200));
    photolist_listWidget->setResizeMode(QListWidget::Adjust);
    photolist_listWidget->setDragDropMode(QAbstractItemView::NoDragDrop);
}

void albumwindow::setAlbum_path(const QString &newAlbum_path)
{
    album_path = newAlbum_path;
}

void albumwindow::addImage(QFile &path){

    QIcon icon(path.fileName());
    QListWidgetItem * widget = new QListWidgetItem(icon, QFileInfo(path.fileName()).fileName());
    widget->setWhatsThis(path.fileName());
    image_widget_map[path.fileName()] = *widget;
    photolist_listWidget->addItem(widget);
}

void albumwindow::removeImage(QString &image_path)
{
    photolist_listWidget->removeItemWidget(&image_widget_map[image_path]);
}


void albumwindow::addFolder(QString path){
    QSqlQuery query = db.getImages(path);
    while (query.next()){
        QFile file(query.value(0).toString());
        addImage(file);
    }
//    QDir dir(path);
//    QStringList images = dir.entryList(QStringList() << "*.jpg" << "*.JPG",QDir::Files);
//    foreach(QString filename, images) {
//        QFile file(path + "/" + filename);
//        addImage(file);
    //    }
}

void albumwindow::addFromQuery(QSqlQuery query){
    while (query.next()){
    QFile file(query.value(0).toString());
        addImage(file);
    }
}

void albumwindow::clearAll(){
//    it iterator;
//    for (iterator = image_widget_map.begin() ;iterator != image_widget_map.end(); ++iterator){
//        photolist_listWidget->removeItemWidget(&(iterator->second));
//        QString path(iterator->first);
//        removeImage(path);
//        qDebug()<<"here";
//    }
    image_widget_map.clear();
    photolist_listWidget->clear();
    photolist_listWidget->update();
}

void albumwindow::openImage(QListWidgetItem *item){
    std::cout << item->whatsThis().toStdString() << std::endl;
    Imagewindow* iw = new Imagewindow(image_widget_map);
    iw->setImageIt(image_widget_map.find(item->whatsThis()));
    iw->show();
}

void albumwindow::addColorFilter(){
    QString color = colorfilter_textEdit->toPlainText();
    if (color != NULL && color.at(color.length()-1) == "\n")
    {
        colorfilter_textEdit->clear();
        color.chop(1);
        colorfilter_listWidget->addItem(color);
        updatePhotoView();
    }
}

void albumwindow::addTagFilter(){
    QString tag = tagfilter_textEdit->toPlainText();
    if (tag != NULL && tag.at(tag.length()-1) == "\n")
    {
        tagfilter_textEdit->clear();
        tag.chop(1);
        tagfilter_listWidget->addItem(tag);
        updatePhotoView();
    }
}

void albumwindow::removeColorFilter(QListWidgetItem *color){
    colorfilter_listWidget->removeItemWidget(color);
    delete color;
}

void albumwindow::removeTagFilter(QListWidgetItem *tag){
    tagfilter_listWidget->removeItemWidget(tag);
    delete tag;
}

void albumwindow::on_search_button_clicked(){
    qDebug() << search_lineEdit->text();
    clearAll();
    db.getImages(album_path,"album");
    QSqlQuery query = db.filterByName(search_lineEdit->text(),"album");
    addFromQuery(query);
}


void albumwindow::updatePhotoView(){
    //ICI FAIRE UNE REQUETE A LA BD POUR OBTENIR LA LISTE DES PHOTOS SATSIFAISANT LES FILTRES ET LE TRI
    QStringList colorFilters = getAllColorsFilter();
    QStringList tagFilter = getAllTagFilter();
}


QStringList albumwindow::getAllColorsFilter(){
    // Here is a little trick to get all elements in a qwidgetlist using findItems
    QList<QListWidgetItem *> items = colorfilter_listWidget->findItems("", Qt::MatchContains);
    QStringList toReturn= QStringList();
    foreach(QListWidgetItem * item, items)
        toReturn.append(item->text());
    return toReturn;
}

QStringList albumwindow::getAllTagFilter(){
    // Here is a little trick to get all elements in a qwidgetlist using findItems
    QList<QListWidgetItem *> items = tagfilter_listWidget->findItems("", Qt::MatchContains);
    QStringList toReturn= QStringList();
    foreach(QListWidgetItem * item, items)
        toReturn.append(item->text());
    return toReturn;
}

