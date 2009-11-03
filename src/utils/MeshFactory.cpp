#include "MeshFactory.h"

using namespace simulacao::utils;
using std::runtime_error;

MeshFactory::MeshFactory(NxPhysicsSDK *physicsSDK){
	this->physicsSDK = physicsSDK;
}

NxTriangleMesh * MeshFactory::criarMesh(int numVertices,int numTriangles,NxVec3 *points,NxU32 *triangles){
	bool status = InitCooking();

	if (!status) {
		throw runtime_error("\nError: Unable to initialize the cooking library. Please make sure that you have correctly installed the latest version of the AGEIA PhysX SDK.\n\n");
	}

	NxTriangleMeshDesc meshDesc;
	meshDesc.numVertices = numVertices;
	meshDesc.numTriangles = numTriangles;
	meshDesc.pointStrideBytes = sizeof(NxVec3);
	meshDesc.triangleStrideBytes = 3 * sizeof(NxU32);
	meshDesc.points = points;
	meshDesc.triangles = triangles;
	MemoryWriteBuffer buf;

	status = CookTriangleMesh(meshDesc, buf);

	if (!status) {
		throw runtime_error("Unable to cook a triangle mesh.");
		exit(1);
	}

	MemoryReadBuffer readBuffer(buf.data);
	CloseCooking();
	return physicsSDK->createTriangleMesh(readBuffer);
}
