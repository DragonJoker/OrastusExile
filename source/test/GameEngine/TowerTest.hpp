/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EOFT_TowerTest_HPP___
#define ___EOFT_TowerTest_HPP___

#include "GameEngineTestPrerequisites.hpp"

namespace orastus
{
	namespace test
	{
		class TowerTest
			: public Testing::TestCase
		{
		public:
			TowerTest();
			~TowerTest();

		private:
			void doRegisterTests()override;

		private:
			void Creation();
		};
	}
}

#endif
