#include <gtest/gtest.h>
#include <utils/jarvis.h>
#include <defs.h>
#include <list>
#include <map>

#define PI 3.1415926535
using namespace simulacao::utils;
using std::list;
using std::map;

namespace {

	TEST(JarvisMarchTest,testarRadianos2Graus){
		double piRadianos = radianos2Graus(PI);
		ASSERT_EQ(piRadianos,180);

		piRadianos = radianos2Graus(PI/6.0);
		ASSERT_EQ(piRadianos,30);

		piRadianos = radianos2Graus(PI/4.0);
		ASSERT_EQ(piRadianos,45);
	}

	TEST(JarvisMarchTest,calcularAlguloRelativoAoEixoXPositivo){
		Ponto p0 = {1,0,2};
		Ponto p1 = {2,0,3};
		
		double angulo = calcularAlguloRelativoAoEixoX(p0,p1,Positivo);
		ASSERT_NEAR(315,angulo,0.0001);

		Ponto p2 = {0,0,9};
		Ponto p3 = {1,0,9};
		angulo = calcularAlguloRelativoAoEixoX(p2,p3,Positivo);
		ASSERT_NEAR(0,angulo,0.0001);

		Ponto p4 = {0,0,0};
		Ponto p5 = {0,0,-1};
		angulo = calcularAlguloRelativoAoEixoX(p4,p5,Positivo);
		ASSERT_NEAR(90,angulo,0.0001);

		Ponto p6 = {0,0,0};
		Ponto p7 = {-1,0,1};
		
		angulo = calcularAlguloRelativoAoEixoX(p6,p7,Positivo);
		ASSERT_NEAR(225,angulo,0.0001);

		Ponto p8 = {0,0,0};
		Ponto p9 = {0,0,1};
		
		angulo = calcularAlguloRelativoAoEixoX(p8,p9,Positivo);
		ASSERT_NEAR(270,angulo,0.0001);
	}

	TEST(JarvisMarchTest,calcularAlguloRelativoAoEixoXNegativo){
		Ponto p0 = {0,0,0};
		Ponto p1 = {-1,0,1};
		
		double angulo = calcularAlguloRelativoAoEixoX(p0,p1,Negativo);
		ASSERT_NEAR(45,angulo,0.0001);

		angulo = calcularAlguloRelativoAoEixoX(p0,p1,Positivo);
		ASSERT_NEAR(180+45,angulo,0.0001);

		Ponto p2 = {-1,0,-1};
		
		angulo = calcularAlguloRelativoAoEixoX(p0,p2,Negativo);
		ASSERT_NEAR(315,angulo,0.0001);

		angulo = calcularAlguloRelativoAoEixoX(p0,p2,Positivo);
		ASSERT_NEAR(90+45,angulo,0.0001);

		Ponto p3 = {1,0,-1};
		
		angulo = calcularAlguloRelativoAoEixoX(p0,p3,Negativo);
		ASSERT_NEAR(180+45,angulo,0.0001);

		angulo = calcularAlguloRelativoAoEixoX(p0,p3,Positivo);
		ASSERT_NEAR(45,angulo,0.0001);

		Ponto p4 = {1,0,1};
		
		angulo = calcularAlguloRelativoAoEixoX(p0,p4,Negativo);
		ASSERT_NEAR(180-45,angulo,0.0001);

		angulo = calcularAlguloRelativoAoEixoX(p0,p4,Positivo);
		ASSERT_NEAR(360-45,angulo,0.0001);
	}

	TEST(JarvisMarchTest,getVerticeComMenorZ){
		Ponto p1 = {1,10,-2};
		Ponto p2 = {3,10,1};
		Ponto p3 = {6,10,2};
		Ponto p4 = {5,10,5};
		Ponto p5 = {4,10,6};
		Ponto p6 = {2,10,4};

		list<Ponto> lista;
		lista.push_back(p1);lista.push_back(p2);lista.push_back(p3);lista.push_back(p4);
		lista.push_back(p5);lista.push_back(p6);

		Ponto vertice = getVerticeComMenorZ(&lista);
		ASSERT_EQ(p1.x,vertice.x);
		ASSERT_EQ(p1.y,vertice.y);
		ASSERT_EQ(p1.z,vertice.z);
	}

