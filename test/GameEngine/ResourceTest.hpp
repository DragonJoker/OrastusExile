/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EOFT_ResourceTest_HPP___
#define ___EOFT_ResourceTest_HPP___

#include "GameEngineTestPrerequisites.hpp"

namespace orastus
{
	namespace test
	{
		class ResourceTest
			: public Testing::TestCase
		{
		public:
			ResourceTest();
			~ResourceTest();

		private:
			void doRegisterTests()override;

		private:
			void ResourceCreation();
			void ResourceAddition();
			void ResourceSubtraction();
		};
	}
}

#endif
