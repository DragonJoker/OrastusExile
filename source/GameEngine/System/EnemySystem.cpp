#include "GameEngine/System/EnemySystem.hpp"

#include "GameEngine/Game.hpp"
#include "GameEngine/ECS/Ecs.hpp"
#include "GameEngine/ECS/Enemy.hpp"
#include "GameEngine/ECS/SoundSource.hpp"
#include "GameEngine/ECS/Target.hpp"

#include <Castor3D/Scene/Geometry.hpp>
#include <Castor3D/Scene/SceneNode.hpp>

namespace orastus
{
	String getName( EnemyStatus state )
	{
		switch ( state )
		{
		case EnemyStatus::eSpawning:
			return cuT( "Spawning" );
		case EnemyStatus::eWalking:
			return cuT( "Walking" );
		case EnemyStatus::eFrozen:
			return cuT( "Frozen" );
		case EnemyStatus::eSlown:
			return cuT( "Slown" );
		case EnemyStatus::eDying:
			return cuT( "Dying" );
		case EnemyStatus::eDead:
			return cuT( "Dead" );
		case EnemyStatus::eCapturing:
			return cuT( "Capturing" );
		case EnemyStatus::eEscaping:
			return cuT( "Escaping" );
		case EnemyStatus::eEscaped:
			return cuT( "Escaped" );
		default:
			CU_Failure( "Unexpected enemy state." );
			return "Unknown";
		}
	}

	namespace enemy
	{
		bool spawn( Ecs & ecs
			, Milliseconds const & elapsed
			, EnemyData & enemy )
		{
			auto & node = *Game::getEnemyNode( enemy.geometry );
			auto const angle = castor::Angle::fromDegrees( -float( elapsed.count() ) * 240 / 1000.0f );
			node.yaw( angle );
			auto distance = float( elapsed.count() ) * enemy.speed / 1000.0f;
			castor::Point3f nextPosition = enemy.walkData->destination;
			castor::Point3f position{ node.getPosition() };
			castor::Point3f direction{ nextPosition - position };
			auto distanceToDst = float( castor::point::length( direction ) );
			direction[1] *= distance / distanceToDst;
			bool result{ distanceToDst <= distance };

			if ( !result )
			{
				// not at destination
				nextPosition = position + direction;
			}
			else
			{
				enemy.status = EnemyStatus::eWalking;
			}

			node.setPosition( nextPosition );
			return result;
		}

		bool wait( Game & game
			, Ecs & ecs
			, EnemyData & enemy )
		{
			auto result = game.selectTarget();

			if ( result )
			{
				auto & data = ecs.getComponentData< TargetData >( result
					, ecs.getComponent( Ecs::TargetStateComponent ) ).getValue();
				enemy.walkData->destination = game.convert( castor::Point2i{ data.cell.x, data.cell.y } );
				enemy.status = EnemyStatus::eSelecting;
				enemy.target = result;
			}
			else
			{
				enemy.status = EnemyStatus::eWaiting;
			}

			return bool( result );
		}

		bool walk( Game & game
			, Ecs & ecs
			, Milliseconds const & elapsed
			, EnemyData & enemy )
		{
			auto & node = *Game::getEnemyNode( enemy.geometry );
			auto const angle = castor::Angle::fromDegrees( -float( elapsed.count() ) * 240 / 1000.0f );
			node.yaw( angle );
			bool result = false;

			if ( enemy.status == EnemyStatus::eWaiting )
			{
				result = wait( game, ecs, enemy );
			}
			else
			{
				auto & data = *enemy.walkData;
				auto distance = float( elapsed.count() ) * enemy.speed / 1000.0f;
				castor::Point3f nextPosition = data.destination;
				castor::Point3f position{ node.getPosition() };
				castor::Point3f direction{ nextPosition - position };
				auto distanceToDst = float( castor::point::length( direction ) );
				direction[0] *= distance / distanceToDst;
				direction[2] *= distance / distanceToDst;
				bool reachDst{ distanceToDst <= distance };

				if ( !reachDst )
				{
					// not at destination
					nextPosition = position + direction;
				}
				else if ( distanceToDst < distance )
				{
					// after destination => start walking to next destination
					++data.current;

					if ( data.current != data.path.end() )
					{
						data.destination = game.convert( castor::Point2i{ data.current->x, data.current->y } )
							+ castor::Point3f{ 0, game.getCellHeight() / 2.0f, 0 };
						result = walk( game
							, ecs
							, elapsed
							, enemy );
					}
					else
					{
						result = wait( game
							, ecs
							, enemy );
					}
				}
				else
				{
					// Right on the destination node.
					++data.current;
					data.destination = game.convert( castor::Point2i{ data.current->x, data.current->y } )
						+ castor::Point3f{ 0, game.getCellHeight() / 2.0f, 0 };
				}

				if ( !result )
				{
					node.setPosition( nextPosition );
				}
			}

			return result;
		}

