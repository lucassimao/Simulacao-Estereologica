#include <map>
#include <set>
#include <vector>
#include "Cubo.h"
#include <NxExportedUtils.h>
#include "..\..\model\interceptos\Poligono.h"
#include "..\..\model\interceptos\Polilinha.h"

using namespace simulacao::model::interceptos;
using namespace simulacao::model::atores;
using std::vector;
using std::map;

Cubo::Cubo(NxScene *cena,double aresta):Ator()
{
	this->lado = aresta;

	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;
	NxBoxShapeDesc boxDesc;

	//sphereDesc.shapeFlags |= NX_SF_DYNAMIC_DYNAMIC_CCD;
	boxDesc.dimensions.set(this->lado,this->lado,this->lado);

	//boxDesc.ccdSkeleton = ccds;
	actorDesc.shapes.pushBack(&boxDesc);

	actorDesc.body = &bodyDesc;
	actorDesc.density=10.0f;
	float a = rand(); float px = 10*(a/32767 - 0.5);
	float b = rand(); float py = 10*(b/32767 - 0.5)+ 15.0;//Devido à altura do eixo y
	float c = rand(); float pz = 10*(c/32767 - 0.5);

	actorDesc.globalPose.t = NxVec3(px,py,pz);
	NxActor *box =  cena->createActor(actorDesc);
	box->userData = (void *)this;
	this->ator = box;
}

Intercepto* Cubo::getIntercepto(NxVec3 planoPos){
	Cor v = VERMELHO;
	

	vector<SegmentoDeReta> segmentosDeRetaInterceptados = getSegmentosDeRetaInterceptados(planoPos);
	vector<SegmentoDeReta>::const_iterator iterator = segmentosDeRetaInterceptados.begin();

	while(iterator!=segmentosDeRetaInterceptados.end())
	{
		SegmentoDeReta seg = *iterator;
		iterator++;
	}
	
	return new Polilinha(v,segmentosDeRetaInterceptados);
}

Cubo::~Cubo(void)
{
}

bool Cubo::estaInterceptadoPeloPlano(NxVec3 planoGlobalPosition){
	NxVec3 *vertices = getPosicaGlobalDosVertices();
	NxVec3 pos = ator->getGlobalPosition();

	for(int i=0;i<8;++i){
		//distância da altura do centro do cubo para a altura do plano
		NxReal distancia_Y2 = abs(pos.y - planoGlobalPosition.y);
		//distância da altura do centro do cubo para o vértice
		NxReal distancia_Y3 = abs(pos.y - vertices[i].y);
		if (distancia_Y2 <= distancia_Y3)
			return true;
	}
	return false;
}

// método que retorna os vértices da caixa
inline NxVec3* Cubo::getPosicaGlobalDosVertices(){
	NxVec3 *vertices = (NxVec3 *) malloc(sizeof(NxVec3)*8);
	NxVec3 pos = ator->getGlobalPosition();
	NxBox box;
	this->ator->getShapes()[0]->isBox()->getWorldOBB(box);
	NxComputeBoxPoints(box,vertices);
	return vertices;
}



