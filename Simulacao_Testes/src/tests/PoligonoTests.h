#include <gtest/gtest.h>
#include <model/interceptos/Poligono.h>
#include <model/grade/RetaDeTeste.h>
#include <defs.h>
#include <list>
#include <iostream>

using namespace simulacao::model::interceptos;
using namespace simulacao::model::grade;
using namespace std;

namespace {


	TEST(PoligonoTest, testarCalcularPerimetro) {
		Cor c = VERMELHO;
		list<Ponto> vertices;
		double alturaDoPlanoDeCorte = 10;

		Ponto p1 = {2,alturaDoPlanoDeCorte,0};
		Ponto p2 = {7,alturaDoPlanoDeCorte,10};
		Ponto p3 = {12,alturaDoPlanoDeCorte,0};
		Ponto p4 = {7,alturaDoPlanoDeCorte,-10};

		// adicionando os vértices aleatoriamente, para confirmar o algoritmo de ordenação
		vertices.push_back(p2);
		vertices.push_back(p4);
		vertices.push_back(p1);
		vertices.push_back(p3);
		Ponto ps[] = {p2,p3,p4,p1};
	
		Poligono quadrado(c,vertices,0,0,0,0);
		int i=0;
		
		list<Ponto> verticesOrdenadosDoQuadrado = quadrado.getVertices();
		list<Ponto>::const_iterator iter = verticesOrdenadosDoQuadrado.begin();
		while(iter!= verticesOrdenadosDoQuadrado.end()){
			Ponto p = *iter;			
			ASSERT_EQ(p.x,ps[i].x);
			ASSERT_EQ(p.y,ps[i].y);
			ASSERT_EQ(p.z,ps[i].z);
			++i;
			++iter;
		}
		double perimetroPrevisto = 4.0 * sqrt(125.0);

		ASSERT_NEAR(perimetroPrevisto,quadrado.getPerimetro(),0.00001);

	}
	

	TEST(PoligonoTest, testarCalcularAreaEPerimetroDeQuadrado) {
		Cor c = VERMELHO;
		list<Ponto> vertices;
		double alturaDoPlanoDeCorte = 10;

		Ponto p1 = {10,alturaDoPlanoDeCorte,5};
		Ponto p2 = {10,alturaDoPlanoDeCorte,15};
		Ponto p3 = {20,alturaDoPlanoDeCorte,15};
		Ponto p4 = {20,alturaDoPlanoDeCorte,5};

		vertices.push_back(p1);
		vertices.push_back(p2);
		vertices.push_back(p3);
		vertices.push_back(p4);

		Poligono quadrado(c,vertices,0,0,0,0);

		ASSERT_EQ(100.0,quadrado.getArea());
		ASSERT_EQ(40.0,quadrado.getPerimetro());

	}

	TEST(PoligonoTest, testarCalcularPerimetroEAreaDeTrinagulo) {
		Cor c = VERMELHO;
		list<Ponto> vertices;
		double alturaDoPlanoDeCorte = 10;

		Ponto p1 = {2,alturaDoPlanoDeCorte,0};
		Ponto p2 = {7,alturaDoPlanoDeCorte,10};
		Ponto p3 = {12,alturaDoPlanoDeCorte,0};

		// adicionando os vértices aleatoriamente, para confirmar
		// o algoritmo de ordenação
		vertices.push_back(p2);
		vertices.push_back(p1);
		vertices.push_back(p3);
		

		Poligono triangulo(c,vertices,0,0,0,0);
		double perimetroPrevisto = 2.0 * sqrt(125.0) + 10;
		double areaPrevista = 50;

		ASSERT_NEAR(perimetroPrevisto,triangulo.getPerimetro(),0.00001);
		ASSERT_NEAR(areaPrevista,triangulo.getArea(),0.00001);

	}

