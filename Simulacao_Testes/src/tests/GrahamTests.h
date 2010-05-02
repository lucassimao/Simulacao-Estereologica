#include <gtest/gtest.h>
#include <utils/graham.h>
#include <defs.h>
#include <list>

using namespace simulacao::utils;
using std::list;

namespace {

	TEST(GrahamTest, testarOrdenacaoDeVertices) {
		list<Ponto> vertices;
		
		Ponto p1 = {1,10,2};
		Ponto p2 = {3,10,1};
		Ponto p3 = {6,10,2};
		Ponto p4 = {5,10,5};
		Ponto p5 = {4,10,6};
		Ponto p6 = {2,10,4};
		Ponto pontos[] = {p1,p6,p5,p4,p3,p2}; // sequência esperada

		vertices.push_back(p1);
		vertices.push_back(p4);
		vertices.push_back(p3);
		vertices.push_back(p6);
		vertices.push_back(p5);
		vertices.push_back(p2);

		list<Ponto> verticesOrdenados = ordenarVertices(&vertices);
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



	TEST(GrahamTest, testarOrdenacaoDeVerticesMuitoProximos) {
		list<Ponto> vertices;
		
		Ponto p1 = {1.1,10,4};
		Ponto p2 = {3.2,10,3.9};
		Ponto p3 = {3,10,2.1};
		Ponto p4 = {1,10,2};
		Ponto pontos[] = {p1,p2,p3,p4}; // sequência esperada

		vertices.push_back(p4);
		vertices.push_back(p2);
		vertices.push_back(p1);
		vertices.push_back(p3);

		list<Ponto> verticesOrdenados =  ordenarVertices(&vertices);

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
