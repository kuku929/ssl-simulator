#ifndef KURUK_H
#define KURUK_H

#include <QMainWindow>
#include "ui_kuruk.h"
#include "vyasa/vyasa.h"
#include "shunya/shunya.h"

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
signals:
    // void configureBots();
};

#endif // KURUK_H
