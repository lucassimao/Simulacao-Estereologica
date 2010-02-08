#ifndef OBSERVER_H
#define OBSERVER_H


#include <vector>

namespace simulacao{

namespace utils{
	
	class Observable;

	class Observer{
	public:
		virtual void update(Observable*) = 0;
	};
}

}

#endif