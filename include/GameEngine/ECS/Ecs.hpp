/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_Ecs_HPP___
#define ___EFO_Ecs_HPP___

#include "EntityComponentData.hpp"

#include "GameEngine/System/EnemySystem.hpp"
#include "GameEngine/System/StateSystem.hpp"
#include "GameEngine/System/SoundSystem.hpp"
#include "GameEngine/System/TowerSystem.hpp"

namespace orastus
{
	/**
	*\brief
	*	The Entity Component System management class.
	*/
	class Ecs
	{
	public:
		/**
		*\brief
		*	An exception thrown when no component data is found for given entity and component.
		*/
		class ComponentDataMatchException
			: public std::exception
		{
		public:
			/**
			*\brief
			*	Constructor, builds the message.
			*\param[in] entity
			*	The entity ID.
			*\param[in] component
			*	The component ID.
			*/
			ComponentDataMatchException( Id const & entity, Id const & component )
				: std::exception{}
				, m_message{ "No component data for entity [" + toString( entity ) + "] and component [" + toString( component ) + "]" }
			{
			}
			/**
			*\return
			*	The exception message.
			*/
			inline char const * what()const override
			{
				return m_message.c_str();
			}

		private:
			std::string m_message;
		};
		/**
		*\brief
		*	An exception thrown when trying to add a component with the same name as an existing one.
		*/
		class DuplicateComponentNameException
			: public std::exception
		{
		public:
			/**
			*\brief
			*	Constructor, builds the message.
			*\param[in] component
			*	The component name.
			*/
			DuplicateComponentNameException( String const & component )
				: std::exception{}
				, m_message{ "Duplicate component name [" + component + "]" }
			{
			}
			/**
			*\return
			*	The exception message.
			*/
			inline char const * what()const override
			{
				return m_message.c_str();
			}

		private:
			std::string m_message;
		};

