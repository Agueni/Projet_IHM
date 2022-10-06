#include "albumdb.h"

/** Connect to the database and ensure that the table exist
 * @brief AlbumDb::AlbumDb
 * @param path the path of the database
 */
AlbumDb::AlbumDb(const QString& path){
    if(QSqlDatabase::isDriverAvailable("QSQLITE")) qDebug()<< "driver ok";
    else qDebug()<< "driver err";
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path);

    if (!db.open()){
         qDebug() << "Error: connection with database failed";}
    else{
         qDebug() << "Database: connection ok";}
    QSqlQuery create_table_query(db);
    create_table_query.prepare("CREATE TABLE IF NOT EXISTS images(image_path text primary key, "
                            "album_path text, date text, type text, main_color text,"
                            "stars integer DEFAULT 0, comment text);");
    if(!create_table_query.exec()){
        qDebug() << "create_table images error :" << create_table_query.lastError();
    }
    else{
        qDebug()<< "images table ok";
    }


    create_table_query.prepare("CREATE TABLE IF NOT EXISTS tags(tag text primary key);");
    if(!create_table_query.exec()){
        qDebug() << "create_table tags error :" << create_table_query.lastError();
    }
    else{
        qDebug()<< "tags table ok";
    }


    create_table_query.prepare("CREATE TABLE IF NOT EXISTS relationImagesTags(image_path text, tag text, CONSTRAINT unicitee UNIQUE(image_path, tag), "
                               "FOREIGN KEY (image_path) REFERENCES images(image_path), "
                               "FOREIGN KEY (tag) REFERENCES tags(tag));");
    if(!create_table_query.exec()){
        qDebug() << "create_table relationImagesTags :" << create_table_query.lastError();
    }
    else{
        qDebug()<< "relationImagesTags table ok";
    }


}

bool AlbumDb::resetAlbums(){
    QSqlQuery query(db);

    query.prepare("DELETE FROM images");

    if(!query.exec()){
         qDebug() << "resetAlbums error:" << query.lastError();
         return false;
    }

    return true;

}

/** add all images contained in album_path in the database
 * @brief AlbumDb::addAlbum
 * @param album_path the album's path
 */
void AlbumDb::addAlbum(QString album_path){
    QDir dir(album_path);
    QStringList images = dir.entryList(QStringList() << "*.jpg" << "*.JPG",QDir::Files);
    foreach(QString filename, images) {
        QFile file(album_path + "/" + filename);
        addImage(QFileInfo(file).absoluteFilePath(),album_path);
    }
}

QSqlQuery AlbumDb::getAllImages(){
    QSqlQuery query(db);
    QString localQString = QString("select * from images");
    query.prepare(localQString);

    if(!query.exec()){
         qDebug() << "addImage error:" << query.lastError();
    }

    return query;
}

/** add an image in database
 * @brief AlbumDb::addImage
 * @param img_path the image's path
 * @param album_path the album's path
 * @return if image was added or not
 */
bool AlbumDb::addImage(QString img_path, QString album_path){
   bool success = false;
   QSqlQuery query(db);
   //stars tags and comment are initially empty or default
   query.prepare("INSERT INTO images (image_path, album_path, date, type,main_color)"
                 "VALUES (?, ?, ?, ?, ?);");

   query.addBindValue(img_path);
   query.addBindValue(album_path);
   QFileInfo fileInfo(img_path);
   QDateTime localBirthTime = fileInfo.birthTime();
   QString iso_birthTime = localBirthTime.toString(Qt::ISODate);

   query.addBindValue( iso_birthTime);
   //TODO mettre le bon type
   query.addBindValue("JPG");
   //TODO mettre la bonne couleur
   query.addBindValue("#000000");


   if(query.exec()){
       success = true;
   }
   else{
        qDebug() << "addImage error:" << query.lastError();
   }

   return success;
}

/** remove image with image_path key from database
 * @brief AlbumDb::removeImage
 * @param img_path
 * @return if query was succesful
 */
bool AlbumDb::removeImage(QString img_path){
    QSqlQuery query(db);
    query.prepare("delete from images where image_path = :path ");
    query.bindValue(":path",img_path);
    if (!query.exec()){
        qDebug() << "removeImage err: " << query.lastError();
        return false;
    }
    return true;
}

