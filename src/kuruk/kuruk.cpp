#include "kuruk.h"
#include "ui_kuruk.h"

Kuruk::Kuruk(QWidget *parent)
    : QMainWindow{parent},
    ui(new Ui::kuruk),
    vyasa(new Vyasa(this)),
    shunya(new Shunya(this)),
    drona(new Drona(this))
{
    ui->setupUi(this);
    connect(vyasa, &Vyasa::recievedState, ui->kshetra, &Kshetra::handleState);
    connect(vyasa, &Vyasa::recievedState, drona, &Drona::handleState);
    connect(ui->actionreset, &QAction::triggered, shunya, &Shunya::setup);
    // shunya->setup();
}


Kuruk::~Kuruk(){
    delete ui;
    delete vyasa;
    delete shunya;
    delete drona;
    ui = nullptr;
    vyasa = nullptr;
    shunya = nullptr;
    drona = nullptr;
}
