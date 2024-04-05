/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_Ecs_HPP___
#define ___EFO_Ecs_HPP___

#include "EntityComponentData.hpp"

#include "GameEngine/System/BulletSystem.hpp"
#include "GameEngine/System/EnemySystem.hpp"
#include "GameEngine/System/SoundSystem.hpp"
#include "GameEngine/System/TargetSystem.hpp"
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
		*\param[in] cell
		*	The block cell.
		*\param[in] geometry
		*	The geometry value.
		*\param[in] pickable
		*	\p true to make the entity pickable.
		*\return
		*	The entity for the created block.
		*/
		EFO_API Entity createMapBlock( GridCell & cell
			, castor3d::GeometryRPtr geometry
			, bool pickable );
		/**
		*\brief
		*	Creates a tower.
		*\param[in] category
		*	The tower category.
		*\param[in] cell
		*	The tower cell.
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
		EFO_API Entity createTower( TowerCategoryPtr category
			, GridCell & cell
			, castor3d::GeometryRPtr geometry
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
			, castor3d::GeometryRPtr geometry
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
			, castor3d::GeometryRPtr geometry
			, WalkDataPtr walkData );
		/**
		*\brief
		*	Creates an enemies target.
		*\param[in] geometry
		*	The geometry value.
		*\param[in] cell
		*	The cell value.
		*/
		EFO_API Entity createTarget( castor3d::GeometryRPtr geometry
			, GridCell cell
			, SoundSource const * targetCapturedSound );
		/**
		*\brief
		*	Resets an enemies target to initial state.
		*\param[in] entity
		*	The target entity.
		*\param[in] cell
		*	The cell value.
		*/
		EFO_API void resetTarget( Entity entity
			, GridCell cell );
		/**
		*\brief
		*	Creates a bullet.
		*\param[in] type
		*	The ammunition type.
		*\param[in] geometry
		*	The geometry value.
		*\param[in] track
		*	The track value.
		*/
		EFO_API Entity createBullet( AmmoType type
			, castor3d::GeometryRPtr geometry
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
		EFO_API static ComponentId const TowerStateComponent;
		EFO_API static ComponentId const EnemyStateComponent;
		EFO_API static ComponentId const TargetStateComponent;
		EFO_API static ComponentId const BulletStateComponent;
		EFO_API static ComponentId const TimeoutComponent;
		EFO_API static ComponentId const GeometryComponent;
		EFO_API static ComponentId const PickableComponent;
		EFO_API static ComponentId const SoundSourceComponent;
		EFO_API static ComponentId const CellComponent;

	private:
		// Components and entities.
		ComponentList m_components;
		EntityList m_entities;
		EntityComponentsMap m_entitiesComponents;
		// Assemblages.
		TargetPtr m_targetSet;
		TowerPtr m_towerSet;
		EnemyPtr m_enemySet;
		BulletPtr m_bulletSet;
		MapBlockPtr m_mapBlockSet;
		// Systems.
		TowerSystem m_towerSystem;
		BulletSystem m_bulletSystem;
		EnemySystem m_enemySystem;
		TargetSystem m_targetSystem;
		SoundSystem m_soundSystem;
	};
}

#include "Ecs.inl"

#endif
