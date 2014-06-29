#include "dialog.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    //Forcing QApplication to always use raster rendering system -> Great performance boost (inconclusive)!
    int extargc = argc + 2;
    char** extargv = new char*[argc + 2];

    for(int i = 0; i < argc; i++)
    {
        extargv[i] = argv[i];
    }

    extargv[argc] = const_cast<char*>("-graphicssystem");
    extargv[argc + 1] = const_cast<char*>("raster");
//    extargv[argc + 1] = const_cast<char*>("opengl");

//    QApplication a(argc, argv);
    QApplication a(extargc, extargv);
    Dialog w;
    w.show();

    return a.exec();
}
