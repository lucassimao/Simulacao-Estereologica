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
#include "../../model/interceptos/Intercepto.h"
#include "../../model/interceptos/Poligono.h"

using std::runtime_error;
using simulacao::model::Parametros;
using namespace simulacao::model::atores;
using namespace simulacao::model::interceptos;
using std::vector;
using std::map;
using std::list;


PrismaTriangular::PrismaTriangular(NxScene *cena,NxCCDSkeleton *ccds,MeshFactory *meshFactory):Ator(){
	this->altura = Parametros::getInstance()->getAlturaPrisma();
	this->base  = Parametros::getInstance()->getBasePrisma();

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

	float px = 9 - (rand() % 20);
	float py = 5 + (rand() % 20);
	float pz = 9 - (rand() % 20);

	actorDesc.globalPose.t  = NxVec3(px,py,pz);
	this->ator = cena->createActor(actorDesc);
	this->ator->userData =  (void *)this;


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
	NxReal verticeMaisAlto=_FPCLASS_NINF, verticeMaisBaixo=_FPCLASS_PINF;

	for(int i=0;i<nbVerts;++i){
		NxVec3 vertice = (pose.M * points[i] + pose.t);

		if ( verticeMaisAlto < vertice.y)
			verticeMaisAlto = vertice.y;
		else
			if (verticeMaisBaixo > vertice.y)
				verticeMaisBaixo = vertice.y;			
	}

	return (verticeMaisAlto >= planoGlobalPosition.y && verticeMaisBaixo <= planoGlobalPosition.y);

}



Intercepto* PrismaTriangular::getIntercepto(NxVec3 planoPos){


	Cor v = VERMELHO;

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

	return new Poligono(v,poligonoPontos);
}

