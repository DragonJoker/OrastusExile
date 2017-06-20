/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EOFT_BulletTest_HPP___
#define ___EOFT_BulletTest_HPP___

#include <UnitTest.hpp>

namespace orastus
{
	namespace test
	{
		class BulletTest
			: public Testing::TestCase
		{
		public:
			BulletTest();
			~BulletTest();

		private:
			void DoRegisterTests()override;

		private:
			void Creation();
		};
	}
}

#endif
