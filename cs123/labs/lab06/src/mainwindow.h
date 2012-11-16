#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;

private slots:
    void on_speedSlider_valueChanged(int value);
    void on_forceSlider_valueChanged(int value);
    void on_forceDial_valueChanged(int value);
    void on_velocitySlider_valueChanged(int value);
    void on_velocityDial_dialMoved(int value);
    void initializeGL_Done();
};

#endif // MAINWINDOW_H
