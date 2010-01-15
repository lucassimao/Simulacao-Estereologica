#ifndef POLILINHA_H
#define POLILINHA_H

#include <vector>
#include "Intercepto.h"
#include "..\..\utils\SegmentoDeReta.h"
#include "..\..\defs.h"

using std::vector;

namespace simulacao{ 
	namespace model {
		namespace interceptos{

			class Polilinha: public Intercepto{				
			public:
				vector<SegmentoDeReta> vertices;
				Polilinha(Cor cor,vector<SegmentoDeReta> vertices);
				void accept(AbstractDrawVisitor *);
				bool contemPonto(Ponto p);
				
			};

		}
	}
}

#endif