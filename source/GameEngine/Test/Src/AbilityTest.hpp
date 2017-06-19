/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EOFT_AbilityTest_HPP___
#define ___EOFT_AbilityTest_HPP___

#include <UnitTest.hpp>

namespace orastus
{
	namespace test
	{
		class AbilityTest
			: public Testing::TestCase
		{
		public:
			AbilityTest();
			~AbilityTest();

		private:
			void DoRegisterTests()override;

		private:
			void AbilityCreation();
			void AbilityComparison();
		};
	}
}

#endif
