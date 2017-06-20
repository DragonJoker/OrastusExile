/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EOFT_ComponentDataTest_HPP___
#define ___EOFT_ComponentDataTest_HPP___

#include <UnitTest.hpp>

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
			void DoRegisterTests()override;

		private:
			void Creation();
			void Comparison();
		};
	}
}

#endif
