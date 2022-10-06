#include "imagewindow.h"
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QDebug>
#include <iostream>
#include <stdio.h>
#include <imageresize.h>
#include <QMouseEvent>
#include <QRubberBand>
#include <QRect>
#include <albumdb.h>
#include <albumwindow.h>
Imagewindow::Imagewindow(iw_map &image_widget_map,QWidget *parent) : QWidget(parent), iwm(image_widget_map){
    setupUi(this);
    this->setStyleSheet("QWidget{background-color: rgb(73,81,92);}"
                        "QPushButton{background-color:rgb(204,255,255);}");


}
/* affichage de l'image sur le label a l'aide d'une chaine de caractère (chemain de l'image) */

void Imagewindow::setImage(QString image){
    QPixmap im(image);

    int w = image_view->width();
    int h = image_view->height();
    // set a scaled pixmap to a w x h window keeping its aspect ratio
    image_view->setPixmap(im.scaled(w,h,Qt::KeepAspectRatio));

    image_view->setAlignment(Qt::AlignCenter);
    conteur = 0;
    setImageInformation(image);

}
void Imagewindow::setImageInformation(QString path)
{   QString ext,type ;
    imageName->setText(path.split("/").last().split(".").first());

    for(int i=path.length();i>path.length()-4;i--){
       ext = ext + path[i];
    }
    for(int j=ext.length();j>0;j--){
      type = type + ext[j];
    }
    imageType->setText(type);
}
    /* affichage de l'image sur le label en utilisant un iterator */

void Imagewindow::setImageIt(it it_image){
    it_c = it_image;
    setImage(it_c->first);
    if(it_c == iwm.begin()) button_previous->setDisabled(true);
    else button_previous->setDisabled(false);
    if (++it_c == iwm.end() ) button_next->setDisabled(true);
    else button_next->setDisabled(false);
    --it_c;
    conteur=0;
}

/* slot pour effetuer des retation sur l'image */
void Imagewindow::on_button_rotation_clicked(){
    QTransform t;
    QImage imageInitial = (QImage)it_c->first;
    conteur+=90;
    QImage img = imageInitial.transformed(t.rotate(conteur));
    image_view->setPixmap(QPixmap::fromImage(img));
    imageInitial = img.transformed(t.rotate(0));
}

/*slots pour changer l'image que ce soit pour avoir l'image suivant ou la precedante */

void Imagewindow::on_button_next_clicked(){
   if( ++it_c != iwm.end() ){
        setImageIt(it_c);
    }
}

void Imagewindow::on_button_previous_clicked(){
   if( it_c != iwm.begin() ){
        setImageIt(--it_c);
    }
}

void Imagewindow::resizeEvent(QResizeEvent*){
    setImageIt(it_c);
}

       /*slot pour redimentionner l'image */
void Imagewindow::on_button_resize_clicked()
{
    imageresize r;
   if(r.exec()){
       QImage imageR = (QImage)it_c->first;
       image_view->setFixedSize(r.newWidth,r.newHeight);
       image_view->setPixmap(QPixmap::fromImage(imageR));
       image_view->setScaledContents( true );
       image_view->setAlignment(Qt::AlignHCenter);

   }
}
   /* slots pour rogner l'image a l'aide de QRubberband et QRect en manipulant les évènements de la souris  */
void Imagewindow::moussePressEvent(QMouseEvent *event){

    origin = event->pos();
   /* if(!rubberBand){
        rubberBand = new QRubberBand(QRubberBand::Rectangle,this);

    }*/
}

void Imagewindow::mouseMoveEvent(QMouseEvent *event){

   //rubberBand->setGeometry(QRect(origin,event->pos()).normalized());
   //button_crop->setVisible(true);
   //rubberBand->show();
    rect = QRect(origin,event->pos()).normalized();

  }



void Imagewindow::mouseReleaseEvent(QMouseEvent *event){
    rect.setTopLeft(origin);
    rect.setBottomRight(event->pos());

}
void Imagewindow::on_button_crop_clicked()
{
     //rubberBand->hide();
     image_view->setPixmap(grab(rect));
     image_view->setScaledContents(true);
     //button_crop->setVisible(false);
     // button_crop->isEnabled();

}



void Imagewindow::on_button_delete_clicked()
{

}