	public:
		/**
		*\brief
		*	Constructor, initialises the base components.
		*/
		EFO_API Ecs();
		/**
		*\brief
		*	Resets the game so a new game can be played.
		*/
		EFO_API void reset();
		/**
		*\brief
		*	Game loop function.
		*\param[in] game
		*	The game instance.
		*\param[in] elapsed
		*	The time elapsed since last call.
		*/
		EFO_API void update( Game & game
			, Milliseconds const & elapsed );
		/**
		*\brief
		*	Retrieves a component, given its name.
		*\remarks
		*	If the name doesn't match any existing component, a
		*	ComponentDataMatchException will be thrown.
		*\param[in] name
		*	The component name.
		*\return
		*	The component.
		*/
		EFO_API Component const & getComponent( ComponentId const & name )const;
		/**
		*\brief
		*	Creates a map block.
		*\param[in] geometry
		*	The geometry value.
		*\param[in] pickable
		*	\p true to make the entity pickable.
		*\return
		*	The entity for the created block.
		*/
		EFO_API Entity createMapBlock( castor3d::GeometrySPtr geometry
			, bool pickable );
		/**
		*\brief
		*	Creates a tower.
		*\param[in] type
		*	The tower type.
		*\param[in] cooldown
		*	The cooldown value.
		*\param[in] damage
		*	The damage value.
		*\param[in] range
		*	The range value.
		*\param[in] bulletSpeed
		*	The bullet speed value.
		*\param[in] geometry
		*	The geometry value.
		*\param[in] animation
		*	The animation value.
		*\param[in] attack
		*	The attack data.
		*\param[in] shootSound
		*	The shoot sound source.
		*\return
		*	The entity for the created tower.
		*/
		EFO_API Entity createTower( TowerType type
			, Milliseconds const & cooldown
			, uint32_t damage
			, float range
			, float bulletSpeed
			, castor3d::GeometrySPtr geometry
			, AnimationDataPtr animation
			, AttackDataPtr attack
			, SoundSource const * shootSound );
		/**
		*\brief
		*	Creates a tower with splash damage.
		*\param[in] type
		*	The tower type.
		*\param[in] cooldown
		*	The cooldown value.
		*\param[in] damage
		*	The damage value.
		*\param[in] range
		*	The range value.
		*\param[in] bulletSpeed
		*	The bullet speed value.
		*\param[in] splashDamage
		*	The splash damage value.
		*\param[in] splashRange
		*	The splash range value.
		*\param[in] geometry
		*	The geometry value.
		*\param[in] animation
		*	The animation value.
		*\param[in] attack
		*	The attack data.
		*\param[in] shootSound
		*	The shoot sound source.
		*\return
		*	The entity for the created tower.
		*/
		EFO_API Entity createTower( TowerType type
			, Milliseconds const & cooldown
			, uint32_t damage
			, float range
			, float bulletSpeed
			, uint32_t splashDamage
			, float splashRange
			, castor3d::GeometrySPtr geometry
			, AnimationDataPtr animation
			, AttackDataPtr attack
			, SoundSource const * shootSound );
		/**
		*\brief
		*	Creates an enemy.
		*\param[in] speed
		*	The speed value.
		*\param[in] life
		*	The life points.
		*\param[in] geometry
		*	The geometry value.
		*\param[in] walkData
		*	The wallk data.
		*/
		EFO_API Entity createEnemy( float speed
			, uint32_t life
			, castor3d::GeometrySPtr geometry
			, WalkDataPtr walkData
			, SoundSource const * killSound );
		/**
		*\brief
		*	Resets an enemy to initial state.
		*\param[in] entity
		*	The enemy entity.
		*\param[in] speed
		*	The speed value.
		*\param[in] life
		*	The life points.
		*\param[in] geometry
		*	The geometry value.
		*\param[in] walkData
		*	The wallk data.
		*/
		EFO_API void resetEnemy( Entity entity
			, float speed
			, uint32_t life
			, castor3d::GeometrySPtr geometry
			, WalkDataPtr walkData );
		/**
		*\brief
		*	Creates an enemies target.
		*\param[in] geometry
		*	The geometry value.
		*\param[in] cell
		*	The cell value.
		*/
		EFO_API Entity createTarget( castor3d::GeometrySPtr geometry
			, GridCell cell
			, SoundSource const * targetCapturedSound );
		/**
		*\brief
		*	Resets an enemies target to initial state.
		*\param[in] entity
		*	The target entity.
		*\param[in] geometry
		*	The geometry value.
		*\param[in] cell
		*	The cell value.
		*/
		EFO_API void resetTarget( Entity entity
			, castor3d::GeometrySPtr geometry
			, GridCell cell );
		/**
		*\brief
		*	Creates a bullet.
		*\param[in] geometry
		*	The geometry value.
		*\param[in] track
		*	The track value.
		*/
		EFO_API Entity createBullet( castor3d::GeometrySPtr geometry
			, SoundSource const * soundSource
			, TrackDataPtr track );
		/**
		*\brief
		*	Resets a bullet to initial state.
		*\param[in] entity
		*	The bullet entity.
		*\param[in] geometry
		*	The geometry value.
		*\param[in] track
		*	The track value.
		*/
		EFO_API void resetBullet( Entity entity
			, castor3d::GeometrySPtr geometry
			, SoundSource const * soundSource
			, TrackDataPtr track );
		/**
		*\brief
		*	Prints a map block.
		*\param[in] entity
		*	The entity.
		*/
		EFO_API void printMapBlock( Entity const & entity )const;
		/**
		*\brief
		*	Prints a tower.
		*\param[in] entity
		*	The entity.
		*/
		EFO_API void printTower( Entity const & entity )const;
		/**
		*\brief
		*	Prints a splash tower.
		*\param[in] entity
		*	The entity.
		*/
		EFO_API void printSplashTower( Entity const & entity )const;
		/**
		*\brief
		*	Prints an enemy.
		*\param[in] entity
		*	The entity.
		*/
		EFO_API void printEnemy( Entity const & entity )const;
		/**
		*\brief
		*	Prints a bullet.
		*\param[in] entity
		*	The entity.
		*/
		EFO_API void printBullet( Entity const & entity )const;
		/**
		*\brief
		*	Tells if an entity has given component.
		*\param[in] entity
		*	The entity.
		*\param[in] component
		*	The component.
		*\return
		*	\p false if it doesn't.
		*/
		EFO_API bool hasComponent( Entity const & entity
			, Component const & component )const;
		/**
		*\brief
		*	Create an entity component data.
		*\param[in] entity
		*	The entity
		*\param[in] component
		*	The component.
		*\param[in] data
		*	The component data's value.
		*/
		template< typename T >
		inline void createComponentData( Entity const & entity
			, Component const & component
			, T data );
		/**
		*\brief
		*	Retrieves an entity component data.
		*\remarks
		*	If no entity component data is found, throws a ComponentDataMatchException.\n
		*	In debug mode, if there is a type mismatch, between the created ComponentData
		*	and the wanted type, an assertion is raised.
		*\param[in] entity
		*	The entity.
		*\param[in] component
		*	The component.
		*\return
		*	The retrieved ComponentData.
		*/
		template< typename T >
		inline ComponentData< T > const & getComponentData( Entity const & entity
			, Component const & component )const;
		/**
		*\brief
		*	Retrieves an entity component data.
		*\remarks
		*	If no entity component data is found, throws a ComponentDataMatchException.\n
		*	In debug mode, if there is a type mismatch, between the created ComponentData
		*	and the wanted type, an assertion is raised.
		*\param[in] entity
		*	The entity.
		*\param[in] component
		*	The component.
		*\return
		*	The retrieved ComponentData.
		*/
		template< typename T >
		inline ComponentData< T > & getComponentData( Entity const & entity
			, Component const & component );
		/**
		*\brief
		*	Finds an entity containing the given component data.
		*\param[in] component
		*	The component.
		*\param[in] data
		*	The component data.
		*\return
		*	The retrieved Entity.
		*/
		template< typename T >
		inline Entity findEntity( Component const & component
			, T const & data )const;
		/**
		*\brief
		*	Retrieves all EntityComponentDatas for given component.
		*\param[in] component
		*	The component.
		*\return
		*	The list.
		*/
		inline EntityComponentsList const & getComponentDatas( Component const & component )const
		{
			return m_entitiesComponents.at( component.getName() );
		}
		/**
		*\brief
		*	Retrieves all EntityComponentDatas for given component.
		*\param[in] component
		*	The component.
		*\return
		*	The list.
		*/
		inline EntityComponentsList & getComponentDatas( Component const & component )
		{
			return m_entitiesComponents.at( component.getName() );
		}
		/**
		*\brief
		*	Hashes a char array to obtain a ComponentId.
		*\param[in] value
		*	The array to hash.
		*\return
		*	The ComponentId.
		*/
		static inline ComponentId hash( std::array< char, 8 > const & value );
		/**
		*\brief
		*	Hashes a char array to obtain a ComponentId.
		*\param[in] value
		*	The array to hash.
		*\return
		*	The ComponentId.
		*/
		template< size_t N >
		static inline ComponentId hash( char const ( &value )[N] );
		/**
		*\brief
		*	Unhashes ComponentId to obtain a string.
		*\param[in] value
		*	The ComponentId.
		*\return
		*	The string.
		*/
		static inline String unhash( ComponentId value );

