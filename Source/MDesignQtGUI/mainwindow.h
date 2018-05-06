#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void readDataFromArduino();

private:
    // current coordinates from arduino
    float x;
    float y;

    // maximum coordinates from arduino
    const float x_max_robuino = 3000;
    const float y_max_robuino = 2000;

    // maximum coordinates scaled to size of gamefield-image
    float x_max;
    float y_max;

    Ui::MainWindow *ui;
    QSerialPort* arduino;
    static const quint16 arduino_uno_vendor_id = 9025;
    static const quint16 arduino_uno_product_id = 67;
    QString arduino_port_name;
    bool arduino_is_available;
    QByteArray serialData;
    QString serialBuffer;
    // gamefield has to be a pointer, so paintEvent() can use it
    QPixmap* gamefield;

protected:
   void paintEvent(QPaintEvent* e);
};

#endif // MAINWINDOW_H
