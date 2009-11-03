#include <NxPhysics.h>
#include <QtDebug>
#include "PrismaTriangular.h"
#include "../../draw/cooking.h"
#include <NxExportedUtils.h>

using namespace simulacao::model::atores;

PrismaTriangular::PrismaTriangular(NxScene *cena,int h,int base,NxCCDSkeleton *ccds,MeshFactory *meshFactory):Ator(){
	this->altura = h;
	this->base  = base;

	NxVec3 boxDim(2.2,1.5,4.5);


	NxVec3 verts[6] = { NxVec3(-boxDim.x,boxDim.y*2,-boxDim.z), 
						NxVec3(boxDim.x,boxDim.y*2,-boxDim.z), 
						NxVec3(boxDim.x,-boxDim.y*2,-boxDim.z), 
						NxVec3(-boxDim.x,-boxDim.y*2,-boxDim.z), 
						NxVec3(boxDim.x,-boxDim.y*2,--boxDim.z/2), 
						NxVec3(boxDim.x,boxDim.y*2,--boxDim.z/2)
	};


	NxConvexMeshDesc* convexDesc = new NxConvexMeshDesc();
	convexDesc->numVertices			= 6;
	convexDesc->pointStrideBytes	= sizeof(NxVec3);
	convexDesc->points				= verts;
	convexDesc->flags				= NX_CF_COMPUTE_CONVEX;

	NxConvexShapeDesc convexShapeDesc;
	convexShapeDesc.localPose.t = NxVec3(0,boxDim.y,0);
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
	return false;
}

void PrismaTriangular::draw(bool useShapeUserData){

}