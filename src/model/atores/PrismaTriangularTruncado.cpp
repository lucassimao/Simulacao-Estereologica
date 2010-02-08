#include <NxPhysics.h>
#include <QtDebug>
#include <stdexcept>
#include <cfloat>
#include <map>
#include <list>
#include <vector>
#include "PrismaTriangularTruncado.h"
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


PrismaTriangularTruncado::PrismaTriangularTruncado(NxScene *cena,NxCCDSkeleton *ccds,MeshFactory *meshFactory):Ator(){
	this->razaoDeAspecto = Parametros::getInstance()->getRazaoAspectoDoPrismaTriangularTruncado();
	this->razaoDeTruncamento  = Parametros::getInstance()->getRazaoDeTruncamentoDoPrismaTriangularTruncado();
	this->L0  = Parametros::getInstance()->getL0DoPrismaTriangularTruncado();
	
	double L1  = this->L0 * razaoDeTruncamento;
	double L2 = this->L0 - 2*L1;
	double alturaDoPrisma = this->razaoDeAspecto*this->L0;
	double alturaDoTrianguloDaBase = sqrt(pow(L0,2.0) - pow(L0/2.0,2.0) );
	double alturaDoTrianguloDosCantos = sqrt(pow(L1,2.0) - pow(L1/2.0,2.0) );
	double a1 = alturaDoTrianguloDosCantos;
	double a2 = alturaDoTrianguloDaBase - a1;

	NxVec3 verts[12] = { 
		NxVec3(L2/2.0,0,0), 
		NxVec3(-L2/2.0,0,0), 
		NxVec3(-L2/2.0 -L1/2.0 ,0,-alturaDoTrianguloDosCantos), 
		NxVec3(-L1/2.0,0,-a2),
		NxVec3(L1/2.0,0,-a2),
		NxVec3(L2/2.0 + L1/2.0 ,0,-alturaDoTrianguloDosCantos), 

		NxVec3(L2/2.0,alturaDoPrisma,0), 
		NxVec3(-L2/2.0,alturaDoPrisma,0), 
		NxVec3(-L2/2.0 -L1/2.0 ,alturaDoPrisma,-alturaDoTrianguloDosCantos), 
		NxVec3(-L1/2.0,alturaDoPrisma,-a2),
		NxVec3(L1/2.0,alturaDoPrisma,-a2),
		NxVec3(L2/2.0 + L1/2.0 ,alturaDoPrisma,-alturaDoTrianguloDosCantos)

	};


	NxConvexMeshDesc* convexDesc = new NxConvexMeshDesc();
	convexDesc->numVertices			= 12;
	convexDesc->pointStrideBytes	= sizeof(NxVec3);
	convexDesc->points				= verts;
	convexDesc->flags				= NX_CF_COMPUTE_CONVEX;

	NxConvexShapeDesc convexShapeDesc;
	convexShapeDesc.localPose.t = NxVec3(0,alturaDoPrisma/2.0,0);
	convexShapeDesc.userData = convexDesc;

	convexShapeDesc.meshData = meshFactory->criarConvexMesh(convexDesc);

	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;
	actorDesc.shapes.pushBack(&convexShapeDesc);

	actorDesc.body = &bodyDesc;
	actorDesc.density = 10.0;

	float a = rand(); float px = 10*(a/32767 - 0.5);
	float b = rand(); float py = 10*(b/32767 - 0.5)+15;//Devido à altura do eixo y
	float c = rand(); float pz = 10*(c/32767 - 0.5);

	actorDesc.globalPose.t  = NxVec3(px,py,pz);
	this->ator = cena->createActor(actorDesc);
	this->ator->userData =  (void *)this;


}
PrismaTriangularTruncado::~PrismaTriangularTruncado(){

}
bool PrismaTriangularTruncado::estaInterceptadoPeloPlano(NxVec3 planoGlobalPosition){
	return false;
}



Intercepto* PrismaTriangularTruncado::getIntercepto(NxVec3 planoPos){


	return NULL;
}

inline vector<SegmentoDeReta> PrismaTriangularTruncado::getSegmentosDeRetaInterceptados(NxVec3 planoPos){
	NxVec3 *vertices = getPosicaoGlobalDosVertices();
	vector<SegmentoDeReta> segmentos;

	

	return segmentos;
}

inline NxVec3* PrismaTriangularTruncado::getPosicaoGlobalDosVertices(){
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