inline vector<SegmentoDeReta> PrismaTriangular::getSegmentosDeRetaInterceptados(NxVec3 planoPos){
	NxVec3 *vertices = getPosicaoGlobalDosVertices();
	vector<SegmentoDeReta> segmentos;

	map<int,NxVec3> verticesAcimaDoPlanoDeCorte;
	map<int,NxVec3> verticesAbaixoDoPlanoDeCorte;

	for(int i=0;i<6;++i)
		if (vertices[i].y > planoPos.y)
			verticesAcimaDoPlanoDeCorte[i] = vertices[i];
		else
			verticesAbaixoDoPlanoDeCorte[i] = vertices[i];

	map<int,NxVec3>::const_iterator iteratorVerticesAcimaDoPlanoDeCorte = verticesAcimaDoPlanoDeCorte.begin();

	while(iteratorVerticesAcimaDoPlanoDeCorte!=verticesAcimaDoPlanoDeCorte.end()){

		int indice_vertice = iteratorVerticesAcimaDoPlanoDeCorte->first;
		NxVec3 vec = iteratorVerticesAcimaDoPlanoDeCorte->second;
		map<int,NxVec3>::const_iterator iterator;

		// ver arquivos prismaTriangularVertices.png na pasta docs
		// para entender a conexao entre os vertices
		switch(indice_vertice){
			case 0:
				iterator = verticesAbaixoDoPlanoDeCorte.find(2);
				if (iterator!=verticesAbaixoDoPlanoDeCorte.end()){
					Vetor v1(vec.x,vec.y,vec.z);
					Vetor v2(iterator->second.x,iterator->second.y,iterator->second.z);
					segmentos.push_back(SegmentoDeReta(v1,v2));
				}
				iterator = verticesAbaixoDoPlanoDeCorte.find(3);
				if (iterator!=verticesAbaixoDoPlanoDeCorte.end()){
					Vetor v1(vec.x,vec.y,vec.z);
					Vetor v2(iterator->second.x,iterator->second.y,iterator->second.z);
					segmentos.push_back(SegmentoDeReta(v1,v2));
				}
				iterator = verticesAbaixoDoPlanoDeCorte.find(4);
				if (iterator!=verticesAbaixoDoPlanoDeCorte.end()){
					Vetor v1(vec.x,vec.y,vec.z);
					Vetor v2(iterator->second.x,iterator->second.y,iterator->second.z);
					segmentos.push_back(SegmentoDeReta(v1,v2));
				}
				break;
			case 1:
				iterator = verticesAbaixoDoPlanoDeCorte.find(2);
				if (iterator!=verticesAbaixoDoPlanoDeCorte.end()){
					Vetor v1(vec.x,vec.y,vec.z);
					Vetor v2(iterator->second.x,iterator->second.y,iterator->second.z);
					segmentos.push_back(SegmentoDeReta(v1,v2));
				}
				iterator = verticesAbaixoDoPlanoDeCorte.find(4);
				if (iterator!=verticesAbaixoDoPlanoDeCorte.end()){
					Vetor v1(vec.x,vec.y,vec.z);
					Vetor v2(iterator->second.x,iterator->second.y,iterator->second.z);
					segmentos.push_back(SegmentoDeReta(v1,v2));
				}
				iterator = verticesAbaixoDoPlanoDeCorte.find(5);
				if (iterator!=verticesAbaixoDoPlanoDeCorte.end()){
					Vetor v1(vec.x,vec.y,vec.z);
					Vetor v2(iterator->second.x,iterator->second.y,iterator->second.z);
					segmentos.push_back(SegmentoDeReta(v1,v2));
				}
				break;
			case 2:
				iterator = verticesAbaixoDoPlanoDeCorte.find(1);
				if (iterator!=verticesAbaixoDoPlanoDeCorte.end()){
					Vetor v1(vec.x,vec.y,vec.z);
					Vetor v2(iterator->second.x,iterator->second.y,iterator->second.z);
					segmentos.push_back(SegmentoDeReta(v1,v2));
				}
				iterator = verticesAbaixoDoPlanoDeCorte.find(5);
				if (iterator!=verticesAbaixoDoPlanoDeCorte.end()){
					Vetor v1(vec.x,vec.y,vec.z);
					Vetor v2(iterator->second.x,iterator->second.y,iterator->second.z);
					segmentos.push_back(SegmentoDeReta(v1,v2));
				}
				iterator = verticesAbaixoDoPlanoDeCorte.find(0);
				if (iterator!=verticesAbaixoDoPlanoDeCorte.end()){
					Vetor v1(vec.x,vec.y,vec.z);
					Vetor v2(iterator->second.x,iterator->second.y,iterator->second.z);
					segmentos.push_back(SegmentoDeReta(v1,v2));
				}
				break;
			case 3:
				iterator = verticesAbaixoDoPlanoDeCorte.find(0);
				if (iterator!=verticesAbaixoDoPlanoDeCorte.end()){
					Vetor v1(vec.x,vec.y,vec.z);
					Vetor v2(iterator->second.x,iterator->second.y,iterator->second.z);
					segmentos.push_back(SegmentoDeReta(v1,v2));
				}
				iterator = verticesAbaixoDoPlanoDeCorte.find(4);
				if (iterator!=verticesAbaixoDoPlanoDeCorte.end()){
					Vetor v1(vec.x,vec.y,vec.z);
					Vetor v2(iterator->second.x,iterator->second.y,iterator->second.z);
					segmentos.push_back(SegmentoDeReta(v1,v2));
				}
				iterator = verticesAbaixoDoPlanoDeCorte.find(5);
				if (iterator!=verticesAbaixoDoPlanoDeCorte.end()){
					Vetor v1(vec.x,vec.y,vec.z);
					Vetor v2(iterator->second.x,iterator->second.y,iterator->second.z);
					segmentos.push_back(SegmentoDeReta(v1,v2));
				}
				break;
			case 4:
				iterator = verticesAbaixoDoPlanoDeCorte.find(0);
				if (iterator!=verticesAbaixoDoPlanoDeCorte.end()){
					Vetor v1(vec.x,vec.y,vec.z);
					Vetor v2(iterator->second.x,iterator->second.y,iterator->second.z);
					segmentos.push_back(SegmentoDeReta(v1,v2));
				}
				iterator = verticesAbaixoDoPlanoDeCorte.find(3);
				if (iterator!=verticesAbaixoDoPlanoDeCorte.end()){
					Vetor v1(vec.x,vec.y,vec.z);
					Vetor v2(iterator->second.x,iterator->second.y,iterator->second.z);
					segmentos.push_back(SegmentoDeReta(v1,v2));
				}
				iterator = verticesAbaixoDoPlanoDeCorte.find(1);
				if (iterator!=verticesAbaixoDoPlanoDeCorte.end()){
					Vetor v1(vec.x,vec.y,vec.z);
					Vetor v2(iterator->second.x,iterator->second.y,iterator->second.z);
					segmentos.push_back(SegmentoDeReta(v1,v2));
				}
				break;
			case 5:
				iterator = verticesAbaixoDoPlanoDeCorte.find(2);
				if (iterator!=verticesAbaixoDoPlanoDeCorte.end()){
					Vetor v1(vec.x,vec.y,vec.z);
					Vetor v2(iterator->second.x,iterator->second.y,iterator->second.z);
					segmentos.push_back(SegmentoDeReta(v1,v2));
				}
				iterator = verticesAbaixoDoPlanoDeCorte.find(1);
				if (iterator!=verticesAbaixoDoPlanoDeCorte.end()){
					Vetor v1(vec.x,vec.y,vec.z);
					Vetor v2(iterator->second.x,iterator->second.y,iterator->second.z);
					segmentos.push_back(SegmentoDeReta(v1,v2));
				}
				iterator = verticesAbaixoDoPlanoDeCorte.find(3);
				if (iterator!=verticesAbaixoDoPlanoDeCorte.end()){
					Vetor v1(vec.x,vec.y,vec.z);
					Vetor v2(iterator->second.x,iterator->second.y,iterator->second.z);
					segmentos.push_back(SegmentoDeReta(v1,v2));
				}
				break;
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

	NxU32 nbVerts = meshDesc.numVertices;	
	// ver arquivo docs\prismaTriangularVertices.png para
	// conhecer a ordem em que esses pontos estão definidos
	NxVec3* points = (NxVec3 *)meshDesc.points;

	// convertendo de posicionamento local p/ global
	for(register int i=0;i<6;++i)
		vertices[i] = (pose.M * points[i] + pose.t);
	
	return vertices;
}