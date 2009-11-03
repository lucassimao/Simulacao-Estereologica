#ifndef MESH_FACTORY_H
#define MESH_FACTORY_H

#include <NxPhysics.h>
#include <stdexcept>
#include "../draw/cooking.h"
#include "../draw/Stream.h"




namespace simulacao{

	namespace utils{

		class MeshFactory{
		private:
			NxPhysicsSDK *physicsSDK;
		public:
			MeshFactory(NxPhysicsSDK *physicsSDK);
			NxTriangleMesh *criarMesh(int numVertices,int numTriangles,NxVec3 *points,NxU32 *triangles);
			

		};
	}
}

#endif