#include "NodeState.hpp"

#include <Castor3D/Event/Frame/FrameListener.hpp>
#include <Castor3D/Event/Frame/CpuFunctorEvent.hpp>
#include <Castor3D/Scene/SceneNode.hpp>

using namespace castor;
using namespace castor3d;

namespace orastus
{
	namespace main
	{
		namespace
		{
			float getValue( Angle const & p_value )
			{
				return p_value.degrees();
			}

			float getValue( float const & p_value )
			{
				return p_value;
			}

			template< typename T >
			RangedValue< T > updateVelocity( RangedValue< T > p_velocity )
			{
				static T const l_zero{};
				auto l_ret = p_velocity.value() / 1.2f;

				if ( std::abs( getValue( l_ret ) ) < 0.2f )
				{
					l_ret = l_zero;
				}

				return RangedValue< T >( l_ret, p_velocity.range() );
			}
		}

		NodeState::NodeState( FrameListener & p_listener
			, castor3d::SceneNodeRPtr p_node )
			: m_listener{ p_listener }
			, m_node{ p_node }
			, m_originalPosition{ p_node->getPosition() }
			, m_originalOrientation{ p_node->getOrientation() }
		{
		}

		void NodeState::reset( float p_speed )
		{
			setMaxSpeed( p_speed );
			m_angularVelocityX = 0.0_degrees;
			m_angularVelocityY = 0.0_degrees;
			m_angles = Angles{ { 0.0_radians, 0.0_radians } };
			m_scalarVelocityX = 0.0f;
			m_scalarVelocityY = 0.0f;
			m_scalarVelocityZ = 0.0f;

			m_listener.postEvent( makeCpuFunctorEvent( EventType::ePostRender
				, [this]()
				{
					m_node->setOrientation( m_originalOrientation );
					m_node->setPosition( m_originalPosition );
				} ) );
		}

		void NodeState::setMaxSpeed( float p_speed )
		{
			m_angularVelocityX.updateRange( makeRange( Angle::fromDegrees( -p_speed * 2 )
				, Angle::fromDegrees( p_speed * 2 ) ) );
			m_angularVelocityY.updateRange( makeRange( Angle::fromDegrees( -p_speed * 2 )
				, Angle::fromDegrees( p_speed * 2 ) ) );
			m_scalarVelocityX.updateRange( makeRange( -p_speed, p_speed ) );
			m_scalarVelocityY.updateRange( makeRange( -p_speed, p_speed ) );
			m_scalarVelocityZ.updateRange( makeRange( -p_speed, p_speed ) );
		}

		bool NodeState::update()
		{
			auto l_angles = m_angles;
			m_angles[0] += m_angularVelocityX.value();
			m_angles[1] += m_angularVelocityY.value();
			m_angularVelocityX = updateVelocity( m_angularVelocityX );
			m_angularVelocityY = updateVelocity( m_angularVelocityY );
			Point3f l_translate;
			l_translate[0] = m_scalarVelocityX.value();
			l_translate[1] = m_scalarVelocityY.value();
			l_translate[2] = m_scalarVelocityZ.value();
			m_scalarVelocityX = updateVelocity( m_scalarVelocityX );
			m_scalarVelocityY = updateVelocity( m_scalarVelocityY );
			m_scalarVelocityZ = updateVelocity( m_scalarVelocityZ );

			bool l_result{ l_angles[0] != m_angles[0] || l_angles[1] != m_angles[1] };

			if ( l_translate != Point3f{} )
			{
				auto l_orientation = m_node->getOrientation();
				Point3f l_right{ 1.0f, 0.0f, 0.0f };
				Point3f l_up{ 0.0f, 1.0f, 0.0f };
				Point3f l_front{ 0.0f, 0.0f, 1.0f };
				l_orientation.transform( l_right, l_right );
				l_orientation.transform( l_up, l_up );
				l_orientation.transform( l_front, l_front );
				l_translate = ( l_right * l_translate[0] ) + ( l_up * l_translate[1] ) + ( l_front * l_translate[2] );
				l_result = true;
			}

			if ( l_result )
			{
				l_angles = m_angles;
				m_listener.postEvent( makeCpuFunctorEvent( EventType::ePostRender
					, [this, l_translate, l_angles]()
					{
						m_node->translate( l_translate );

						Quaternion l_x{ Quaternion::fromAxisAngle( Point3f{ 1.0, 0.0, 0.0 }, l_angles[0] ) };
						Quaternion l_y{ Quaternion::fromAxisAngle( Point3f{ 0.0, 1.0, 0.0 }, l_angles[1] ) };
						m_node->setOrientation( m_originalOrientation * l_y * l_x );
					} ) );
			}

			return l_result;
		}

		void NodeState::setAngularVelocity( castor::Point2f const & p_value )noexcept
		{
			m_angularVelocityX = castor::Angle::fromDegrees( p_value[0] );
			m_angularVelocityY = castor::Angle::fromDegrees( p_value[1] );
		}

		void NodeState::setScalarVelocity( castor::Point3f const & p_value )noexcept
		{
			m_scalarVelocityX = p_value[0];
			m_scalarVelocityY = p_value[1];
			m_scalarVelocityZ = p_value[2];
		}

		void NodeState::addAngularVelocity( castor::Point2f const & p_value )noexcept
		{
			m_angularVelocityX += castor::Angle::fromDegrees( p_value[0] );
			m_angularVelocityY += castor::Angle::fromDegrees( p_value[1] );
		}

		void NodeState::addScalarVelocity( castor::Point3f const & p_value )noexcept
		{
			m_scalarVelocityX += p_value[0];
			m_scalarVelocityY += p_value[1];
			m_scalarVelocityZ += p_value[2];
		}
	}
}
