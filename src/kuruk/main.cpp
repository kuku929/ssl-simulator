#include "kuruk.h"
#include <QApplication>

int main(int argc, char **argv){
    QApplication a(argc, argv);
    Kuruk shetra;
    shetra.show();
    qDebug() << "executing...";
    return a.exec();
}
