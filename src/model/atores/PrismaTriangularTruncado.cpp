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
#include "../../utils/GlobalPoseFactory.h"


using std::runtime_error;
using simulacao::model::Parametros;
using namespace simulacao::model::atores;
using namespace simulacao::model::interceptos;
using namespace simulacao::utils;
using std::vector;
using std::map;
using std::list;

/** 
	Sobre a estrutura, vertices e o significado das variáveis definidas neste construtor
	ver arquivo docs/PrismaTriangularTruncado.doc
*/
PrismaTriangularTruncado::PrismaTriangularTruncado(NxScene *cena,NxCCDSkeleton *ccds,MeshFactory *meshFactory):Ator(){
	
	this->razaoDeAspecto = Parametros::getInstance()->getRazaoAspectoDoPrismaTriangularTruncado();
	this->razaoDeTruncamento  = Parametros::getInstance()->getRazaoDeTruncamentoDoPrismaTriangularTruncado();
	this->L0  = Parametros::getInstance()->getL0DoPrismaTriangularTruncado();
	this->cor = Parametros::getInstance()->getCorDoPrismaTriangularTruncado();
	
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

	actorDesc.globalPose.t  = GlobalPoseFactory::getInstance()->newGlobalPosition();
	this->ator = cena->createActor(actorDesc);
	this->ator->userData =  (void *)this;


}

	// para calcular o volume de um prisma triangular truncado
	// eu primeiro calculo o volume de um prisma triangular normal (area da base vezes a altura)
	// logo em seguida subtraio o volume equivalente dos cantos que são retirados
	// para criar o truncamento

#define AREA_DO_TRIANGLO_EQUILATERO(LADO) ( pow(LADO,2)*sqrt(3.0)/4.0 )

double PrismaTriangularTruncado::calcularVolume(double razaoDeAspecto,double razaoDeTruncamento,double L0){
	if ( razaoDeAspecto < 0 || razaoDeAspecto > 1 )
		throw new runtime_error("Razão de aspecto inválida. Apenas valores 0 <= razaoDeAspecto <= 1 são aceitos");
		
	if (razaoDeTruncamento<0 || razaoDeTruncamento>0.5)
		throw new runtime_error("Razão de truncamento inválida. Apenas valores 0 <= razaoDeTruncamento <= 0.5 são aceitos");

	double alturaDoPrisma = L0 * razaoDeAspecto;
	double volumeDeUmPrismaTriangular = alturaDoPrisma * AREA_DO_TRIANGLO_EQUILATERO(L0);

	// agora calculando o volume do prisma que será retirado de cada vértice do prisma
	// triangular p/ criar o prisma triangular truncado
	double alturaDoPrismaDoVertice = L0 * razaoDeAspecto;
	double arestaDoPrismaDoVertice = razaoDeTruncamento*L0; // parâmetro L1;
	double volumeDeUmPrismaTriangularDoVertice = alturaDoPrismaDoVertice * AREA_DO_TRIANGLO_EQUILATERO(arestaDoPrismaDoVertice);

	return volumeDeUmPrismaTriangular - 3*volumeDeUmPrismaTriangularDoVertice;
}

bool PrismaTriangularTruncado::estaInterceptadoPeloPlano(NxVec3 planoGlobalPosition){
	NxShape *mesh =  this->ator->getShapes()[0];
	NxVec3 pos = ator->getGlobalPosition();

	NxMat34 pose = mesh->getGlobalPose();

	NxConvexMeshDesc meshDesc;
	mesh->isConvexMesh()->getConvexMesh().saveToDesc(meshDesc); 

	NxU32 nbVerts = meshDesc.numVertices;	
	NxVec3* points = (NxVec3 *)meshDesc.points;

	// alto e baixo em termos da coordenada Y
	NxReal verticeMaisAlto=_FPCLASS_NINF, verticeMaisBaixo=_FPCLASS_PINF;

	for(NxU32 i=0; i < nbVerts ; ++i){
		NxVec3 vertice = (pose.M * points[i] + pose.t);

		if ( verticeMaisAlto < vertice.y)
			verticeMaisAlto = vertice.y;
		else
			if (verticeMaisBaixo > vertice.y)
				verticeMaisBaixo = vertice.y;			
	}

	return (verticeMaisAlto > planoGlobalPosition.y && verticeMaisBaixo < planoGlobalPosition.y);
}



Intercepto* PrismaTriangularTruncado::getIntercepto(NxVec3 planoPos){
	
	vector<SegmentoDeReta> segmentosDeRetaInterceptados = getSegmentosDeRetaInterceptados(planoPos);
	int a=segmentosDeRetaInterceptados.size();
	
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

	assert(poligonoPontos.size(),segmentosDeRetaInterceptados.size());
	
	return new Poligono(this->cor,poligonoPontos,this->razaoDeAspecto,this->razaoDeTruncamento,this->L0);
}

inline vector<SegmentoDeReta> PrismaTriangularTruncado::getSegmentosDeRetaInterceptados(NxVec3 planoPos){
	NxVec3 *vertices = getPosicaoGlobalDosVertices();
	vector<SegmentoDeReta> segmentos;

	vector<NxVec3> verticesAcimaDoPlanoDeCorte;
	vector<NxVec3> verticesAbaixoDoPlanoDeCorte;

	for(int i=0;i<12;++i)
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

inline NxVec3* PrismaTriangularTruncado::getPosicaoGlobalDosVertices(){
	NxVec3 *vertices = (NxVec3 *) malloc(sizeof(NxVec3)*12);

	NxShape *mesh =  this->ator->getShapes()[0];
	NxConvexMeshDesc meshDesc;
	mesh->isConvexMesh()->getConvexMesh().saveToDesc(meshDesc); 

	NxU32 nbVerts = meshDesc.numVertices;	
	assert(nbVerts ==12);
	
	// ver arquivo docs\PrismaTriangularTruncado.doc para
	// conhecer a ordem em que esses pontos estão definidos
	NxVec3* points = (NxVec3 *)meshDesc.points;
	NxMat34 pose = mesh->getGlobalPose();
	// convertendo de posicionamento local p/ global
	for(register int i=0;i<12;++i)
		vertices[i] = (pose.M * points[i] + pose.t);
	
	return vertices;
}