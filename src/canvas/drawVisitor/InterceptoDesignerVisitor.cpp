#include <QDebug>
#include "..\..\model\interceptos\Disco.h"
using namespace simulacao::model::interceptos;

#include "InterceptoDesignerVisitor.h"
using namespace simulacao::canvas::drawVisitor;

InterceptoDesignerVisitor::InterceptoDesignerVisitor(){

}

void InterceptoDesignerVisitor::visit(Disco *disco){
	qDebug() << disco->getCentro().x << disco->getCentro().y << disco->getCentro().z << "\n";
}

void InterceptoDesignerVisitor::visit(Poligono *poligono){
	qDebug() << "Atencao mais um Poligono hein!\n";
}