	private:
		Entity doCreateEntity( xchar const * const name );
		void doCreateComponent( ComponentId const & name, String const & desc );
		void doRegisterComponents();
		void doCreateAssemblages();

	public:
		EFO_API static ComponentId const StateComponent;
		EFO_API static ComponentId const TowerStateComponent;
		EFO_API static ComponentId const SplashTowerStateComponent;
		EFO_API static ComponentId const EnemyStateComponent;
		EFO_API static ComponentId const StatusComponent;
		EFO_API static ComponentId const TimeoutComponent;
		EFO_API static ComponentId const CellComponent;
		EFO_API static ComponentId const GeometryComponent;
		EFO_API static ComponentId const PickableComponent;
		EFO_API static ComponentId const TrackComponent;
		EFO_API static ComponentId const SoundSourceComponent;

	private:
		// Components and entities.
		ComponentList m_components;
		EntityList m_entities;
		EntityComponentsMap m_entitiesComponents;
		// Assemblages.
		TargetPtr m_targetSet;
		TowerPtr m_towerSet;
		SplashTowerPtr m_splashTowerSet;
		EnemyPtr m_enemySet;
		BulletPtr m_bulletSet;
		MapBlockPtr m_mapBlockSet;
		// Systems.
		TowerSystem m_towerSystem;
		EnemySystem m_enemySystem;
		StateSystem m_stateSystem;
		SoundSystem m_soundSystem;
	};
}

#include "Ecs.inl"

#endif
