#ifndef DATABASE_FACTORY
#define DATABASE_FACTORY

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
		bool criarBanco(const char*);
};


#endif
