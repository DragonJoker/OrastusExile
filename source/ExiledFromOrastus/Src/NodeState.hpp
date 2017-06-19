/**
See licence file in root folder, MIT.txt
*/
#ifndef ___EFO_NodeState_HPP___
#define ___EFO_NodeState_HPP___
#pragma once

#include <Castor3DPrerequisites.hpp>
#include <Math/Angle.hpp>
#include <Math/Point.hpp>
#include <Math/Quaternion.hpp>
#include <Math/RangedValue.hpp>

namespace orastus
{
	namespace main
	{
		using Angles = std::array< Castor::Angle, 2u >;
		/**
		*\brief
		*	Classe de gestion des déplacements d'un noeud de scène.
		*/
		class NodeState
		{
		public:
			/**
			*\brief
			*	Définit le noeud affecté par les évènements.
			*\param[in] p_listener
			*	Le listener qui recevra les évènements.
			*\param[in] p_node
			*	Le noeud
			*/
			NodeState( Castor3D::FrameListener & p_listener
				, Castor3D::SceneNodeSPtr p_node );
			/**
			*\brief
			*	Réinitialise l'état.
			*/
			void Reset( float p_speed );
			/**
			*\brief
			*	Met à jour la vitesse maximale (de rotation et translation).
			*/
			void SetMaxSpeed( float p_speed );
			/**
			*\brief
			*	Met à jour l'angle et le zoom en fonction des vitesses.
			*\return
			 * 	\p true s'il y a eu du mouvement.
			*/
			bool Update();
			/**
			*\brief
			*	Définit la vitesse de rotation du noeud.
			*param[in] p_value
			*	La nouvelle valeur.
			*/
			void SetAngularVelocity( Castor::Point2r const & p_value )noexcept;
			/**
			*\brief
			*	Définit la vitesse de translation du noeud.
			*param[in] p_value
			*	La nouvelle valeur.
			*/
			void SetScalarVelocity( Castor::Point3r const & p_value )noexcept;
			/**
			*\brief
			*	Définit la vitesse de rotation du noeud.
			*param[in] p_value
			*	La nouvelle valeur.
			*/
			void AddAngularVelocity( Castor::Point2r const & p_value )noexcept;
			/**
			*\brief
			*	Définit la vitesse de translation du noeud.
			*param[in] p_value
			*	La nouvelle valeur.
			*/
			void AddScalarVelocity( Castor::Point3r const & p_value )noexcept;

		private:
			//! Le listener qui recevra les évènements de déplacement / rotation.
			Castor3D::FrameListener & m_listener;
			//! Le noeud de scène affecté par les évènements.
			Castor3D::SceneNodeSPtr const m_node;
			//! La position originelle du noeud.
			Castor::Point3r const m_originalPosition;
			//! L'orientation originelle du noeud.
			Castor::Quaternion const m_originalOrientation;
			//! La rotation sur les axes X et Y.
			Angles m_angles;
			//! La vitesse de rotation sur l'axe X.
			Castor::RangedValue< Castor::Angle > m_angularVelocityX
			{
				0.0_degrees,
				Castor::make_range( -5.0_degrees, 5.0_degrees )
			};
			//! La vitesse de rotation sur l'axe Y.
			Castor::RangedValue< Castor::Angle > m_angularVelocityY
			{
				0.0_degrees,
				Castor::make_range( -5.0_degrees, 5.0_degrees )
			};
			//! La vitesse de translation sur l'axe X.
			Castor::RangedValue< float > m_scalarVelocityX
			{
				0.0f,
				Castor::make_range( -5.0f, 5.0f )
			};
			//! La vitesse de translation sur l'axe Y.
			Castor::RangedValue< float > m_scalarVelocityY
			{
				0.0f,
				Castor::make_range( -5.0f, 5.0f )
			};
			//! La vitesse de translation sur l'axe Z.
			Castor::RangedValue< float > m_scalarVelocityZ
			{
				0.0f,
				Castor::make_range( -5.0f, 5.0f )
			};
		};
		using NodeStatePtr = std::unique_ptr< NodeState >;
	}
}

#endif
