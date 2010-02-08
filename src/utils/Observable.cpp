#include "Observable.h"
#include "Observer.h"
using namespace simulacao::utils;

void Observable::addObserver(Observer* obs){
	this->observadores.push_back(obs);
}

void Observable::notifyObservers(){
	vector<Observer*>::const_iterator iter = this->observadores.begin();

	while(iter!= this->observadores.end()){
		Observer* o = *iter;
		o->update(this);
		++iter;
	}
}