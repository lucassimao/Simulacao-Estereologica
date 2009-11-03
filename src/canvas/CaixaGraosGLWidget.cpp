#include "CaixaGraosGLWidget.h"

#include <QMouseEvent>
#include <QTimer>
#include <QMessageBox>
#include <cstdlib>
#include <ctime>
#include <QtDebug>
#include "../draw/cooking.h"
#include "../draw/Stream.h"

using namespace simulacao::canvas;
using namespace simulacao::model;

NxVec3 CaixaGraosGLWidget::gCameraForward(0,0,1);
NxVec3 CaixaGraosGLWidget::gCameraRight(-1,0,0);
const NxReal CaixaGraosGLWidget::gCameraSpeed = 250;
int mx, my;
//NxCCDSkeleton * ccds;


CaixaGraosGLWidget::CaixaGraosGLWidget(QWidget *parent,SimulacaoCaixa *simulacao)
: QGLWidget(parent),gCameraPos(NxVec3(0,20,-50.0))
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

	gCameraAspectRatio = 1;	
}




void CaixaGraosGLWidget::initializeGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glEnable(GL_LIGHTING);
	float AmbientColor[]	=  { 0.0f, 0.0f, 0.0f, 0.1f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, AmbientColor);

	float DiffuseColor[]	=  {1.0f, 1.0f, 1.0f, 1.0f };		
	glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseColor);

	float SpecularColor[]	= { 0.0f, 0.0f, 0.0f, 1.0f };		
	glLightfv(GL_LIGHT0, GL_SPECULAR, SpecularColor);

	float Position[]		= {80.0f,80.0f, -70.0f,1.0f };	
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

void CaixaGraosGLWidget::draw()
{

	SetupCamera();

	if (*simulacao)
	{
		simulacao->GetPhysicsResults();
		simulacao->iniciarSimulacao();
	}
	RenderActors();

}


void CaixaGraosGLWidget::RenderActors(){

	NxU32 nbActors = simulacao->getCena()->getNbActors();
	NxActor** actors = simulacao->getCena()->getActors();

	while (nbActors--)
	{
		NxActor* actor = *actors++;

		{
			DrawActor(actor, NULL, false);
		}

	}
	glColor4f(0.0f, 0.0f, 0.4f, 1.0f);
	drawCuboid(box);

}

void CaixaGraosGLWidget::drawCuboid(const NxActor * cuboid, NX_BOOL drawBothSides)
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
	gluPerspective(45.0f, gCameraAspectRatio, 1.0f, 10000.0f);//angulo de visão da caixa
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