	TEST(JarvisMarchTest,getVerticeComMaiorZ){
		Ponto p1 = {1,10,2};
		Ponto p2 = {3,10,1};
		Ponto p3 = {6,10,2};
		Ponto p4 = {5,10,5};
		Ponto p5 = {4,10,6};
		Ponto p6 = {2,10,4};

		list<Ponto> lista;
		lista.push_back(p1);lista.push_back(p2);lista.push_back(p3);lista.push_back(p4);
		lista.push_back(p5);lista.push_back(p6);

		Ponto vertice = getVerticeComMaiorZ(&lista);
		ASSERT_EQ(p5.x,vertice.x);
		ASSERT_EQ(p5.y,vertice.y);
		ASSERT_EQ(p5.z,vertice.z);
	}

	TEST(JarvisMarchTest,getVerticeComMaiorZUsandoDesempate){
		Ponto p1 = {1,10,2};
		Ponto p2 = {3,10,1};
		Ponto p3 = {6,10,2};
		Ponto p4 = {5,10,5};
		Ponto p5 = {4,10,6};
		Ponto p6 = {2,10,6};

		list<Ponto> lista;
		lista.push_back(p1);lista.push_back(p2);lista.push_back(p3);lista.push_back(p4);
		lista.push_back(p5);lista.push_back(p6);

		Ponto vertice = getVerticeComMaiorZ(&lista);
		ASSERT_EQ(p6.x,vertice.x);
	}

	TEST(JarvisMarchTest,getVerticeComMenorZUsandoDesempate){
		Ponto p1 = {1,10,-2};
		Ponto p2 = {3,10,-2};
		Ponto p3 = {6,10,2};
		Ponto p4 = {5,10,5};
		Ponto p5 = {4,10,6};
		Ponto p6 = {2,10,6};

		list<Ponto> lista;
		lista.push_back(p1);lista.push_back(p2);lista.push_back(p3);lista.push_back(p4);
		lista.push_back(p5);lista.push_back(p6);

		Ponto vertice = getVerticeComMenorZ(&lista);
		ASSERT_TRUE(vertice == p1);
	}


	TEST(JarvisMarchTest, testarOrdenacaoDeVertices) {
		list<Ponto> vertices;
		
		Ponto p1 = {1,10,2};
		Ponto p2 = {3,10,1};
		Ponto p3 = {6,10,2};
		Ponto p4 = {5,10,5};
		Ponto p5 = {4,10,6};
		Ponto p6 = {2,10,4};
		Ponto pontos[] = {p5,p4,p3,p2,p1,p6}; // sequência esperada

		vertices.push_back(p1);vertices.push_back(p4);vertices.push_back(p3);
		vertices.push_back(p6);	vertices.push_back(p5);	vertices.push_back(p2);

		list<Ponto> verticesOrdenados = ordenarVertices(vertices);
		ASSERT_EQ(6,verticesOrdenados.size());

		int i=0;
		list<Ponto>::const_iterator iter = verticesOrdenados.begin();
		while(iter!=verticesOrdenados.end()){
			Ponto p = *iter;
			ASSERT_EQ(p.x,pontos[i].x);
			ASSERT_EQ(p.z,pontos[i].z);
			++iter;
			++i;
		}		
	}

	TEST(JarvisMarchTest, testarOrdenacaoDeVerticesComPontosInternos) {
		list<Ponto> vertices;
		
		Ponto p1 = {1,10,2};
		Ponto p2 = {3,10,1};
		Ponto p3 = {6,10,2};
		Ponto p4 = {5,10,5};
		Ponto p5 = {4,10,6};
		Ponto p6 = {2,10,4};
		Ponto p7 = {4,10,4};
		Ponto p8 = {3,10,3};
		Ponto pontos[] = {p5,p4,p3,p2,p1,p6}; // sequência esperada

		vertices.push_back(p1);vertices.push_back(p4);vertices.push_back(p3);
		vertices.push_back(p6);	vertices.push_back(p5);	vertices.push_back(p2);
		vertices.push_back(p8);vertices.push_back(p7);

		list<Ponto> verticesOrdenados = ordenarVertices(vertices);
		ASSERT_EQ(6,verticesOrdenados.size());

		int i=0;
		list<Ponto>::const_iterator iter = verticesOrdenados.begin();
		while(iter!=verticesOrdenados.end()){
			Ponto p = *iter;
			ASSERT_EQ(p.x,pontos[i].x);
			ASSERT_EQ(p.z,pontos[i].z);
			++iter;
			++i;
		}	
	}

