#ifndef OBSERVABLE_H
#define OBSERVABLE_H


#include <vector>

using std::vector;

namespace simulacao{

	namespace utils{

		class Observer;

		class Observable{
		private:
			vector<Observer*> observadores;
		public:
			void addObserver(Observer*);
			void notifyObservers();	
		};
	}

}

#endif