#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <qmessagebox.h>
#include <event.h>
#include <eventmanager.h>
#include <QCalendarWidget>

#include <qgraphicseffect.h>
#include <qpainter.h>
#include <qstandardpaths.h>

class InversionEffect : public QGraphicsEffect {
protected:
    void draw(QPainter *painter) override {
        // Create an image with the exact size of the widget
        QSize size = sourceBoundingRect(Qt::LogicalCoordinates).size().toSize();
        if (size.isEmpty()) {
            return;
        }

        QImage image(size, QImage::Format_ARGB32_Premultiplied);
        image.fill(Qt::transparent); // Start with a transparent image

        // Create a temporary QPainter for drawing into the image
        QPainter imagePainter(&image);
        imagePainter.setRenderHints(painter->renderHints() | QPainter::SmoothPixmapTransform | QPainter::Antialiasing);
        imagePainter.setCompositionMode(QPainter::CompositionMode_SourceOver);

        // Render the widget's content onto the image
        drawSource(&imagePainter);
        imagePainter.end();

        // Invert the colors of the image
        for (int y = 0; y < image.height(); ++y) {
            QRgb *line = reinterpret_cast<QRgb *>(image.scanLine(y));
            for (int x = 0; x < image.width(); ++x) {
                QRgb pixel = line[x];
                line[x] = qRgba(255 - qRed(pixel), 255 - qGreen(pixel), 255 - qBlue(pixel), qAlpha(pixel));
            }
        }

        // Draw the inverted image back to the widget using a high-quality painter
        painter->setRenderHint(QPainter::SmoothPixmapTransform);
        painter->drawImage(0, 0, image);
    }
};


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    ui->Dark->hide();
    ui->Dark_2->hide();

    QDateTime currentDateTime = QDateTime::currentDateTime();
    ui->dateTimeEdit->setDateTime(currentDateTime);

    QString savePath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);

    // Ensure the directory exists
    QDir().mkpath(savePath);
    QString filePath = savePath + "/savefile.json";

    QFile file(filePath);

    if (!file.exists()) {
        file.open(QIODevice::WriteOnly);
        file.close();
    }
    em =  new EventManager("savefile.json");

    em->loadEvents();

    events = em->events;

    updateWidgets();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_7_clicked()
{
    ui->pushButton->setEnabled(1);
    ui->addEv->setText("Create Event");
    ui->canceEv->setEnabled(0);
    ui->lineEdit->setText("");
    ui->lineEdit->setPlaceholderText("Title");

    ui->plainTextEdit->setPlainText("");
    ui->plainTextEdit->setPlaceholderText("Description");

    ui->comboBox->setCurrentIndex(0);

    ui->locationEdit->setText("");

    QDateTime currentDateTime = QDateTime::currentDateTime();
    ui->dateTimeEdit->setDateTime(currentDateTime);

    ui->priority->setText("");

}

void MainWindow::updateWidgets(){
    int up = 0;
    int today = 0;
    int perso = 0;
    int meet  = 0;
    int deadline = 0;
    for(const Event& event : events){
        if(event.dateTime  > QDateTime::currentDateTime()){
            up++ ;
        }
        if(event.dateTime.date() == QDateTime::currentDateTime().date()){
            today ++;
        }
        if(event.type.toLower().contains("personal")){
            perso++;
        }
        if(event.type.toLower().contains("meetings")){
            meet++;
        }
        if(event.type.toLower().contains("deadline")){
            deadline++;
        }
    }

    ui->upcoming_total->setText(QString::number(up));
    ui->todayC->setText(QString::number(today));
    ui->persoC->setText(QString::number(perso));
    ui->meetC->setText(QString::number(meet));
    ui->DeadC->setText(QString::number(deadline));

    on_todayBtn_clicked();
}

