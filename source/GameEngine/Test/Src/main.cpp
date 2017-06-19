/**
See licence file in root folder, MIT.txt
*/
#include "AbilityTest.hpp"
#include "AbilitySetTest.hpp"
#include "ResourceTest.hpp"
#include "TowerTest.hpp"

#include <Log/Logger.hpp>
#include <Data/File.hpp>

#include <BenchManager.hpp>

#include <algorithm>

int main( int argc, char * const * const argv )
{
	int l_result = 0;
	int l_count = 1;

	if ( argc == 2 )
	{
		l_count = std::max< int >( 1, atoi( argv[2] ) );
	}

#if defined( NDEBUG )
	Castor::Logger::Initialise( Castor::LogType::eInfo );
#else
	Castor::Logger::Initialise( Castor::LogType::eDebug );
#endif

	Castor::Logger::SetFileName( Castor::File::GetExecutableDirectory() / cuT( "Tests.log" ) );
#if defined( CASTOR_USE_OCL )
	Testing::Register( std::make_unique< Testing::OpenCLBench >() );
#endif
	Testing::Register( std::make_unique< orastus::test::AbilityTest >() );
	Testing::Register( std::make_unique< orastus::test::AbilitySetTest >() );
	Testing::Register( std::make_unique< orastus::test::ResourceTest >() );
	Testing::Register( std::make_unique< orastus::test::TowerTest >() );
	BENCHLOOP( l_count, l_result );
	Castor::Logger::Cleanup();
	return l_result;
}
