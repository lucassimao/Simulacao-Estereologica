#ifndef MESH_FACTORY_H
#define MESH_FACTORY_H

#include <NxPhysics.h>
#include <stdexcept>
#include "../draw/cooking.h"
#include "../draw/Stream.h"



/**
 * Essa é uma classe especialista que "sabe" como
 * utilizar um objeto do tipo NxPhysicsSDK para criar meshes
*/
namespace simulacao{

	namespace utils{

		class MeshFactory{
		private:
			NxPhysicsSDK *physicsSDK;
		public:
			MeshFactory(NxPhysicsSDK *physicsSDK);
			NxTriangleMesh *criarTriangleMesh(int numVertices,int numTriangles,NxVec3 *points,NxU32 *triangles);
			NxConvexMesh *criarConvexMesh(NxConvexMeshDesc* convexDesc);
			

		};
	}
}

#endif