/** get the list of image_path in album_path in the database
 * @brief AlbumDb::getImages
 * @param album_path the path of the album
 * @return the list of image_path in album_path in the database
 */
QSqlQuery AlbumDb::getImages(QString album_path, QString viewOut){

    QSqlQuery query(db);
    QString d("DROP VIEW IF EXISTS " + viewOut);
    query.prepare(d);
    if (!query.exec()){
        qDebug() << "getImages 1 err: " << query.lastError();
    }

    QString r = "CREATE VIEW "+viewOut+" AS select * from images "
            "where album_path = \""+album_path +"\"";
    query.prepare(r);

//    query.prepare("select image_path "
//                  "from images "
//                  "where album_path = :path ");
//    query.bindValue(":path",album_path);

    if (!query.exec()){
        qDebug() << "getImage 2 err: " << query.lastError();
    }

    QString v = "select * from "+viewOut;
    query.prepare(v);
    if (!query.exec()){
        qDebug() << "getImage 3 err: " << query.lastError();
    }

    return query;


}

/** get the list of distinct album_path in database
 * @brief AlbumDb::getAlbums
 * @return the list of distinct album_path in database
 */
QList<QString> AlbumDb::getAlbums(){
    QSqlQuery query(db);
    query.prepare("select distinct album_path from images");
    QList<QString> list;
    if (!query.exec()){
        qDebug() << "getAlbums err: " << query.lastError();
    }
    while(query.next()){
        list.append(query.value(0).toString());
    }
    return list;
}


void AlbumDb::affichageQuery(QSqlQuery query){

    while(query.next()){
        qDebug() << query.value(0).toString() <<  query.value(1).toString();
    }

}


void AlbumDb::test(){
    qDebug() << "debut test.";

    //test addalbum
    affichageQuery(getAllImages());

    if(addImage("image test", "album test")){
        affichageQuery(getAllImages());
    }

    if(removeImage("image test")){
        affichageQuery(getAllImages());
    }



    qDebug() << "fin test.";
}


bool AlbumDb::addTag(QString tag){

    bool success = false;
    QSqlQuery query(db);

    query.prepare("INSERT INTO tags (tag)"
                  "VALUES (?);");

    query.addBindValue(tag);

    if(query.exec()){
        success = true;
    }
    else{
         qDebug() << "addTag error:" << query.lastError();
    }

    return success;

}


bool AlbumDb::addImageTag(QString img_path, QString tag){

    bool success = false;
    QSqlQuery query(db);

    QString a = "INSERT INTO relationImagesTags (image_path, tag) ";
    QString b = "VALUES (\"";
    QString c = "\", \"";
    QString d = "\");";
    QString result = a + b + img_path + c + tag + d;
    //qDebug() << result;
    query.prepare(result);

    //query.bindValue(":path",img_path);
    //query.bindValue(":tag",tag);

    if(query.exec()){
        success = true;
    }
    else{
         qDebug() << "addImageTag error:" << query.lastError();
    }

    return success;
}
bool AlbumDb::removeImageTag(QString img_path, QString tag){

    QSqlQuery query(db);

    QString a = "delete from relationImagesTags where (image_path = \"";
    QString b = "\" AND tag = \"";
    QString c = "\");";
    QString result = a + img_path + b + tag + c;
    //qDebug() << result;
    query.prepare(result);

    if (!query.exec()){
        qDebug() << "removeImageTag err: " << query.lastError();
        return false;
    }
    return true;
}
QSqlQuery AlbumDb::getTagsOfImage(QString img_path){
    QSqlQuery query(db);
    query.prepare("select tag from relationImagesTags where image_path = :path");
    query.bindValue(":path",img_path);
    if (!query.exec()){
        qDebug() << "removeImageTag err: " << query.lastError();
    }
    return query;
}

QSqlQuery AlbumDb::getImagesByTag(QString tag){
    QSqlQuery query(db);
    query.prepare("select image_path from relationImagesTags where tag = :tag");
    query.bindValue(":tag",tag);
    if (!query.exec()){
        qDebug() << "removeImageTag err: " << query.lastError();
    }
    return query;
}

