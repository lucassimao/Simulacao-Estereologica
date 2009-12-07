#ifndef POLIGONO_DRAW_COMMAND_H
#define POLIGONO_DRAW_COMMAND_H

#include "DrawCommand.h"

namespace simulacao{ 
	namespace canvas {
		namespace interceptos{
			namespace designer{

				class PoligonoDrawCommand:DrawCommand{
				public:
					PoligonoDrawCommand();
					inline void draw();

				};
			}

		}
	}
}

#endif