void MainWindow::on_addEv_clicked()
{
    updateWidgets();
    if (ui->pushButton->isEnabled()){
        updateWidgets();
        if(ui->lineEdit->text().isEmpty()){
            QMessageBox::information(nullptr, "make sure to enter the title", "make sure to enter the title");
            return;
        }
        else {
            Event *e  = new Event(ui->lineEdit->text(),ui->dateTimeEdit->dateTime(),ui->locationEdit->text(),ui->plainTextEdit->toPlainText(),ui->comboBox->currentText(),ui->priority->text().toInt());
            em->addEvent(*e);
            QMessageBox::information(nullptr, "event added","your event is added sucessfully");


            em->loadEvents();
            events = em->events;
            updateWidgets();
            on_pushButton_7_clicked();
        }
    }else {
        int i = ui->eventId->text().toInt();
        Event event = events.at(i);
        for(Event &e : events){
            if((e.name == event.name && e.description ==  event.description) || &e == &event){
                e.name = ui->lineEdit->text();
                e.description = ui->plainTextEdit->toPlainText();
                e.location = ui->locationEdit->text();
                e.type = ui->comboBox->currentText();
                e.priority = ui->priority->text().toInt();
                e.dateTime = ui->dateTimeEdit->dateTime();
            }
        }

        on_todayBtn_clicked();
        em->events = events;
        em->saveEvents();
        updateWidgets();
    }
}


// void MainWindow::newEvent(Event& event){
//     QWidget *w = new QWidget(this);
//     w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
//     // Create the label and button
//     QLabel *label = new QLabel(event.name, this);
//     QPushButton *button = new QPushButton("", this);  // Set button text
//     button->setStyleSheet("border: 1px solid grey; border-radius: 5px;");

//     // Set the stylesheet for the label
//     label->setStyleSheet("font: 500 12pt \"Roboto\"; color: rgb(109, 109, 109);");

//     // Set the size of the button
//     button->setFixedSize(22, 22);

//     // Create a horizontal layout
//     QHBoxLayout *layout = new QHBoxLayout(w);
//     layout->setContentsMargins(30,5,50,5);             // Set margin to 0
//     layout->setSpacing(50);           // Set layout spacing to 10

//     // Add the label and button to the layout
//     layout->addWidget(label);
//     layout->addWidget(button);

//     // Align the contents within the layout (left for label, right for button)
//     layout->setAlignment(label, Qt::AlignLeft);
//     // layout->setAlignment(button, Qt::AlignLeft);

//     // Set the layout for the widget
//     setLayout(layout);

//     ui->eventLayout->addWidget(w);




//     connect(button, &QPushButton::clicked, [this,event](){
//         ui->pushButton->setEnabled(0);
//         ui->addEv->setText("Update Event");
//         ui->canceEv->setEnabled(1);
//         ui->lineEdit->setText(event.name);

//         ui->plainTextEdit->setPlainText(event.description);


//         ui->comboBox->setCurrentText(event.type);

//         ui->locationEdit->setText(event.location);

//         ui->dateTimeEdit->setDateTime(event.dateTime);

//         ui->priority->setText(QString::number(event.priority));

//         int i = 0;

//         for (Event &e : events){
//             if(e.name == event.name && e.description == event.description){
//                 ui->eventId->setText(QString::number(i));
//                 return;
//             }
//             i++;
//         }

//     });

// }

void MainWindow::newEvent(Event& event){
    QWidget *w = new QWidget(this);
    w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // Create the label and button
    QLabel *label = new QLabel(event.name, this);
    QPushButton *button = new QPushButton("", this);  // Set button text
    button->setStyleSheet("border: 1px solid grey; border-radius: 5px;");

    // Set the stylesheet for the label
    label->setStyleSheet("font: 500 12pt \"Roboto\"; color: rgb(109, 109, 109);");

    // Set the size of the button
    button->setFixedSize(22, 22);

    // Create a horizontal layout
    QHBoxLayout *layout = new QHBoxLayout(w);
    layout->setContentsMargins(30,5,50,5);             // Set margin to 0
    layout->setSpacing(90);                            // Set layout spacing to 10

    // Add the label to the layout
    layout->addWidget(label);

    // Add a spacer between label and button to control the margin
    QSpacerItem *labelButtonSpacer = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum); // Adjust the value to your preference
    layout->addItem(labelButtonSpacer);

    // Add the button to the layout
    layout->addWidget(button);

    // Create a spacer and add it after the button
    QSpacerItem *spacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    layout->addItem(spacer);

    // Align the contents within the layout (left for label, right for button)
    layout->setAlignment(label, Qt::AlignLeft);

    // Set the layout for the widget
    setLayout(layout);

    ui->eventLayout->addWidget(w);

    connect(button, &QPushButton::clicked, [this,event](){
        ui->pushButton->setEnabled(0);
        ui->addEv->setText("Update Event");
        ui->canceEv->setEnabled(1);
        ui->lineEdit->setText(event.name);

        ui->plainTextEdit->setPlainText(event.description);

        ui->comboBox->setCurrentText(event.type);

        ui->locationEdit->setText(event.location);

        ui->dateTimeEdit->setDateTime(event.dateTime);

        ui->priority->setText(QString::number(event.priority));

        int i = 0;
        for (Event &e : events){
            if(e.name == event.name && e.description == event.description){
                ui->eventId->setText(QString::number(i));
                return;
            }
            i++;
        }
    });
}



