#ifndef IMAGERESIZE_H
#define IMAGERESIZE_H

#include "ui_imageresize.h"

class imageresize : public QDialog, private Ui::imageresize
{
    Q_OBJECT

public:
    explicit imageresize(QWidget *parent = nullptr);
    int newWidth,newHeight;

private slots:
    void on_buttonBox_accepted();
    void on_heightValue_textEdited(const QString &arg1);
    void on_widthValue_textEdited(const QString &arg1);
    void on_buttonBox_rejected();
};

#endif // IMAGERESIZE_H
