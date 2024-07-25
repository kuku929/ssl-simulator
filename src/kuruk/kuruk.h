#ifndef KURUK_H
#define KURUK_H

#include <QMainWindow>
#include "ui_kuruk.h"
#include "vyasa/vyasa.h"
#include "shunya/shunya.h"
#include "drona/drona.h"

class Kuruk : public QMainWindow
{
    Q_OBJECT
public:
    explicit Kuruk(QWidget *parent = nullptr);
    ~Kuruk();

private:
    Ui::kuruk *ui;
    Vyasa *vyasa;
    Shunya *shunya;
    Drona *drona;
signals:
    // void configureBots();
};

#endif // KURUK_H
