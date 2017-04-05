#include <QApplication>
#include <QPushButton>
#include "ram.h"

int main(int argc, char **argv)
{
    RAM ramtest(823,1345);
    ramtest.RAM_info();
/*
 QApplication app (argc, argv);

 QPushButton button ("Hello world !");
 button.show();

 return app.exec();
 */
}
