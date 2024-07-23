#include "kuruk.h"
#include "ui_kuruk.h"
#include "vyasa/vyasa.h"

Kuruk::Kuruk(QWidget *parent)
    : QMainWindow{parent},
    ui(new Ui::kuruk),
    vyasa(new Vyasa(this)),
    shunya(new Shunya(this))
{
    ui->setupUi(this);
    connect(vyasa, &Vyasa::recievedState, ui->kshetra, &Kshetra::handleState);
    // connect(this, &Kuruk::configureBots, shunya, &Shunya::setup);
    // emit configureBots();
    shunya->setup();
}


Kuruk::~Kuruk(){
    delete ui;
    delete vyasa;
    delete shunya;
    ui = nullptr;
    vyasa = nullptr;
    shunya = nullptr;
}