void CaixaGraosGLWidget::keyPressEvent ( QKeyEvent * event ){

	double deltaTime = 1.0/60.0;
	switch (event->key())
	{
		// Camera controls
	case Qt::Key_W :{ gCameraPos += gCameraForward*gCameraSpeed*deltaTime; break; }
	case Qt::Key_S :{ gCameraPos -= gCameraForward*gCameraSpeed*deltaTime; break; }
	case Qt::Key_A :{ gCameraPos -= gCameraRight*gCameraSpeed*deltaTime; break; }
	case Qt::Key_D :{ gCameraPos += gCameraRight*gCameraSpeed*deltaTime; break; }
	case Qt::Key_Z :{ gCameraPos -= NxVec3(0,1,0)*gCameraSpeed*deltaTime; break; }
	case Qt::Key_Q :{ gCameraPos += NxVec3(0,1,0)*gCameraSpeed*deltaTime; break; }
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


/*

void CaixaGraosGLWidget::CreateMeshes(){
	NxVec3 dim(10,10,10);// vetor que determina o tamanho da caixa
	NxVec3 dim1(0.5f, 0.5f , 0.5f);

	NxU32 triangles[3 * 12] = { 
		0,1,3,
		0,3,2,
		3,7,6,
		3,6,2,
		1,5,7,
		1,7,3,
		4,6,7,
		4,7,5,
		1,0,4,
		5,1,4,
		4,0,2,
		4,2,6
	};
	NxVec3 points[8];


	//static mesh
	points[0].set( dim.x, -dim.y, -dim.z);
	points[1].set( dim.x, -dim.y,  dim.z);
	points[2].set( dim.x,  dim.y, -dim.z);
	points[3].set( dim.x,  dim.y,  dim.z);

	points[4].set(-dim.x, -dim.y, -dim.z);
	points[5].set(-dim.x, -dim.y,  dim.z);
	points[6].set(-dim.x,  dim.y, -dim.z);
	points[7].set(-dim.x,  dim.y,  dim.z);

	bool status = InitCooking();
	if (!status) {
		printf("\nError: Unable to initialize the cooking library. Please make sure that you have correctly installed the latest version of the AGEIA PhysX SDK.\n\n");
		exit(1);
	}

	NxTriangleMeshDesc		meshDesc;
	meshDesc.numVertices = 8;
	meshDesc.numTriangles = 12;
	meshDesc.pointStrideBytes = sizeof(NxVec3);
	meshDesc.triangleStrideBytes = 3 * sizeof(NxU32);
	meshDesc.points = points;
	meshDesc.triangles = triangles;
	MemoryWriteBuffer buf;
	status = CookTriangleMesh(meshDesc, buf);
	if (!status) {
		printf("Unable to cook a triangle mesh.");
		exit(1);
	}
	MemoryReadBuffer readBuffer(buf.data);
	staticTriangleMesh = simulacao->getSDK()->createTriangleMesh(readBuffer);

	dim = dim1;
	points[0].set( dim.x, -dim.y, -dim.z);
	points[1].set( dim.x, -dim.y,  dim.z);
	points[2].set( dim.x,  dim.y, -dim.z);
	points[3].set( dim.x,  dim.y,  dim.z);

	points[4].set(-dim.x, -dim.y, -dim.z);
	points[5].set(-dim.x, -dim.y,  dim.z);
	points[6].set(-dim.x,  dim.y, -dim.z);
	points[7].set(-dim.x,  dim.y,  dim.z);

	NxConvexMeshDesc convexDesc;
	convexDesc.numVertices = 8;
	convexDesc.numTriangles = 0;
	convexDesc.pointStrideBytes = sizeof(NxVec3);
	convexDesc.points = points;
	convexDesc.flags |= NX_CF_COMPUTE_CONVEX;

	buf.clear();
	status = CookConvexMesh(convexDesc, buf);
	if (!status) {
		printf("Unable to cook a convex mesh.");
		exit(1);
	}
	{
		MemoryReadBuffer readBuffer(buf.data);
		simulacao->getSDK()->createConvexMesh(readBuffer);
	}

	NxSimpleTriangleMesh stm;
	stm.numVertices = 8;
	stm.numTriangles = 12;
	stm.pointStrideBytes = sizeof(NxVec3);
	stm.triangleStrideBytes = sizeof(NxU32)*3;

	for (NxU32 i = 0; i < 8; i++)
		points[i].arrayMultiply(points[i], NxVec3(0.8f, 0.8f, 0.9f));

	stm.points = points;
	stm.triangles = triangles;
	stm.flags |= NX_MF_FLIPNORMALS;
	ccds = simulacao->getSDK()->createCCDSkeleton(stm);

	CloseCooking();
}


NxActor * CaixaGraosGLWidget::criarCaixa(){

	NxBodyDesc bodyDesc;
	CreateMeshes();
	NxActorDesc actorDesc;
	actorDesc.body = &bodyDesc;
	actorDesc.density = 100.0f;
	actorDesc.globalPose.t = NxVec3(0,15,0);// Define a altura dos objetos no eixo y
	NxTriangleMeshShapeDesc meshShapeDesc;

	meshShapeDesc.meshData = staticTriangleMesh;
	actorDesc.shapes.pushBack(&meshShapeDesc);
	return  simulacao->getCena()->createActor(actorDesc);


}*/