/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EOFT_EnemyTest_HPP___
#define ___EOFT_EnemyTest_HPP___

#include <UnitTest.hpp>

namespace orastus
{
	namespace test
	{
		class EnemyTest
			: public Testing::TestCase
		{
		public:
			EnemyTest();
			~EnemyTest();

		private:
			void DoRegisterTests()override;

		private:
			void Creation();
		};
	}
}

#endif
