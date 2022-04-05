/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EOFT_ComponentDataTest_HPP___
#define ___EOFT_ComponentDataTest_HPP___

#include "GameEngineTestPrerequisites.hpp"

namespace orastus
{
	namespace test
	{
		class ComponentDataTest
			: public Testing::TestCase
		{
		public:
			ComponentDataTest();
			~ComponentDataTest();

		private:
			void doRegisterTests()override;

		private:
			void Creation();
			void Comparison();
		};
	}
}

#endif
