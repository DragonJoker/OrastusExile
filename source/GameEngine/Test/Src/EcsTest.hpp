/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EOFT_EcsTest_HPP___
#define ___EOFT_EcsTest_HPP___

#include <UnitTest.hpp>

namespace orastus
{
	namespace test
	{
		class EcsTest
			: public Testing::TestCase
		{
		public:
			EcsTest();
			~EcsTest();

		private:
			void DoRegisterTests()override;

		private:
			void Hashing();
		};
	}
}

#endif
