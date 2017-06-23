/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_Ecs_HPP___
#define ___EFO_Ecs_HPP___

#include "EntityComponentData.hpp"
#include "StateSystem.hpp"

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
			*\param[in] p_entity
			*	The entity ID.
			*\param[in] p_component
			*	The component ID.
			*/
			ComponentDataMatchException( Id const & p_entity, Id const & p_component )
				: std::exception{}
				, m_message{ "No component data for entity [" + ToString( p_entity ) + "] and component [" + ToString( p_component ) + "]" }
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
			*\param[in] p_component
			*	The component name.
			*/
			DuplicateComponentNameException( String const & p_component )
				: std::exception{}
				, m_message{ "Duplicate component name [" + p_component + "]" }
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
		EFO_API void Reset();
		/**
		*\brief
		*	Game loop function.
		*\param[in] p_elapsed
		*	The time elapsed since last call.
		*/
		EFO_API void Update( Milliseconds const & p_elapsed );
		/**
		*\brief
		*	Retrieves a component, given its name.
		*\remarks
		*	If the name doesn't match any existing component, a
		*	ComponentDataMatchException will be thrown.
		*\param[in] p_name
		*	The component name.
		*\return
		*	The component.
		*/
		EFO_API Component const & GetComponent( ComponentId const & p_name )const;
		/**
		*\brief
		*	Creates a map block.
		*\param[in] p_geometry
		*	The geometry value.
		*\return
		*	The entity for the created block.
		*/
		EFO_API Entity CreateMapBlock( Castor3D::GeometrySPtr p_geometry );
		/**
		*\brief
		*	Creates a tower.
		*\param[in] p_cooldown
		*	The cooldown value.
		*\param[in] p_damage
		*	The damage value.
		*\param[in] p_range
		*	The range value.
		*\param[in] p_bulletSpeed
		*	The bullet speed value.
		*\param[in] p_requiredLevel
		*	The required level value.
		*\param[in] p_geometry
		*	The geometry value.
		*\param[in] p_animation
		*	The animation value.
		*\param[in] p_attack
		*	The attack data.
		*\return
		*	The entity for the created tower.
		*/
		EFO_API Entity CreateTower( Milliseconds const & p_cooldown
			, uint32_t p_damage
			, float p_range
			, float p_bulletSpeed
			, uint32_t p_requiredLevel
			, Castor3D::GeometrySPtr p_geometry
			, AnimationDataPtr p_animation
			, TowerAttackDataPtr p_attack );
		/**
		*\brief
		*	Creates a tower with splash damage.
		*\param[in] p_cooldown
		*	The cooldown value.
		*\param[in] p_damage
		*	The damage value.
		*\param[in] p_range
		*	The range value.
		*\param[in] p_bulletSpeed
		*	The bullet speed value.
		*\param[in] p_splashDamage
		*	The splash damage value.
		*\param[in] p_splashRange
		*	The splash range value.
		*\param[in] p_requiredLevel
		*	The required level value.
		*\param[in] p_geometry
		*	The geometry value.
		*\param[in] p_animation
		*	The animation value.
		*\param[in] p_attack
		*	The attack data.
		*\return
		*	The entity for the created tower.
		*/
		EFO_API Entity CreateTower( Milliseconds const & p_cooldown
			, uint32_t p_damage
			, float p_range
			, float p_bulletSpeed
			, uint32_t p_splashDamage
			, float p_splashRange
			, uint32_t p_requiredLevel
			, Castor3D::GeometrySPtr p_geometry
			, AnimationDataPtr p_animation
			, TowerAttackDataPtr p_attack );
		/**
		*\brief
		*	Creates an enemy.
		*\param[in] p_speed
		*	The speed value.
		*\param[in] p_life
		*	The life points.
		*\param[in] p_geometry
		*	The geometry value.
		*\param[in] p_walkData
		*	The wallk data.
		*/
		EFO_API Entity CreateEnemy( float p_speed
			, uint32_t p_life
			, Castor3D::GeometrySPtr p_geometry
			, WalkDataPtr p_walkData );
		/**
		*\brief
		*	Resets an enemy to initial state.
		*\param[in] p_entity
		*	The enemy entity.
		*\param[in] p_speed
		*	The speed value.
		*\param[in] p_life
		*	The life points.
		*\param[in] p_geometry
		*	The geometry value.
		*\param[in] p_walkData
		*	The wallk data.
		*/
		EFO_API void ResetEnemy( Entity p_entity
			, float p_speed
			, uint32_t p_life
			, Castor3D::GeometrySPtr p_geometry
			, WalkDataPtr p_walkData );
		/**
		*\brief
		*	Creates a bullet.
		*\param[in] p_speed
		*	The speed value.
		*\param[in] p_damage
		*	The damage value.
		*\param[in] p_geometry
		*	The geometry value.
		*/
		EFO_API Entity CreateBullet( float p_speed
			, uint32_t p_damage
			, Castor3D::GeometrySPtr p_geometry );
		/**
		*\brief
		*	Resets a bullet to initial state.
		*\param[in] p_entity
		*	The bullet entity.
		*\param[in] p_speed
		*	The speed value.
		*\param[in] p_damage
		*	The damage value.
		*\param[in] p_geometry
		*	The geometry value.
		*/
		EFO_API void ResetBullet( Entity p_entity
			, float p_speed
			, uint32_t p_damage
			, Castor3D::GeometrySPtr p_geometry );
		/**
		*\brief
		*	Prints a map block.
		*\param[in] p_entity
		*	The entity.
		*/
		EFO_API void PrintMapBlock( Entity const & p_entity )const;
		/**
		*\brief
		*	Prints a tower.
		*\param[in] p_entity
		*	The entity.
		*/
		EFO_API void PrintTower( Entity const & p_entity )const;
		/**
		*\brief
		*	Prints a splash tower.
		*\param[in] p_entity
		*	The entity.
		*/
		EFO_API void PrintSplashTower( Entity const & p_entity )const;
		/**
		*\brief
		*	Prints an enemy.
		*\param[in] p_entity
		*	The entity.
		*/
		EFO_API void PrintEnemy( Entity const & p_entity )const;
		/**
		*\brief
		*	Prints a bullet.
		*\param[in] p_entity
		*	The entity.
		*/
		EFO_API void PrintBullet( Entity const & p_entity )const;
		/**
		*\brief
		*	Tells if an entity has given component.
		*\param[in] p_entity
		*	The entity.
		*\param[in] p_component
		*	The component.
		*\return
		*	\p false if it doesn't.
		*/
		EFO_API bool HasComponent( Entity const & p_entity
			, Component const & p_component )const;
		/**
		*\brief
		*	Create an entity component data.
		*\param[in] p_entity
		*	The entity
		*\param[in] p_component
		*	The component.
		*\param[in] p_data
		*	The component data's value.
		*/
		template< typename T >
		inline void CreateComponentData( Entity const & p_entity
			, Component const & p_component
			, T const & p_data );
		/**
		*\brief
		*	Retrieves an entity component data.
		*\remarks
		*	If no entity component data is found, throws a ComponentDataMatchException.\n
		*	In debug mode, if there is a type mismatch, between the created ComponentData
		*	and the wanted type, an assertion is raised.
		*\param[in] p_entity
		*	The entity.
		*\param[in] p_component
		*	The component.
		*\return
		*	The retrieved ComponentData.
		*/
		template< typename T >
		inline ComponentData< T > const & GetComponentData( Entity const & p_entity
			, Component const & p_component )const;
		/**
		*\brief
		*	Retrieves an entity component data.
		*\remarks
		*	If no entity component data is found, throws a ComponentDataMatchException.\n
		*	In debug mode, if there is a type mismatch, between the created ComponentData
		*	and the wanted type, an assertion is raised.
		*\param[in] p_entity
		*	The entity.
		*\param[in] p_component
		*	The component.
		*\return
		*	The retrieved ComponentData.
		*/
		template< typename T >
		inline ComponentData< T > & GetComponentData( Entity const & p_entity
			, Component const & p_component );
		/**
		*\brief
		*	Finds an entity containing the given component data.
		*\param[in] p_component
		*	The component.
		*\param[in] p_data
		*	The component data.
		*\return
		*	The retrieved Entity.
		*/
		template< typename T >
		inline Entity FindEntity( Component const & p_component
			, T const & p_data )const;
		/**
		*\brief
		*	Retrieves all EntityComponentDatas for given component.
		*\param[in] p_component
		*	The component.
		*\return
		*	The list.
		*/
		inline EntityComponentsList const & GetComponentDatas( Component const & p_component )const
		{
			return m_entitiesComponents.at( p_component.GetName() );
		}
		/**
		*\brief
		*	Retrieves all EntityComponentDatas for given component.
		*\param[in] p_component
		*	The component.
		*\return
		*	The list.
		*/
		inline EntityComponentsList & GetComponentDatas( Component const & p_component )
		{
			return m_entitiesComponents.at( p_component.GetName() );
		}
		/**
		*\brief
		*	Hashes a char array to obtain a ComponentId.
		*\param[in] p_value
		*	The array to hash.
		*\return
		*	The ComponentId.
		*/
		static inline ComponentId Hash( std::array< char, 8 > const & p_value );
		/**
		*\brief
		*	Hashes a char array to obtain a ComponentId.
		*\param[in] p_value
		*	The array to hash.
		*\return
		*	The ComponentId.
		*/
		template< size_t N >
		static inline ComponentId Hash( char const ( &p_value )[N] );
		/**
		*\brief
		*	Unhashes ComponentId to obtain a string.
		*\param[in] p_value
		*	The ComponentId.
		*\return
		*	The string.
		*/
		static inline String Unhash( ComponentId p_value );

	private:
		Entity DoCreateEntity( xchar const * const p_name );
		void DoCreateComponent( ComponentId const & p_name, String const & p_desc );
		void DoRegisterComponents();
		void DoCreateAssemblages();

	public:
		EFO_API static ComponentId const StateComponent;
		EFO_API static ComponentId const CooldownComponent;
		EFO_API static ComponentId const DamageComponent;
		EFO_API static ComponentId const RangeComponent;
		EFO_API static ComponentId const SplashDamageComponent;
		EFO_API static ComponentId const SplashRangeComponent;
		EFO_API static ComponentId const LevelComponent;
		EFO_API static ComponentId const SpeedComponent;
		EFO_API static ComponentId const LifeComponent;
		EFO_API static ComponentId const PositionComponent;
		EFO_API static ComponentId const GeometryComponent;
		EFO_API static ComponentId const PickableComponent;
		EFO_API static ComponentId const AnimationComponent;
		EFO_API static ComponentId const WalkComponent;
		EFO_API static ComponentId const AttackComponent;

	private:
		// Components and entities.
		ComponentList m_components;
		EntityList m_entities;
		EntityComponentsMap m_entitiesComponents;
		// Assemblages.
		TowerPtr m_towerSet;
		SplashTowerPtr m_splashTowerSet;
		EnemyPtr m_enemySet;
		BulletPtr m_bulletSet;
		MapBlockPtr m_mapBlockSet;
		// Systems.
		StateSystem m_stateSystem;
	};
}

#include "Ecs.inl"

#endif
