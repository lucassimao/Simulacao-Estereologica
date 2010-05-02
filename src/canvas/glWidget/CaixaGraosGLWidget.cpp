#include "CaixaGraosGLWidget.h"

#include <QMouseEvent>
#include <QTimer>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QFileDialog>
#include <QPixmap>
#include <QString>
#include <cstdlib>
#include <ctime>
#include <QtDebug>
#include "RenderizarAtoresStrategy.h"
#include "RenderizarInterceptosStrategy.h"
#include "..\..\draw\cooking.h"
#include "..\..\draw\Stream.h"
#include "..\..\defs.h"


using namespace simulacao::canvas::glWidget;
using namespace simulacao::model;

const NxReal CaixaGraosGLWidget::gCameraSpeed = 250;
int mx, my;

CaixaGraosGLWidget::CaixaGraosGLWidget(QWidget *parent,SimulacaoCaixa *simulacao)
: QGLWidget(parent),gCameraPos(NxVec3(0.65125149,29.323162,48.303276)),
	gCameraForward(NxVec3(-0.011699641,-0.28244135,-0.95921326)),gCameraAspectRatio(1),
	gCameraRight(NxVec3(0.95747489,0,-0.011678438))
{
	srand(time(0));
	setFormat(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer));
	setAutoBufferSwap(true);

	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(updateGL()));
	timer->start(0);

	this->simulacao = simulacao;
	box = this->simulacao->getCaixa();
	this->simulacao->novoPlanoDeCorte();
	this->mudancaDeEstrategiaDeVisualizacaoHabilitada = false;
}

void CaixaGraosGLWidget::draw()
{

	SetupCamera();

	if (*simulacao)
	{
		simulacao->GetPhysicsResults();
		simulacao->iniciarSimulacao();
	}

	this->renderizacaoStrategy->draw(this->simulacao);
	glColor4f(0.0f, 0.0f, 0.4f, 1.0f);
	drawCuboid(box);

}




void CaixaGraosGLWidget::initializeGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glEnable(GL_LIGHTING);
	float AmbientColor[]	=  { 0.0f, 0.0f, 0.0f, 0.1f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, AmbientColor);

	float DiffuseColor[]	=  {1.0f, 1.0f, 1.0f, 1.0f };		
	glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseColor);

	float SpecularColor[]	= { 0.0f, 0.0f, 0.0f, 1.0f };		
	glLightfv(GL_LIGHT0, GL_SPECULAR, SpecularColor);

	float Position[]		= {80.0f,80.0f, 70.0f,1.0f };	
	glLightfv(GL_LIGHT0, GL_POSITION, Position);

	glEnable(GL_LIGHT0);

	this->simulacao->iniciarSimulacao();
}

void CaixaGraosGLWidget::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GLfloat x = GLfloat(width) / height;
	glFrustum(-x, +x, -1.0, +1.0, 4.0, 15.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gCameraAspectRatio = float(width)/float(height);
}


void CaixaGraosGLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	draw();
}


void CaixaGraosGLWidget::posicionarCameraNoTopoDaCaixa(){
	gCameraPos.x = 0.066;
	gCameraPos.y = 44.368;
	gCameraPos.z = 0.342;

	gCameraRight.x = 0.0355;
	gCameraRight.y = 0;
	gCameraRight.z = -0.00024;

	gCameraForward.x = -3.0782681e-005;
	gCameraForward.y = -0.999;
	gCameraForward.z = -0.0355;

}

void CaixaGraosGLWidget::posicionarCameraNoPontoInicial(){
	gCameraPos.x = 0.65125149;
	gCameraPos.y = 29.323162;
	gCameraPos.z = 48.303276;

	gCameraRight.x = 0.95747489;
	gCameraRight.y = 0;
	gCameraRight.z = -0.011678438;

	gCameraForward.x = -0.011699641;
	gCameraForward.y = -0.28244135;
	gCameraForward.z = -0.95921326;
}





inline void CaixaGraosGLWidget::drawCuboid(const NxActor * cuboid, NX_BOOL drawBothSides)
{
	NxU32 numTrigs;
	const NxTriangle32* trigs;
	const NxPoint* verts;
	const NxPoint* normals;

	if(cuboid->getShapes()[0]->getType()== NX_SHAPE_MESH)
	{
		NxTriangleMesh & tm = static_cast<NxTriangleMeshShape*>(cuboid->getShapes()[0])->getTriangleMesh();

		numTrigs = tm.getCount(0, NX_ARRAY_TRIANGLES);
		trigs = (const NxTriangle32*)tm.getBase(0, NX_ARRAY_TRIANGLES);
		verts = (const NxPoint*)tm.getBase(0, NX_ARRAY_VERTICES);
		normals = (const NxPoint*)tm.getBase(0, NX_ARRAY_NORMALS);
	}
	else if(cuboid->getShapes()[0]->getType() == NX_SHAPE_CONVEX)
	{
		NxConvexMesh & tm = static_cast<NxConvexShape*>(cuboid->getShapes()[0])->getConvexMesh();

		numTrigs = tm.getCount(0, NX_ARRAY_TRIANGLES);
		trigs = (const NxTriangle32*)tm.getBase(0, NX_ARRAY_TRIANGLES);
		verts = (const NxPoint*)tm.getBase(0, NX_ARRAY_VERTICES);
		normals = (const NxPoint*)tm.getBase(0, NX_ARRAY_NORMALS);
	}
	else return;

	float* pVertList = new float[numTrigs*3*3];
	float* pNormList = new float[numTrigs*3*3];

	int vertIndex = 0;
	int normIndex = 0;
	for(unsigned int i=0;i<numTrigs;i++)
	{
		for(int j=0;j<3;j++)
		{
			pVertList[vertIndex++] = verts[trigs[i].v[j]].x;
			pVertList[vertIndex++] = verts[trigs[i].v[j]].y+15;
			pVertList[vertIndex++] = verts[trigs[i].v[j]].z;

			pNormList[normIndex++] = normals[trigs[i].v[j]].x;
			pNormList[normIndex++] = normals[trigs[i].v[j]].y+15;
			pNormList[normIndex++] = normals[trigs[i].v[j]].z;
		}
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3,GL_FLOAT, 0, pVertList);
	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, 0, pNormList);
	glDrawArrays(GL_TRIANGLES, 0, numTrigs*3);


	if(drawBothSides)
	{	
		glCullFace(GL_FRONT);
		glVertexPointer(3,GL_FLOAT, 0, pVertList);
		glNormalPointer(GL_FLOAT, 0, pNormList);
		glDrawArrays(GL_TRIANGLES, 0, numTrigs*3);
		glCullFace(GL_BACK);
	}

	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	delete[] pVertList;
	delete[] pNormList;
}

