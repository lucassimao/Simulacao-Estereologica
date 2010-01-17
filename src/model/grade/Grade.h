#ifndef GRADE_H
#define GRADE_H

#include <vector>
#include "Linha.h"
using std::vector;

namespace simulacao{

	namespace model{

		namespace grade{

			class Grade{
			private:
				vector<Linha> linhas;
			public:
				Grade(float z0, float z1,float altura,int linhas,int pontosPorLinha);
				vector<Linha> getLinhasNoIntervalo(double z0, double z1);
				inline vector<Linha>::const_iterator getLinhasIterator(){ return this->linhas.begin(); };
				inline vector<Linha>::const_iterator getLinhasIteratorEnd(){ return this->linhas.end(); };
				

			};
		}

	}
}

#endif