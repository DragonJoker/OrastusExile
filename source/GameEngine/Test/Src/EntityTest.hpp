/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EOFT_EntityTest_HPP___
#define ___EOFT_EntityTest_HPP___

#include <UnitTest.hpp>

namespace orastus
{
	namespace test
	{
		class EntityTest
			: public Testing::TestCase
		{
		public:
			EntityTest();
			~EntityTest();

		private:
			void DoRegisterTests()override;

		private:
			void Creation();
		};
	}
}

#endif
