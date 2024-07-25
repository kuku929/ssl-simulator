#include "kuruk.h"
#include "ui_kuruk.h"
// #include "vyasa/vyasa.h"
// #include "drona/drona.h"
#include "vyasa/vyasa.h"
#include "drona/drona.h"

Kuruk::Kuruk(QWidget *parent)
    : QMainWindow{parent},
    ui(new Ui::kuruk),
    vyasa(new Vyasa(this)),
    shunya(new Shunya(this)),
    drona(new Drona(this))
{
    ui->setupUi(this);
    connect(vyasa, &Vyasa::recievedState, ui->kshetra, &Kshetra::handleState);
    // connect(this, &Kuruk::configureBots, shunya, &Shunya::setup);
    // emit configureBots();

    shunya->setup();
    drona->run();
}


Kuruk::~Kuruk(){
    delete ui;
    delete vyasa;
    delete shunya;
    ui = nullptr;
    vyasa = nullptr;
    shunya = nullptr;
}