void CaixaGraosGLWidget::SetupCamera(){

	// Setup camera

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0f, gCameraAspectRatio, 10.0f, 100.0f);//angulo de visão da caixa
	gluLookAt(gCameraPos.x,gCameraPos.y,gCameraPos.z,gCameraPos.x + gCameraForward.x, 
		gCameraPos.y + gCameraForward.y, gCameraPos.z + gCameraForward.z, 0.0f,1.0f, 0.0f);

	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


}

void CaixaGraosGLWidget::mousePressEvent(QMouseEvent *event)
{
	mx = event->x();
	my = event->y();
}
/*
Paulo Freire
Leonardo Boff
Rubem Alves
*/
void CaixaGraosGLWidget::mudarEstrategiaDeVisualizacao(){
	switch (this->renderizacaoStrategy->getTypeOf()){
	case RenderizarInterceptos:
		setStrategy(new RenderizarAtoresStrategy());
		break;
	case RenderizarAtores:
		setStrategy(new RenderizarInterceptosStrategy(this->simulacao->getGrade()));
		break;
	}
}
void CaixaGraosGLWidget::salvarImagem(){
	QString filename = QFileDialog::getSaveFileName( this, "Save File", getenv( "HOME" ), "JPEG Image (*.jpg *.jpeg)");
   
	if( !filename.endsWith( ".jpg" ) && !filename.endsWith( ".jpeg" ) )
   {
      filename.append( ".jpg" );
   }

   //QImage image = mpPlot->grabFrameBuffer( );
   //QPixmap image = mpPlot->renderPixmap( );
   QPixmap image = QPixmap::grabWidget(this);
   if(image.save( filename, "JPG" ) )
      QMessageBox::warning( this, "Informação", "Imagem salva com sucesso!" );
   else
        QMessageBox::warning( this, "Erro", "Erro ao salvar a imagem!" );


}
void CaixaGraosGLWidget::mouseReleaseEvent ( QMouseEvent * event )
{
    if(event->button() == Qt::RightButton)
    {
        QMenu menu;

        QAction* openAct = new QAction("Salvar como imagem ....", this);
	    connect(openAct, SIGNAL(triggered()), this, SLOT(salvarImagem()));
		menu.addAction(openAct);

		if (mudancaDeEstrategiaDeVisualizacaoHabilitada){
			QAction* mudarStrategiaAct = new QAction("Mudar estratégia de visualização ....", this);
			connect(mudarStrategiaAct, SIGNAL(triggered()), this, SLOT(mudarEstrategiaDeVisualizacao()));	        
			menu.addAction(mudarStrategiaAct);
		}
		menu.addSeparator();
        menu.exec(mapToGlobal(event->pos()));
    }
}

void CaixaGraosGLWidget::habilitarMudancaDeEstrategiaDeVisualizacao(bool b){
	this->mudancaDeEstrategiaDeVisualizacaoHabilitada = b;
}

void CaixaGraosGLWidget::keyPressEvent ( QKeyEvent * event ){

	double deltaTime = 1.0/60.0;
	switch (event->key())
	{

	case Qt::Key_W :{ gCameraPos += gCameraForward*gCameraSpeed*deltaTime; break; }
	case Qt::Key_S :{ gCameraPos -= gCameraForward*gCameraSpeed*deltaTime; break; }
	case Qt::Key_A :{ gCameraPos -= gCameraRight*gCameraSpeed*deltaTime; break; }
	case Qt::Key_D :{ gCameraPos += gCameraRight*gCameraSpeed*deltaTime; break; }
	case Qt::Key_Z :{ gCameraPos -= NxVec3(0,0.4,0)*gCameraSpeed*deltaTime; break; }
	case Qt::Key_Q :{ gCameraPos += NxVec3(0,0.4,0)*gCameraSpeed*deltaTime; break; }
	}
	updateGL();

}

void CaixaGraosGLWidget::mouseMoveEvent(QMouseEvent *event)
{
	int dx = mx - event->x();
	int dy = my - event->y();

	gCameraForward.normalize();
	gCameraRight.cross(gCameraForward,NxVec3(0,1,0));

	NxQuat qx(NxPiF32 * dx * 20 / 180.0f, NxVec3(0,1,0));
	qx.rotate(gCameraForward);
	NxQuat qy(NxPiF32 * dy * 20 / 180.0f, gCameraRight);
	qy.rotate(gCameraForward);

	mx = event->x();
	my = event->y();
}