	TEST(JarvisMarchTest, testarOrdenacaoDeVerticesComPontosInternos2) {
		list<Ponto> vertices;
		
		Ponto p1 = {0,10,0};
		Ponto p2 = {0.5,10,0};
		Ponto p3 = {0,10,-0.5};
		Ponto p4 = {-0.5,10,0};
		Ponto p5 = {0,10,0.5};
		Ponto p6 = {-1,10,1};
		Ponto p7 = {1,10,1};
		Ponto p8 = {1,10,-1};
		Ponto p9 = {-1,10,-1};
		Ponto p10= {0.5,10,0.5};
		Ponto p11= {0.5,10,-0.5};
		Ponto p12= {-0.5,10,-0.5};
		Ponto p13= {-0.5,10,0.5};
		Ponto pontos[] = {p6,p7,p8,p9}; // sequência esperada

		vertices.push_back(p1);vertices.push_back(p4);vertices.push_back(p3);
		vertices.push_back(p5);vertices.push_back(p2);
		vertices.push_back(p9);vertices.push_back(p8);vertices.push_back(p7);vertices.push_back(p6);
		vertices.push_back(p13);vertices.push_back(p12);vertices.push_back(p10);vertices.push_back(p11);

		list<Ponto> verticesOrdenados = ordenarVertices(vertices);
		ASSERT_EQ(verticesOrdenados.size(), 4);

		int i=0;
		list<Ponto>::const_iterator iter = verticesOrdenados.begin();
		while(iter!=verticesOrdenados.end()){
			Ponto p = *iter;
			ASSERT_EQ(p.x,pontos[i].x);
			ASSERT_EQ(p.z,pontos[i].z);
			++iter;
			++i;
		}	
	}

	TEST(JarvisMarchTest,testarGetVerticeMaisDistante){
		Ponto origem = {0,0,0};
		Ponto p1 = {1,0,1};
		Ponto p2 = {5,0,5};

		double angulo1 = calcularAlguloRelativoAoEixoX(origem,p1,Positivo);
		double angulo2 = calcularAlguloRelativoAoEixoX(origem,p2,Positivo);

		ASSERT_NEAR(angulo1,angulo2,0.0001);

		ASSERT_TRUE(getVerticeMaisDistante(origem,p1,p2) == p2);
	}


	TEST(JarvisMarchTest, testarOrdenacaoDeVerticesComPontosColineares) {
		list<Ponto> vertices;		
		
		Ponto p1 = {0,10,0};
		Ponto p2 = {0.5,10,0};
		Ponto p3 = {0,10,-0.5};
		Ponto p4 = {-0.5,10,0};
		Ponto p5 = {0,10,0.5};
		Ponto p6 = {-1,10,1};
		Ponto p6_p7 = {0,10,1};
		Ponto p7 = {1,10,1};
		Ponto p7_p8 = {1,10,0};
		Ponto p8 = {1,10,-1};
		Ponto p8_p9 = {0,10,-1};
		Ponto p9 = {-1,10,-1};
		Ponto p9_p6 = {-1,10,0};
		Ponto p10= {0.5,10,0.5};
		Ponto p11= {0.5,10,-0.5};
		Ponto p12= {-0.5,10,-0.5};
		Ponto p13= {-0.5,10,0.5};
		Ponto pontos[] = {p6,p7,p8,p9}; // sequência esperada

		vertices.push_back(p1);vertices.push_back(p4);vertices.push_back(p3);
		vertices.push_back(p5);vertices.push_back(p2);
		vertices.push_back(p9);vertices.push_back(p8);vertices.push_back(p7);vertices.push_back(p6);
		vertices.push_back(p13);vertices.push_back(p12);vertices.push_back(p10);vertices.push_back(p11);
		vertices.push_back(p6_p7);vertices.push_back(p7_p8);vertices.push_back(p8_p9);vertices.push_back(p9_p6);

		list<Ponto> verticesOrdenados = ordenarVertices(vertices);
		ASSERT_EQ(verticesOrdenados.size(), 4);

		int i=0;
		list<Ponto>::const_iterator iter = verticesOrdenados.begin();
		while(iter!=verticesOrdenados.end()){
			Ponto p = *iter;
			ASSERT_EQ(p.x,pontos[i].x);
			ASSERT_EQ(p.z,pontos[i].z);
			++iter;
			++i;
		}		
	}

}