bool AlbumDb::editImageProperties(QString img_path, QString namePropertie, QString value){
    QSqlQuery query(db);
    QString a = "UPDATE images set " +namePropertie+ " = \""+ value +
              "\" where image_path = \"" + img_path +"\"";

    query.prepare(a);
//    query.bindValue(":prop",namePropertie);
//    query.bindValue(":val",value);
//    query.bindValue(":path",img_path);
    if (!query.exec()){
        qDebug() << "editImageProperties err: " << query.lastError();
        return false;
    }
    else return true;

}

QSqlQuery AlbumDb::filterByName(QString name, QString viewIn, QString viewOut){

    QSqlQuery query(db);
    QString d("DROP VIEW IF EXISTS " + viewOut);
    query.prepare(d);
    if (!query.exec()){
        qDebug() << "filterByName 1 err: " << query.lastError();
    }

    QString r = "CREATE VIEW "+viewOut+" AS select * from "+
            viewIn+" where image_path like \"%"+name+"%\"";
    query.prepare(r);
    if (!query.exec()){
        qDebug() << "filterByName 2 err: " << query.lastError();
    }

    QString v = "select * from "+viewOut;
    query.prepare(v);
    if (!query.exec()){
        qDebug() << "filterByName 3 err: " << query.lastError();
    }

    return query;
}

//QSqlQuery AlbumDb::getImagesSortedByName_toView(QString viewName){
//    QSqlQuery query(db);
//    query.prepare("select image_path from relationImagesTags where tag = :tag");
//    query.bindValue(":tag",tag);
//    if (!query.exec()){
//        qDebug() << "removeImageTag err: " << query.lastError();
//    }
//    return query;
//}

void AlbumDb::testTags(){
    //create_table_query.prepare("CREATE TABLE IF NOT EXISTS tags(tag text primary key);");
    //create_table_query.prepare("CREATE TABLE IF NOT EXISTS relationImagesTags(image_path text foreign key, tag text foreign key, CONSTRAINT unicitee UNIQUE(image_path, tag));");

    qDebug() << "debut test tags";

    addTag("A");
    addTag("B");
    addTag("C");
    addImageTag("/amuhome/b18001457/Bureau/Projet_IHM-main/chatons/chat1.jpg", "A");
    addImageTag("/amuhome/b18001457/Bureau/Projet_IHM-main/chatons/chat2.jpg", "A");
    addImageTag("/amuhome/b18001457/Bureau/Projet_IHM-main/chatons/chat2.jpg", "C");

    qDebug() << "img 1 tags :";
    affichageQuery(getTagsOfImage("/amuhome/b18001457/Bureau/Projet_IHM-main/chatons/chat1.jpg"));
    qDebug() << "img 2 tags :";
    affichageQuery(getTagsOfImage("/amuhome/b18001457/Bureau/Projet_IHM-main/chatons/chat2.jpg"));


    qDebug() << "imgs of tags A:";
    affichageQuery(getImagesByTag("A"));
    qDebug() << "imgs of tags B:";
    affichageQuery(getImagesByTag("B"));
    qDebug() << "imgs of tags C:";
    affichageQuery(getImagesByTag("C"));

    qDebug() << "remove";
//    removeImageTag("/amuhome/b18001457/Bureau/Projet_IHM-main/chatons/chat1.jpg", "A");
//    removeImageTag("/amuhome/b18001457/Bureau/Projet_IHM-main/chatons/chat2.jpg", "C");


    qDebug() << "img 1 tags :";
    affichageQuery(getTagsOfImage("/amuhome/b18001457/Bureau/Projet_IHM-main/chatons/chat1.jpg"));
    qDebug() << "img 2 tags :";
    affichageQuery(getTagsOfImage("/amuhome/b18001457/Bureau/Projet_IHM-main/chatons/chat2.jpg"));

    editImageProperties("/home/tom/Documents/cours/IHM/Projet/Projet_IHM/chatons/chat1.jpg","type","NIMP");
    qDebug() << "fin test tags";
}





