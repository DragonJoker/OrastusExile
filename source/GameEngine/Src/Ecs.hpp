/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_Ecs_HPP___
#define ___EFO_Ecs_HPP___

#include "EntityComponentData.hpp"
#include "Grid.hpp"
#include "Hud.hpp"

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
		*/
		EFO_API void Update();
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
		EFO_API Component const & GetComponent( String const & p_name )const;
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
		*\return
		*	The entity for the created tower.
		*/
		EFO_API Entity CreateTower( Milliseconds const & p_cooldown
			, uint32_t p_damage
			, float p_range
			, float p_bulletSpeed
			, uint32_t p_requiredLevel );
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
		*\return
		*	The entity for the created tower.
		*/
		EFO_API Entity CreateTower( Milliseconds const & p_cooldown
			, uint32_t p_damage
			, float p_range
			, float p_bulletSpeed
			, uint32_t p_splashDamage
			, float p_splashRange
			, uint32_t p_requiredLevel );
		/**
		*\brief
		*	Creates an enemy.
		*\param[in] p_speed
		*	The speed value.
		*\param[in] p_life
		*	The life points.
		*/
		EFO_API Entity CreateEnemy( float p_speed
			, uint32_t p_life );
		/**
		*\brief
		*	Creates a bullet.
		*\param[in] p_speed
		*	The speed value.
		*\param[in] p_damage
		*	The damage value.
		*/
		EFO_API Entity CreateBullet( float p_speed
			, uint32_t p_damage );
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

	private:
		Entity DoCreateEntity( xchar const * const p_name );
		void DoCreateComponent( String const & p_name, String const & p_desc );

	public:
		EFO_API static String const StateComponent;
		EFO_API static String const CooldownComponent;
		EFO_API static String const DamageComponent;
		EFO_API static String const RangeComponent;
		EFO_API static String const SplashDamageComponent;
		EFO_API static String const SplashRangeComponent;
		EFO_API static String const LevelComponent;
		EFO_API static String const SpeedComponent;
		EFO_API static String const LifeComponent;

	private:
		ComponentList m_components;
		EntityList m_entities;
		EntityComponentsList m_entitiesComponents;
		TowerPtr m_towerSet;
		SplashTowerPtr m_splashTowerSet;
		EnemyPtr m_enemySet;
		BulletPtr m_bulletSet;
	};
}

#include "Ecs.inl"

#endif
