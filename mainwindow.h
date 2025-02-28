#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <event.h>
#include <eventmanager.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void newEvent(Event& event);
    void updateWidgets();


private slots:
    void on_pushButton_7_clicked();

    void on_addEv_clicked();

    void on_upcomingBtn_clicked();

    void on_todayBtn_clicked();

    void on_persoBtn_clicked();

    void on_meetBtn_clicked();

    void on_DeadBtn_clicked();

    void on_canceEv_clicked();

    void on_Calendar_clicked();

    void on_Dark_toggled(bool checked);

    void on_Dark_clicked();

    void on_Dark_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::MainWindow *ui;
    EventManager *em;
    QVector<Event> events;
    QPalette originalPalette; // Store the original palette
    bool isInverted = false;  // Track the state of inversion

};
#endif // MAINWINDOW_H
