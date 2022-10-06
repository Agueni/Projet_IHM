#include "imageresize.h"

imageresize::imageresize(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    this->setStyleSheet("QDialog{background-color: rgb(73,81,92);}"
                     "QPushButton{background-color:rgb(204,255,255);}"
                     "QTreeView{background-color: rgb(224, 224, 224);}");
}
 /*slots liés au button de  la fenetre imageresize des images */

void imageresize::on_buttonBox_accepted()
{

}


void imageresize::on_buttonBox_rejected()
{

}


void imageresize::on_widthValue_textEdited(const QString &arg1)
{
    newWidth = widthValue->text().toInt();
}


void imageresize::on_heightValue_textEdited(const QString &arg1)
{
    newHeight = heightValue->text().toInt();
}
