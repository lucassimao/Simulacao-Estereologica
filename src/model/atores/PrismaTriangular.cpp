#include <NxPhysics.h>
#include <QtDebug>
#include <stdexcept>
#include <cfloat>
#include "PrismaTriangular.h"
#include "../../draw/cooking.h"

using std::runtime_error;
using namespace simulacao::model::atores;

PrismaTriangular::PrismaTriangular(NxScene *cena,int h,int base,NxCCDSkeleton *ccds,MeshFactory *meshFactory):Ator(){
	this->altura = h;
	this->base  = base;
	
	const int raiz_de_3 = sqrt(3.0);

	NxVec3 verts[6] = { 
		NxVec3(0,h/2,base*raiz_de_3/2), 
		NxVec3(base/2,h/2,0), 
		NxVec3(base/2,-h/2,0), 
		NxVec3(-base/2,-h/2,0), 
		NxVec3(-base/2,h/2,0), 
		NxVec3(0,-h/2,base *raiz_de_3/2)
	};


	NxConvexMeshDesc* convexDesc = new NxConvexMeshDesc();
	convexDesc->numVertices			= 6;
	convexDesc->pointStrideBytes	= sizeof(NxVec3);
	convexDesc->points				= verts;
	convexDesc->flags				= NX_CF_COMPUTE_CONVEX;

	NxConvexShapeDesc convexShapeDesc;
	convexShapeDesc.localPose.t = NxVec3(0,this->altura,0);
	convexShapeDesc.userData = convexDesc;

	convexShapeDesc.meshData = meshFactory->criarConvexMesh(convexDesc);

	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;
	actorDesc.shapes.pushBack(&convexShapeDesc);

	actorDesc.body = &bodyDesc;
	actorDesc.density = 10.0;

	float a = rand(); float px = 10*(a/32767 - 0.5);
	float b = rand(); float py = 10*(b/32767 - 0.5)+ 15.0;//Devido à altura do eixo y
	float c = rand(); float pz = 10*(c/32767 - 0.5);

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


	//qDebug()<<pos.y << " " << planoGlobalPosition.y <<"\n";
	// alto e baixo em termos da coordenada Y
	NxReal verticeMaisAlto=_FPCLASS_NINF, verticeMaisBaixo=_FPCLASS_PINF;


	for(int i=0;i<nbVerts;++i){
		NxVec3 vertice = (pose.M * points[i] + pose.t);
		//qDebug()<< vertice.x <<" "<< vertice.y<<" "<< vertice.z <<"\n";

		if ( verticeMaisAlto < vertice.y)
			verticeMaisAlto = vertice.y;
		else
		if (verticeMaisBaixo > vertice.y)
			verticeMaisBaixo = vertice.y;			
	}
	//qDebug()<< verticeMaisAlto <<" "<< verticeMaisBaixo<<"\n";

	return (verticeMaisAlto >= planoGlobalPosition.y && verticeMaisBaixo <= planoGlobalPosition.y);

}

void PrismaTriangular::draw(bool useShapeUserData){

}

vector<NxVec3> PrismaTriangular::getInterceptacoes(NxVec3 planoGlobalPosition){
	vector<NxVec3> pontos;

	NxShape *mesh =  this->ator->getShapes()[0];
	NxMat34 pose = mesh->getGlobalPose();
	NxConvexMeshDesc meshDesc;
	mesh->isConvexMesh()->getConvexMesh().saveToDesc(meshDesc); 

	NxU32 nbVerts = meshDesc.numVertices;	
	// ver arquivo docs\prismaTriangularVertices.png para
	// conhecer a ordem em que esses pontos estão definidos
	NxVec3* points = (NxVec3 *)meshDesc.points;
	
	NxVec3 vertic1 = (pose.M * points[0] + pose.t);
	NxVec3 vertic2 = (pose.M * points[1] + pose.t);
	NxVec3 vertic3 = (pose.M * points[2] + pose.t);
	NxVec3 vertic4 = (pose.M * points[3] + pose.t);
	NxVec3 vertic5 = (pose.M * points[4] + pose.t);
	NxVec3 vertic6 = (pose.M * points[5] + pose.t);

	return pontos;

	





	
}