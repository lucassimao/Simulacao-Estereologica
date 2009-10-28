#include <QMessageBox>
#include <QString>
#include "gui/MainWindow.h"

using namespace simulacao::gui;
using namespace std;

int main(int argc,char **argv){
	QApplication app(argc,argv);
	
	if (!QGLFormat::hasOpenGL()) {
		QMessageBox::critical(NULL,QString("Simulação"),QString("Problemas ao tentar usar OpenGL neste sistema"));
        exit(EXIT_FAILURE);
    }
	MainWindow win;
	win.setVisible(true);

    return app.exec();


}