	TEST(PoligonoTest, testarCalcularPerimetroEAreaDePentagonoIrregular) {
		Cor c = VERMELHO;
		list<Ponto> vertices;
		double alturaDoPlanoDeCorte = 12.89;

		Ponto p1 = {0,alturaDoPlanoDeCorte,0};
		Ponto p2 = {0,alturaDoPlanoDeCorte,9.73};
		Ponto p3 = {30.47,alturaDoPlanoDeCorte,9.73};
		Ponto p4 = {30.47,alturaDoPlanoDeCorte,0};
		Ponto p5 = { (30.47/2.0),alturaDoPlanoDeCorte,-12.5};

		// adicionando os vértices aleatoriamente, para confirmar
		// o algoritmo de ordenação
		vertices.push_back(p4);
		vertices.push_back(p2);
		vertices.push_back(p1);
		vertices.push_back(p3);
		vertices.push_back(p5);
		

		Poligono pentagono(c,vertices,0,0,0,0);
		double perimetroPrevisto = 2 * 9.73 + 30.47 +19.7067304492653*2;
		double areaPrevista = 9.73*30.47 + (30.47*12.5)/2.0;

		ASSERT_NEAR(perimetroPrevisto,pentagono.getPerimetro(),0.00001);
		ASSERT_NEAR(areaPrevista,pentagono.getArea(),0.001);

	}

	TEST(PoligonoTest, testarCalcularPerimetroEAreaDeHexagonoRegular) {
		Cor c = VERMELHO;
		list<Ponto> vertices;
		double alturaDoPlanoDeCorte = -4.89;

		Ponto p1 = {5,alturaDoPlanoDeCorte,-5};
		Ponto p2 = {0,alturaDoPlanoDeCorte,0};
		Ponto p3 = {0,alturaDoPlanoDeCorte,sqrt(50.0)};
		Ponto p4 = {5,alturaDoPlanoDeCorte,sqrt(50.0) + 5};
		Ponto p5 = {10,alturaDoPlanoDeCorte,sqrt(50.0)};
		Ponto p6 = {10,alturaDoPlanoDeCorte,0};

		// adicionando os vértices aleatoriamente, para confirmar
		// o algoritmo de ordenação
		vertices.push_back(p4);
		vertices.push_back(p2);
		vertices.push_back(p1);
		vertices.push_back(p3);
		vertices.push_back(p5);
		vertices.push_back(p6);

		Poligono pentagono(c,vertices,0,0,0,0);
		double perimetroPrevisto = 6 * sqrt(50.0);
		double areaPrevista = sqrt(50.0) * 10 + (10 * 5);

		ASSERT_NEAR(perimetroPrevisto,pentagono.getPerimetro(),0.00001);
		ASSERT_NEAR(areaPrevista,pentagono.getArea(),0.00001);

	}

	TEST(PoligonoTest, testarArestasInterceptadas) {
		Cor c = VERMELHO;
		list<Ponto> vertices;
		double alturaDoPlanoDeCorte = -4.89;

		Ponto p1 = {5,alturaDoPlanoDeCorte,-5};
		Ponto p2 = {0,alturaDoPlanoDeCorte,0};
		Ponto p3 = {0,alturaDoPlanoDeCorte,sqrt(50.0)};
		Ponto p4 = {5,alturaDoPlanoDeCorte,sqrt(50.0) + 5};
		Ponto p5 = {10,alturaDoPlanoDeCorte,sqrt(50.0)};
		Ponto p6 = {10,alturaDoPlanoDeCorte,0};

		// adicionando os vértices aleatoriamente, para confirmar
		// o algoritmo de ordenação
		vertices.push_back(p4);
		vertices.push_back(p2);
		vertices.push_back(p1);
		vertices.push_back(p3);
		vertices.push_back(p5);
		vertices.push_back(p6);

		Poligono pentagono(c,vertices,0,0,0,0);

		Ponto pa = {0,alturaDoPlanoDeCorte,sqrt(50.0)/2.0};
		Ponto pb = {10,alturaDoPlanoDeCorte,sqrt(50.0)/2.0};

		RetaDeTeste r1(pa,pb,1);

		Ponto pc = {6,alturaDoPlanoDeCorte,sqrt(50.0)/2.0};
		Ponto pd = {10,alturaDoPlanoDeCorte,sqrt(50.0)/2.0};

		RetaDeTeste r2(pc,pd,1);


		list<SegmentoDeReta> arestasInterceptadas1 = pentagono.getArestasInterceptadas(r1);
		ASSERT_EQ(2,arestasInterceptadas1.size());

		list<SegmentoDeReta> arestasInterceptadas2 = pentagono.getArestasInterceptadas(r2);
		ASSERT_EQ(1,arestasInterceptadas2.size());


	}	
}