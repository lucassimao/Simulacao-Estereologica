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
#include "../../model/interceptos/InterceptoDeArea.h"
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
Sobre a estrutura, vertices e o significado das vari�veis definidas neste construtor
ver arquivo docs/PrismaTriangularTruncado.doc
*/
PrismaTriangularTruncado::PrismaTriangularTruncado(NxScene *cena,MeshFactory *meshFactory,Cor cor):Ator(){

	this->razaoDeAspecto = Parametros::getInstance()->getRazaoAspectoDoPrismaTriangularTruncado();
	this->razaoDeTruncamento  = Parametros::getInstance()->getRazaoDeTruncamentoDoPrismaTriangularTruncado();
	this->L0  = Parametros::getInstance()->getL0DoPrismaTriangularTruncado();
	this->cor = cor;

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
	actorDesc.density = 10;

	NxVec3 posicaoInicial = GlobalPoseFactory::getInstance()->newGlobalPosition();
	if (posicaoInicial.y + alturaDoPrisma >= Parametros::getInstance()->getAlturaDoTopoDaCaixa()){
		posicaoInicial.y -= 2*alturaDoPrisma;
	}

	actorDesc.globalPose.t  = posicaoInicial;
	this->ator = cena->createActor(actorDesc);
	this->ator->userData =  (void *)this;

	double Pi =  3.14159265;
	NxVec3 vetores[3];

	for(int i=0;i<3;++i){
		double random=rand()/(RAND_MAX +1.0); // gera um n�mero entre [0;1]	
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

double PrismaTriangularTruncado::calcularVolume(double razaoDeAspecto,double razaoDeTruncamento,double L0){
	if ( razaoDeAspecto < 0 || razaoDeAspecto > 1 )
		throw new runtime_error("Raz�o de aspecto inv�lida. Apenas valores 0 <= razaoDeAspecto <= 1 s�o aceitos");

	if (razaoDeTruncamento<0 || razaoDeTruncamento>0.49)
		throw new runtime_error("Raz�o de truncamento inv�lida. Apenas valores 0 <= razaoDeTruncamento <= 0.5 s�o aceitos");


	double volume =  (sqrt(3.0)/4.0)*razaoDeAspecto*(1 - 3*pow(razaoDeTruncamento,2))*pow(L0,3);
	assert (volume>=0);
	return volume;
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



InterceptoDeArea* PrismaTriangularTruncado::getIntercepto(NxVec3 planoPos){

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

	assert(poligonoPontos.size(),segmentosDeRetaInterceptados.size());

	return new Poligono(this->cor,poligonoPontos,this->razaoDeAspecto,this->razaoDeTruncamento,this->L0,getAtorID());
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
	// conhecer a ordem em que esses pontos est�o definidos
	NxVec3* points = (NxVec3 *)meshDesc.points;
	NxMat34 pose = mesh->getGlobalPose();
	// convertendo de posicionamento local p/ global
	for(register int i=0;i<12;++i)
		vertices[i] = (pose.M * points[i] + pose.t);

	return vertices;
}