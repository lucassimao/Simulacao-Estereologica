#ifndef DRAW_STRATEGY_H
#define DRAW_STRATEGY_H

namespace simulacao{ 
	namespace canvas {
		namespace interceptos{
			namespace designer{

				class DrawCommand{
				public:
					DrawCommand();
					virtual inline void draw() = 0;

				};
			}

		}
	}
}

#endif