/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EOFT_ComponentTest_HPP___
#define ___EOFT_ComponentTest_HPP___

#include <UnitTest.hpp>

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
			void DoRegisterTests()override;

		private:
			void Creation();
			void Comparison();
		};
	}
}

#endif
