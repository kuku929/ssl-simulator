#ifndef KURUK_H
#define KURUK_H

#include <QMainWindow>
#include "ui_kuruk.h"
#include "vyasa/vyasa.h"
#include "shunya/shunya.h"
#include "drona/drona.h"
#include "yodha/yodha.h"
#include "vishnu/vishnu.h"

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
    Vishnu *vishnu;
    std::shared_ptr<std::vector<BlueBot>> pandav;
    std::shared_ptr<std::vector<YellowBot>> kaurav;
    std::shared_ptr<Ball> ball;
signals:
    // void configureBots();
};

#endif // KURUK_H
