#ifndef GRADE_H
#define GRADE_H

#include <vector>
#include "RetaDeTeste.h"
using std::vector;

namespace simulacao{

	namespace model{

		namespace grade{

			class Grade{
			private:
				vector<RetaDeTeste> linhas;
			public:
				Grade(float z0, float z1,float altura,int linhas,int pontosPorLinha);
				vector<RetaDeTeste> getLinhasNoIntervalo(double z0, double z1);
				inline vector<RetaDeTeste>::const_iterator getLinhasIterator(){ return this->linhas.begin(); };
				inline vector<RetaDeTeste>::const_iterator getLinhasIteratorEnd(){ return this->linhas.end(); };
				

			};
		}

	}
}

#endif