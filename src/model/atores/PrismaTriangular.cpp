#include <NxPhysics.h>
#include <QtDebug>
#include <stdexcept>
#include <cfloat>
#include <map>
#include <list>
#include <vector>
#include "PrismaTriangular.h"
#include "../Parametros.h"
#include "../../draw/cooking.h"
#include "../../utils/SegmentoDeReta.h"
#include "../../utils/Vetor.h"
#include "../../model/interceptos/InterceptoDeArea.h"
#include "../../model/interceptos/Poligono.h"
#include "../../utils/GlobalPoseFactory.h"

using std::runtime_error;
using simulacao::model::Parametros;
using namespace simulacao::model::atores;
using namespace simulacao::utils;
using namespace simulacao::model::interceptos;
using std::vector;
using std::map;
using std::list;
#define BETWEEN(x,y,z) ((x < y) && (y < z))


PrismaTriangular::PrismaTriangular(NxScene *cena,MeshFactory *meshFactory,Cor cor):Ator(){
	this->altura = Parametros::getInstance()->getAlturaPrisma();
	this->base  = Parametros::getInstance()->getBasePrisma();
	this->cor = cor;


	const double raiz_de_3 = sqrt(3.0);

	NxVec3 verts[6] = { 
		NxVec3(0.0,altura/2.0,base*raiz_de_3/2.0), 
		NxVec3(base/2.0,altura/2.0,0), 
		NxVec3(base/2.0,-altura/2.0,0), 
		NxVec3(-base/2.0,-altura/2.0,0), 
		NxVec3(-base/2.0,altura/2.0,0), 
		NxVec3(0,-altura/2.0,base *raiz_de_3/2.0)
	};


	NxConvexMeshDesc* convexDesc = new NxConvexMeshDesc();
	convexDesc->numVertices			= 6;
	convexDesc->pointStrideBytes	= sizeof(NxVec3);
	convexDesc->points				= verts;
	convexDesc->flags				= NX_CF_COMPUTE_CONVEX;

	NxConvexShapeDesc convexShapeDesc;
	convexShapeDesc.localPose.t = NxVec3(0,0,0);
	convexShapeDesc.userData = convexDesc;

	convexShapeDesc.meshData = meshFactory->criarConvexMesh(convexDesc);

	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;
	actorDesc.shapes.pushBack(&convexShapeDesc);

	actorDesc.body = &bodyDesc;
	actorDesc.density = 10.0;

	NxVec3 posicaoInicial = GlobalPoseFactory::getInstance()->newGlobalPosition();
	if (posicaoInicial.y + altura >= Parametros::getInstance()->getAlturaDoTopoDaCaixa()){
		posicaoInicial.y -= 2*altura;
	}

	if (posicaoInicial.x + base >= Parametros::getInstance()->getArestaDaCaixa()/2.0){
		posicaoInicial.x -= 2*base;
	}else
		if (posicaoInicial.x - base < - Parametros::getInstance()->getArestaDaCaixa()/2.0){
			posicaoInicial.x += 2*base;
		}

		actorDesc.globalPose.t  = posicaoInicial;
		this->ator = cena->createActor(actorDesc);
		this->ator->userData =  (void *)this;


		double Pi =  3.14159265;
		NxVec3 vetores[3];

		for(int i=0;i<3;++i){
			double random=rand()/(RAND_MAX +1.0); // gera um número entre [0;1]	
			double anguloA = Pi*random;
			double anguloB = 2*Pi*random;

			double radA = anguloA * Pi/180.0;
			double radB = anguloB * Pi/180.0;

			double x = sin(anguloA) * cos(anguloB);
			double y = sin(anguloA)*sin(anguloB);
			double z = cos(anguloA);

			vetores[i] = NxVec3(x,y,z);
		}

		this->ator->setGlobalOrientation(NxMat33(vetores[0],vetores[1],vetores[2]));

}
PrismaTriangular::~PrismaTriangular(){

}