		bool select( Game & game
			, Ecs & ecs
			, Milliseconds const & elapsed
			, EnemyData & enemy )
		{
			auto & node = *Game::getEnemyNode( enemy.geometry );
			auto const angle = castor::Angle::fromDegrees( -float( elapsed.count() ) * 240 / 1000.0f );
			node.yaw( angle );
			auto distance = float( elapsed.count() ) * ( enemy.speed / 2.0f ) / 1000.0f;
			castor::Point3f nextPosition = enemy.walkData->destination
				+ castor::Point3f{ 0, game.getCellHeight() * 2.0f, 0 };
			castor::Point3f position{ node.getPosition() };
			castor::Point3f direction{ nextPosition - position };
			auto distanceToDst = float( castor::point::length( direction ) );
			direction[0] *= distance / distanceToDst;
			direction[1] *= distance / distanceToDst;
			direction[2] *= distance / distanceToDst;
			bool result{ distanceToDst <= distance };

			if ( !result )
			{
				// not at destination
				nextPosition = position + direction;
			}
			else
			{
				enemy.status = EnemyStatus::eCapturing;
				game.enemyCapturing( enemy );
			}

			node.setPosition( nextPosition );
			return result;
		}

		bool capture( Game & game
			, Ecs & ecs
			, Milliseconds const & elapsed
			, EnemyData & enemy )
		{
			auto & node = *Game::getEnemyNode( enemy.geometry );
			auto const angle = castor::Angle::fromDegrees( -float( elapsed.count() ) * 60 / 1000.0f );
			node.yaw( angle );
			auto time = enemy.timeout;
			auto result = time >= 2000_ms;

			if ( !result )
			{
				enemy.timeout = time + elapsed;
			}
			else
			{
				enemy.status = EnemyStatus::eEscaping;
				game.enemyEscaping( enemy );
			}

			return result;
		}

		bool escape( Game & game
			, Ecs & ecs
			, Milliseconds const & elapsed
			, EnemyData & enemy )
		{
			auto & node = *Game::getEnemyNode( enemy.geometry );
			auto const angle = castor::Angle::fromDegrees( -float( elapsed.count() ) * 240 / 1000.0f );
			node.yaw( angle );
			auto distance = float( elapsed.count() ) * enemy.speed / 1000.0f;
			castor::Point3f nextPosition = enemy.walkData->destination
				+ castor::Point3f{ 0, game.getCellHeight() * 5.0f, 0 };
			castor::Point3f position{ node.getPosition() };
			castor::Point3f direction{ nextPosition - position };
			auto distanceToDst = float( castor::point::length( direction ) );
			direction[1] *= distance / distanceToDst;
			bool result{ distanceToDst <= distance };

			if ( !result )
			{
				// not at destination
				node.setPosition( position + direction );
			}
			else
			{
				enemy.status = EnemyStatus::eEscaped;
				game.enemyEscaped( enemy );
			}

			return result;
		}
	}

	EnemySystem::EnemySystem( Ecs & ecs )
		: m_ecs{ ecs }
	{
	}

	void EnemySystem::update( Game & game
		, Milliseconds const & elapsed )
	{
		auto & states = m_ecs.getComponentDatas( m_ecs.getComponent( Ecs::EnemyStateComponent ) );

		for ( auto & data : states )
		{
			if ( data.data )
			{
				auto & enemy = static_cast< ComponentData< EnemyData > & >( *data.data ).getValue();

				if ( !enemy.life )
				{
					enemy.status = EnemyStatus::eDead;
				}

				switch ( enemy.status )
				{
				case EnemyStatus::eSpawning:
					enemy::spawn( m_ecs, elapsed, enemy );
					break;
				case EnemyStatus::eWalking:
					enemy::walk( game, m_ecs, elapsed, enemy );
					break;
				case EnemyStatus::eFrozen:
					break;
				case EnemyStatus::eSlown:
					enemy::walk( game, m_ecs, elapsed, enemy );
					break;
				case EnemyStatus::eDying:
					break;
				case EnemyStatus::eDead:
					break;
				case EnemyStatus::eSelecting:
					enemy::select( game, m_ecs, elapsed, enemy );
					break;
				case EnemyStatus::eWaiting:
					enemy::wait( game, m_ecs, enemy );
					break;
				case EnemyStatus::eCapturing:
					enemy::capture( game, m_ecs, elapsed, enemy );
					break;
				case EnemyStatus::eEscaping:
					enemy::escape( game, m_ecs, elapsed, enemy );
					break;
				case EnemyStatus::eEscaped:
					break;
				default:
					break;
				}
			}
		}
	}
}
