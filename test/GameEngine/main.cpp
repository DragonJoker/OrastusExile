/**
See licence file in root folder, MIT.txt
*/
#include "EntityTest.hpp"
#include "ComponentTest.hpp"
#include "ComponentDataTest.hpp"
#include "ResourceTest.hpp"
#include "EcsTest.hpp"
#include "TowerTest.hpp"
#include "EnemyTest.hpp"
#include "BulletTest.hpp"

#include <CastorUtils/Log/Logger.hpp>
#include <CastorUtils/Data/File.hpp>

#include <CastorTest/BenchManager.hpp>

#include <algorithm>

int main( int argc, char * const * const argv )
{
	uint32_t result = 0;
	uint32_t count = 1;

	if ( argc == 2 )
	{
		count = uint32_t( std::max< int >( 1, atoi( argv[2] ) ) );
	}

#if defined( NDEBUG )
	castor::Logger::initialise( castor::LogType::eInfo );
#else
	castor::Logger::initialise( castor::LogType::eDebug );
#endif

	castor::Logger::setFileName( castor::File::getExecutableDirectory() / cuT( "Tests.log" ) );
#if defined( CASTOR_USE_OCL )
	Testing::Register( std::make_unique< Testing::OpenCLBench >() );
#endif
	Testing::registerType( std::make_unique< orastus::test::EntityTest >() );
	Testing::registerType( std::make_unique< orastus::test::ComponentTest >() );
	Testing::registerType( std::make_unique< orastus::test::ComponentDataTest >() );
	Testing::registerType( std::make_unique< orastus::test::ResourceTest >() );
	Testing::registerType( std::make_unique< orastus::test::EcsTest >() );
	Testing::registerType( std::make_unique< orastus::test::TowerTest >() );
	Testing::registerType( std::make_unique< orastus::test::EnemyTest >() );
	Testing::registerType( std::make_unique< orastus::test::BulletTest >() );
	BENCHLOOP( count, result );
	castor::Logger::cleanup();
	return int( result );
}