// Retorna os segmentos de reta (arestas) do cubo que estão interceptados pelo cubo
inline vector<SegmentoDeReta> Cubo::getSegmentosDeRetaInterceptados(NxVec3 planoPos){
	NxVec3 *vertices = getPosicaGlobalDosVertices();
	vector<SegmentoDeReta> segmentos;

	map<int,NxVec3> verticesAcimaDoPlanoDeCorte;
	map<int,NxVec3> verticesAbaixoDoPlanoDeCorte;

	for(int i=0;i<8;++i)
		if (vertices[i].y > planoPos.y)
			verticesAcimaDoPlanoDeCorte[i] = vertices[i];
		else
			verticesAbaixoDoPlanoDeCorte[i] = vertices[i];

	map<int,NxVec3>::const_iterator iteratorVerticesAcimaDoPlanoDeCorte = verticesAcimaDoPlanoDeCorte.begin();

	while(iteratorVerticesAcimaDoPlanoDeCorte!=verticesAcimaDoPlanoDeCorte.end()){

		int indice_vertice = iteratorVerticesAcimaDoPlanoDeCorte->first;
		NxVec3 vec = iteratorVerticesAcimaDoPlanoDeCorte->second;
		map<int,NxVec3>::const_iterator iterator;
		
		switch(indice_vertice){
			case 0:
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
				iterator = verticesAbaixoDoPlanoDeCorte.find(4);
				if (iterator!=verticesAbaixoDoPlanoDeCorte.end()){
					Vetor v1(vec.x,vec.y,vec.z);
					Vetor v2(iterator->second.x,iterator->second.y,iterator->second.z);
					segmentos.push_back(SegmentoDeReta(v1,v2));
				}
				break;
			case 1:
				iterator = verticesAbaixoDoPlanoDeCorte.find(0);
				if (iterator!=verticesAbaixoDoPlanoDeCorte.end()){
					Vetor v1(vec.x,vec.y,vec.z);
					Vetor v2(iterator->second.x,iterator->second.y,iterator->second.z);
					segmentos.push_back(SegmentoDeReta(v1,v2));
				}
				iterator = verticesAbaixoDoPlanoDeCorte.find(2);
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
				iterator = verticesAbaixoDoPlanoDeCorte.find(6);
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
			case 3:
				iterator = verticesAbaixoDoPlanoDeCorte.find(2);
				if (iterator!=verticesAbaixoDoPlanoDeCorte.end()){
					Vetor v1(vec.x,vec.y,vec.z);
					Vetor v2(iterator->second.x,iterator->second.y,iterator->second.z);
					segmentos.push_back(SegmentoDeReta(v1,v2));
				}
				iterator = verticesAbaixoDoPlanoDeCorte.find(7);
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
			case 4:
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
				iterator = verticesAbaixoDoPlanoDeCorte.find(7);
				if (iterator!=verticesAbaixoDoPlanoDeCorte.end()){
					Vetor v1(vec.x,vec.y,vec.z);
					Vetor v2(iterator->second.x,iterator->second.y,iterator->second.z);
					segmentos.push_back(SegmentoDeReta(v1,v2));
				}
				break;
			case 5:
				iterator = verticesAbaixoDoPlanoDeCorte.find(4);
				if (iterator!=verticesAbaixoDoPlanoDeCorte.end()){
					Vetor v1(vec.x,vec.y,vec.z);
					Vetor v2(iterator->second.x,iterator->second.y,iterator->second.z);
					segmentos.push_back(SegmentoDeReta(v1,v2));
				}
				iterator = verticesAbaixoDoPlanoDeCorte.find(6);
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
			case 6:
				iterator = verticesAbaixoDoPlanoDeCorte.find(5);
				if (iterator!=verticesAbaixoDoPlanoDeCorte.end()){
					Vetor v1(vec.x,vec.y,vec.z);
					Vetor v2(iterator->second.x,iterator->second.y,iterator->second.z);
					segmentos.push_back(SegmentoDeReta(v1,v2));
				}
				iterator = verticesAbaixoDoPlanoDeCorte.find(2);
				if (iterator!=verticesAbaixoDoPlanoDeCorte.end()){
					Vetor v1(vec.x,vec.y,vec.z);
					Vetor v2(iterator->second.x,iterator->second.y,iterator->second.z);
					segmentos.push_back(SegmentoDeReta(v1,v2));
				}
				iterator = verticesAbaixoDoPlanoDeCorte.find(7);
				if (iterator!=verticesAbaixoDoPlanoDeCorte.end()){
					Vetor v1(vec.x,vec.y,vec.z);
					Vetor v2(iterator->second.x,iterator->second.y,iterator->second.z);
					segmentos.push_back(SegmentoDeReta(v1,v2));
				}
				break;
			case 7:
				iterator = verticesAbaixoDoPlanoDeCorte.find(6);
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
		}
		++iteratorVerticesAcimaDoPlanoDeCorte;
		
	}

	return segmentos;
}