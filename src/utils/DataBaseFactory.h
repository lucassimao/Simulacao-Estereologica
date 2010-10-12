#ifndef DATABASE_FACTORY
#define DATABASE_FACTORY

#include "..\sqlite3\sqlite3.h"

class DataBaseFactory{

	private:
		DataBaseFactory();
		static DataBaseFactory* instance;

	public:
		static DataBaseFactory* getInstance(){
			if (!DataBaseFactory::instance)
				instance = new DataBaseFactory();

			return DataBaseFactory::instance;
		}
		sqlite3 *criarBanco(const char*);
};


#endif