void MainWindow::on_upcomingBtn_clicked()
{
    while (QLayoutItem *item = ui->eventLayout->takeAt(1))
    {
        // If the item is a widget, delete it
        if (QWidget *widget = item->widget())
        {
            widget->setParent(nullptr);
            delete widget;
        }
    }
    int i = 0;
    ui->Current->setText("Upcoming");
    for(Event& event : events){
        if(event.dateTime  >= QDateTime::currentDateTime()){
            i++ ;
            newEvent(event);
        }
    }

    ui->Current_C->setText(QString::number(i));

}


void MainWindow::on_todayBtn_clicked()
{
    while (QLayoutItem *item = ui->eventLayout->takeAt(1))
    {
        // If the item is a widget, delete it
        if (QWidget *widget = item->widget())
        {
            widget->setParent(nullptr);
            delete widget;
        }
    }
    int i = 0;
    ui->Current->setText("Today");
    for(Event& event : events){
        if(event.dateTime.date()  == QDateTime::currentDateTime().date()){
            i++ ;
            newEvent(event);
        }
    }

    ui->Current_C->setText(QString::number(i));

}


void MainWindow::on_persoBtn_clicked()
{
    while (QLayoutItem *item = ui->eventLayout->takeAt(1))
    {
        if (QWidget *widget = item->widget())
        {
            widget->setParent(nullptr);
            delete widget;
        }
    }
    int i = 0;
    ui->Current->setText("Personal");
    for(Event& event : events){
        if(event.type.toLower() == "personal"  ){

            i++ ;
            newEvent(event);
        }
    }

    ui->Current_C->setText(QString::number(i));

}


void MainWindow::on_meetBtn_clicked()
{
    while (QLayoutItem *item = ui->eventLayout->takeAt(1))
    {
        if (QWidget *widget = item->widget())
        {
            widget->setParent(nullptr);
            delete widget;
        }
    }
    int i = 0;
    ui->Current->setText("Meetings");
    for(Event& event : events){
        if(event.type.toLower() == "meetings" ){

            i++ ;
            newEvent(event);
        }
    }

    ui->Current_C->setText(QString::number(i));
}


void MainWindow::on_DeadBtn_clicked()
{
    while (QLayoutItem *item = ui->eventLayout->takeAt(1))
    {
        if (QWidget *widget = item->widget())
        {
            widget->setParent(nullptr);
            delete widget;
        }
    }
    int i = 0;
    ui->Current->setText("DeadLine");
    for(Event& event : events){
        if(event.type.toLower() == "deadline"  ){
            i++ ;
            newEvent(event);
        }
    }

    ui->Current_C->setText(QString::number(i));
}


void MainWindow::on_canceEv_clicked()
{

    int i = ui->eventId->text().toInt();

    on_todayBtn_clicked();
    events.removeAt(i);
    em->events = events;
    em->saveEvents();
    updateWidgets();
}




