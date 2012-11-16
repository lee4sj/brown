#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "common.h"
#include "particleemitter.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->widget, SIGNAL(_glInit()), this, SLOT(initializeGL_Done()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::on_velocityDial_dialMoved(int value)
{
    float3 &v = ui->widget->emitter()->velocity();
    float magnitude = v.getMagnitude();
    v.x = magnitude * cosf((270 - value) * PI / 180.0f);
    v.y = magnitude * sinf((270 - value) * PI / 180.0f);
    ui->velocityLabel1->setText(QString("Direction: ") + QString::number(ui->velocityDial->value() % 360));
}

void MainWindow::on_velocitySlider_valueChanged(int value)
{
    float3 &v = ui->widget->emitter()->velocity();
    v = v.getNormalized() * value;
    ui->velocityLabel2->setText(QString("Magnitude: ") + QString::number(ui->velocitySlider->value()));
}

void MainWindow::on_forceDial_valueChanged(int value)
{
    float3 &f = ui->widget->emitter()->force();
    float magnitude = f.getMagnitude();
    f.x = magnitude * cosf((270 - value) * PI / 180.0f);
    f.y = magnitude * sinf((270 - value) * PI / 180.0f);
    ui->forceLabel1->setText(QString("Direction: ") + QString::number(ui->forceDial->value() % 360));
}

void MainWindow::on_forceSlider_valueChanged(int value)
{
    float3 &f = ui->widget->emitter()->force();
    f = f.getNormalized() * value / 100.0f;
    ui->forceLabel2->setText(QString("Magnitude: ") + QString::number(ui->forceSlider->value() / 100.0f));
}

void MainWindow::on_speedSlider_valueChanged(int value)
{
    ui->widget->emitter()->speed() = value * 0.00005f;
    ui->speedLabel->setText(QString("Magnitude: ") + QString::number(ui->speedSlider->value() / 100.0f));
}

void MainWindow::initializeGL_Done()
{
    float r = ui->widget->emitter()->velocity().getMagnitude();
    ui->velocitySlider->setValue(r);
    ui->velocityDial->setValue(270.0f - acosf(ui->widget->emitter()->velocity().x / r) * 180.0f / PI);
    r = ui->widget->emitter()->force().getMagnitude();
    ui->forceSlider->setValue(r);
    ui->forceDial->setValue(270.0f - acosf(ui->widget->emitter()->force().x / r) * 180.0f / PI);
    ui->speedSlider->setValue(ui->widget->emitter()->speed() * 10000.0f);

    ui->velocityLabel1->setText(QString("Direction: ") + QString::number(ui->velocityDial->value() % 360));
    ui->velocityLabel2->setText(QString("Magnitude: ") + QString::number(ui->velocitySlider->value()));
    ui->forceLabel1->setText(QString("Direction: ") + QString::number(ui->forceDial->value() % 360));
    ui->forceLabel2->setText(QString("Magnitude: ") + QString::number(ui->forceSlider->value() / 100.0f));
    ui->speedLabel->setText(QString("Magnitude: ") + QString::number(ui->speedSlider->value() / 100.0f));
}
