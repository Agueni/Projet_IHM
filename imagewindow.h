#ifndef IMAGEWINDOW_H
#define IMAGEWINDOW_H

#include "ui_imagewindow.h"
#include "ui_albumwindow.h"
#include <QString>
#include <QListWidgetItem>
#include <map>
#include <QPoint>
#include <QRubberBand>
#include <QRect>
typedef std::map<QString,QListWidgetItem>::iterator it;
typedef std::map<QString,QListWidgetItem> iw_map;
class Imagewindow : public QWidget, private Ui::Imagewindow
{
    Q_OBJECT

public:
    explicit Imagewindow(iw_map &image_widget_map,QWidget *parent = nullptr);
    void setImage(QString image);
    void setImageIt();

    std::map<QString,QListWidgetItem> &iwm;
    it it_c, it_begin, it_end;
    void setImageIt(it it_image);
    int conteur = 0;
private:
    QPoint origin;
    QRubberBand *rubberBand;
    QRect rect;

private slots:
    void resizeEvent(QResizeEvent*);
    void on_button_rotation_clicked();
    void on_button_previous_clicked();
    void on_button_next_clicked();
    void on_button_resize_clicked();
    void setImageInformation(QString path);
    void on_button_crop_clicked();
    void moussePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void on_button_delete_clicked();
};

#endif // IMAGEWINDOW_H
