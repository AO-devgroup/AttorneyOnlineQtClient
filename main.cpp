#include "main.h"

int main(int argc, char *argv[])
{

  QApplication a(argc, argv);
  Lobby w;
  //w.QuitOnLastWindowClosed(false);
  w.setTheme();
  w.connectMaster();
  w.show();

  return a.exec();
}