bool PrismaTriangular::estaInterceptadoPeloPlano(NxVec3 planoGlobalPosition){
	NxShape *mesh =  this->ator->getShapes()[0];
	NxVec3 pos = ator->getGlobalPosition();

	NxMat34 pose = mesh->getGlobalPose();

	NxConvexMeshDesc meshDesc;
	mesh->isConvexMesh()->getConvexMesh().saveToDesc(meshDesc); 

	NxU32 nbVerts = meshDesc.numVertices;	
	NxVec3* points = (NxVec3 *)meshDesc.points;

	// alto e baixo em termos da coordenada Y
	NxReal verticeMaisAlto = (pose.M * points[0] + pose.t).y;
	NxReal verticeMaisBaixo = (pose.M * points[1] + pose.t).y;

	for(NxU32 i=1; i < nbVerts ; ++i){
		NxVec3 vertice = (pose.M * points[i] + pose.t);

		if ( verticeMaisAlto < vertice.y)
			verticeMaisAlto = vertice.y;
		else
			if (verticeMaisBaixo > vertice.y)
				verticeMaisBaixo = vertice.y;			
	}

	return (verticeMaisAlto > planoGlobalPosition.y && verticeMaisBaixo < planoGlobalPosition.y);

}



InterceptoDeArea* PrismaTriangular::getIntercepto(NxVec3 planoPos){

	vector<SegmentoDeReta> segmentosDeRetaInterceptados = getSegmentosDeRetaInterceptados(planoPos);
	vector<SegmentoDeReta>::const_iterator iterator = segmentosDeRetaInterceptados.begin();
	list<Ponto> poligonoPontos;

	while(iterator!=segmentosDeRetaInterceptados.end())
	{
		Ponto p = {0,0,0};
		SegmentoDeReta seg = *iterator;

		if (seg.interceptarComPlano(planoPos.y,&p)){
			poligonoPontos.push_back(p);
		}

		iterator++;
	}

	return new Poligono(this->cor,poligonoPontos,this->altura/this->base,0,this->base,getAtorID());
}

inline vector<SegmentoDeReta> PrismaTriangular::getSegmentosDeRetaInterceptados(NxVec3 planoPos){
	NxVec3 *vertices = getPosicaoGlobalDosVertices();
	vector<SegmentoDeReta> segmentos;

	vector<NxVec3> verticesAcimaDoPlanoDeCorte;
	vector<NxVec3> verticesAbaixoDoPlanoDeCorte;

	for(int i=0;i<6;++i)
	{
		if (vertices[i].y >= planoPos.y)
			verticesAcimaDoPlanoDeCorte.push_back(vertices[i]);
		else
			verticesAbaixoDoPlanoDeCorte.push_back(vertices[i]);
	}


	qDebug() << verticesAcimaDoPlanoDeCorte.size() << " " << verticesAbaixoDoPlanoDeCorte.size() << endl;

	vector<NxVec3>::const_iterator iteratorVerticesAcimaDoPlanoDeCorte = verticesAcimaDoPlanoDeCorte.begin();

	while(iteratorVerticesAcimaDoPlanoDeCorte!=verticesAcimaDoPlanoDeCorte.end()){

		NxVec3 vec = *iteratorVerticesAcimaDoPlanoDeCorte;
		Vetor v1(vec.x,vec.y,vec.z);

		vector<NxVec3>::const_iterator iteratorVerticesAbaixoDoPlanoDeCorte = verticesAbaixoDoPlanoDeCorte.begin();

		while(iteratorVerticesAbaixoDoPlanoDeCorte != verticesAbaixoDoPlanoDeCorte.end()){
			NxVec3 vec2 = *iteratorVerticesAbaixoDoPlanoDeCorte;
			Vetor v2(vec2.x,vec2.y,vec2.z);

			segmentos.push_back(SegmentoDeReta(v1,v2));
			++iteratorVerticesAbaixoDoPlanoDeCorte;
		}	

		++iteratorVerticesAcimaDoPlanoDeCorte;
	}

	return segmentos;
}

inline NxVec3* PrismaTriangular::getPosicaoGlobalDosVertices(){
	NxVec3 *vertices = (NxVec3 *) malloc(sizeof(NxVec3)*6);

	NxShape *mesh =  this->ator->getShapes()[0];
	NxMat34 pose = mesh->getGlobalPose();
	NxConvexMeshDesc meshDesc;
	mesh->isConvexMesh()->getConvexMesh().saveToDesc(meshDesc); 

	//NxU32 nbVerts = meshDesc.numVertices;	
	// ver arquivo docs\prismaTriangularVertices.png para
	// conhecer a ordem em que esses pontos estão definidos
	NxVec3* points = (NxVec3 *)meshDesc.points;

	// convertendo de posicionamento local p/ global
	for(register int i=0;i<6;++i)
		vertices[i] = (pose.M * points[i] + pose.t);

	return vertices;
}