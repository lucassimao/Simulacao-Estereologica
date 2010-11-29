#ifndef GLOBALPOSE_FACTORY_H
#define GLOBALPOSE_FACTORY_H

#include <NxVec3.h>
namespace simulacao{

	namespace utils{

		class GlobalPoseFactory{
		private:
			GlobalPoseFactory();
			static GlobalPoseFactory *instance;
		public:
			static GlobalPoseFactory* GlobalPoseFactory::getInstance(){
				if (GlobalPoseFactory::instance == NULL)
					GlobalPoseFactory::instance = new GlobalPoseFactory();
				return GlobalPoseFactory::instance;
			}
			NxVec3 newGlobalPosition();

		};
	}
}
#endif
