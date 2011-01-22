#include <QMessageBox>
#include <QString>
#include <QDebug>
#include <ctime>
#include <cstdio>
#include "gui/MainWindow.h"

using namespace simulacao::gui;
using namespace std;


	
int main(int argc,char **argv){
	srand(time(0));
	QApplication app(argc,argv);
	
	if (!QGLFormat::hasOpenGL()) {
		QMessageBox::critical(NULL,QString("Simulação"),QString("Problemas ao tentar usar OpenGL neste sistema"));
        exit(EXIT_FAILURE);
    }
 	MainWindow win;
	win.setVisible(true);

    return app.exec();
}