void MainWindow::on_Calendar_clicked()
{
    // Creating the dialog
    QDialog *dialog = new QDialog;
    dialog->setWindowTitle("Choose Date");

    // Creating the combo box
    QComboBox *comboBox = new QComboBox;
    comboBox->addItem("Day");
    comboBox->addItem("Week");
    comboBox->addItem("Month");

    // Creating the calendar widget
    QCalendarWidget *calendarWidget = new QCalendarWidget;
    calendarWidget->setStyleSheet("QCalendarWidget { background-color: #ffffff; }"); // Light theme

    // Creating the accept button
    QPushButton *acceptButton = new QPushButton("Accept");

    // Lambda function for button press
    QObject::connect(acceptButton, &QPushButton::clicked, [this,comboBox, calendarWidget,dialog]() {

        int selectedOption = comboBox->currentIndex();
        QDate selectedDate = calendarWidget->selectedDate();
        int i = 0;
        while (QLayoutItem *item = ui->eventLayout->takeAt(1))
        {
            // If the item is a widget, delete it
            if (QWidget *widget = item->widget())
            {
                widget->setParent(nullptr);
                delete widget;
            }
        }

        switch(selectedOption) {
        case 0:
            ui->Current->setText(selectedDate.toString());
            for (Event &event : events){
                if(event.dateTime.date() == selectedDate){
                    newEvent(event) ;
                    i++;

                }
            }
            ui->Current_C->setText(QString::number(i));
            break;
        case 1:
            int monthW;
            int yearW;
            int dayW;
            selectedDate.getDate(&yearW,&monthW,&dayW);
            ui->Current->setText("Week Number " + QString::number(selectedDate.weekNumber(&yearW)) + " Of " + QString::number(yearW));
            for (Event &event : events){
                if(event.dateTime.date() >=  selectedDate && event.dateTime.date() <= selectedDate.addDays(7)){
                    newEvent(event) ;
                    i++;
                }
            }
            ui->Current_C->setText(QString::number(i));
            break;
        case 2:
            int month;
            int year;
            int day;
            selectedDate.getDate(&year,&month,&day);

            ui->Current->setText("Month" + QString::number(month) + "/" + QString::number(year) );
            for (Event &event : events){
                if(event.dateTime.date().month() ==  selectedDate.month()){
                    newEvent(event) ;
                    i++;
                }
            }
            ui->Current_C->setText(QString::number(i));
            break;

        default:
            break;
        }

        dialog->close();
    });

    // Layout for the dialog
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(comboBox);
    mainLayout->addWidget(calendarWidget);

    // Layout for the button
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(acceptButton);
    mainLayout->addLayout(buttonLayout);

    dialog->setLayout(mainLayout);
    dialog->exec();
}


void MainWindow::on_Dark_toggled(bool checked)
{

}


void MainWindow::on_Dark_clicked()
{
    if(ui->test->isEnabled()){
        auto *effect = new InversionEffect();
        this->setGraphicsEffect(effect);
        ui->Dark->setText("Light");
        ui->test->setEnabled(0);
    }else {
        this->setGraphicsEffect(nullptr);
        ui->Dark->setText("Dark");
        ui->test->setEnabled(1);
    }
}




void MainWindow::on_Dark_2_clicked()
{
    if(ui->test_2->isEnabled()){
        ui->horizontalLayout->setStretch(1,7);
        ui->horizontalLayout->setStretch(2,10);
        ui->test_2->setEnabled(0);
        ui->Dark_2->setText("Minimize Task");
    }else {
        ui->horizontalLayout->setStretch(1,10);
        ui->horizontalLayout->setStretch(2,7);
        ui->test_2->setEnabled(1);
        ui->Dark_2->setText("Expand Task");
    }
}


void MainWindow::on_pushButton_3_clicked()
{
    while (QLayoutItem *item = ui->eventLayout->takeAt(1))
    {
        if (QWidget *widget = item->widget())
        {
            widget->setParent(nullptr);
            delete widget;
        }
    }
    int i = 0;
    ui->Current->setText("Search Result");
    for(Event& event : events){
        if(event.name.toLower().contains(ui->lineEdit_2->text().toLower()) || event.description.toLower().contains(ui->lineEdit_2->text().toLower())  ){
            i++ ;
            newEvent(event);
        }
    }

    ui->Current_C->setText(QString::number(i));
}


void MainWindow::on_pushButton_5_clicked()
{
    if(ui->Dark->isVisible()){
        ui->Dark->hide();
        ui->Dark_2->hide();
    }else {
        ui->Dark->show();
        ui->Dark_2->show();

    }
}

