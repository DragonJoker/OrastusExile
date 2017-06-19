/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EOFT_AbilitySetTest_HPP___
#define ___EOFT_AbilitySetTest_HPP___

#include <UnitTest.hpp>

namespace orastus
{
	namespace test
	{
		class AbilitySetTest
			: public Testing::TestCase
		{
		public:
			AbilitySetTest();
			~AbilitySetTest();

		private:
			void DoRegisterTests()override;

		private:
			void AbilitySetCreation();
		};
	}
}

#endif
