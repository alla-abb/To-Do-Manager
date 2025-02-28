#include "dialog.h"
#include "ui_dialog.h"
#include <qdatetime.h>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_comboBox_currentIndexChanged(int index)
{
    switch (index) {
    case 1:
        ui->calendarWidget->selectedDate();

        break;
    default:
        break;
    }
}

