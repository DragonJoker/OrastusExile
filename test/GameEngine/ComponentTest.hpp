/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EOFT_ComponentTest_HPP___
#define ___EOFT_ComponentTest_HPP___

#include "GameEngineTestPrerequisites.hpp"

namespace orastus
{
	namespace test
	{
		class ComponentTest
			: public Testing::TestCase
		{
		public:
			ComponentTest();
			~ComponentTest();

		private:
			void doRegisterTests()override;

		private:
			void Creation();
			void Comparison();
		};
	}
}

#endif
