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
			float getValue( Angle const & value )
			{
				return value.degrees();
			}

			float getValue( float const & value )
			{
				return value;
			}

			template< typename T >
			RangedValue< T > updateVelocity( RangedValue< T > velocity )
			{
				static T const zero{};
				auto ret = velocity.value() / 1.2f;

				if ( std::abs( getValue( ret ) ) < 0.2f )
				{
					ret = zero;
				}

				return RangedValue< T >( ret, velocity.range() );
			}
		}

		NodeState::NodeState( FrameListener & listener
			, castor3d::SceneNodeRPtr node )
			: m_listener{ listener }
			, m_node{ node }
			, m_originalPosition{ node->getPosition() }
			, m_originalOrientation{ node->getOrientation() }
		{
		}

		void NodeState::reset( float speed )
		{
			setMaxSpeed( speed );
			m_angularVelocityX = 0.0_degrees;
			m_angularVelocityY = 0.0_degrees;
			m_angles = Angles{ { 0.0_radians, 0.0_radians } };
			m_scalarVelocityX = 0.0f;
			m_scalarVelocityY = 0.0f;
			m_scalarVelocityZ = 0.0f;

			m_listener.postEvent( makeCpuFunctorEvent( CpuEventType::ePostCpuStep
				, [this]()
				{
					m_node->setOrientation( m_originalOrientation );
					m_node->setPosition( m_originalPosition );
				} ) );
		}

		void NodeState::setMaxSpeed( float speed )
		{
			m_angularVelocityX.updateRange( makeRange( Angle::fromDegrees( -speed * 2 )
				, Angle::fromDegrees( speed * 2 ) ) );
			m_angularVelocityY.updateRange( makeRange( Angle::fromDegrees( -speed * 2 )
				, Angle::fromDegrees( speed * 2 ) ) );
			m_scalarVelocityX.updateRange( makeRange( -speed, speed ) );
			m_scalarVelocityY.updateRange( makeRange( -speed, speed ) );
			m_scalarVelocityZ.updateRange( makeRange( -speed, speed ) );
		}

		bool NodeState::update()
		{
			auto angles = m_angles;
			m_angles[0] += m_angularVelocityX.value();
			m_angles[1] += m_angularVelocityY.value();
			m_angularVelocityX = updateVelocity( m_angularVelocityX );
			m_angularVelocityY = updateVelocity( m_angularVelocityY );
			Point3f translate;
			translate[0] = m_scalarVelocityX.value();
			translate[1] = m_scalarVelocityY.value();
			translate[2] = m_scalarVelocityZ.value();
			m_scalarVelocityX = updateVelocity( m_scalarVelocityX );
			m_scalarVelocityY = updateVelocity( m_scalarVelocityY );
			m_scalarVelocityZ = updateVelocity( m_scalarVelocityZ );

			bool result{ angles[0] != m_angles[0] || angles[1] != m_angles[1] };

			if ( translate != Point3f{} )
			{
				auto orientation = m_node->getOrientation();
				Point3f right{ 1.0f, 0.0f, 0.0f };
				Point3f up{ 0.0f, 1.0f, 0.0f };
				Point3f front{ 0.0f, 0.0f, 1.0f };
				orientation.transform( right, right );
				orientation.transform( up, up );
				orientation.transform( front, front );
				translate = ( right * translate[0] ) + ( up * translate[1] ) + ( front * translate[2] );
				result = true;
			}

			if ( result )
			{
				angles = m_angles;
				m_listener.postEvent( makeCpuFunctorEvent( CpuEventType::ePostCpuStep
					, [this, translate, angles]()
					{
						m_node->translate( translate );

						Quaternion x{ Quaternion::fromAxisAngle( Point3f{ 1.0, 0.0, 0.0 }, angles[0] ) };
						Quaternion y{ Quaternion::fromAxisAngle( Point3f{ 0.0, 1.0, 0.0 }, angles[1] ) };
						m_node->setOrientation( m_originalOrientation * y * x );
					} ) );
			}

			return result;
		}

		void NodeState::setAngularVelocity( castor::Point2f const & value )noexcept
		{
			m_angularVelocityX = castor::Angle::fromDegrees( value[0] );
			m_angularVelocityY = castor::Angle::fromDegrees( value[1] );
		}

		void NodeState::setScalarVelocity( castor::Point3f const & value )noexcept
		{
			m_scalarVelocityX = value[0];
			m_scalarVelocityY = value[1];
			m_scalarVelocityZ = value[2];
		}

		void NodeState::addAngularVelocity( castor::Point2f const & value )noexcept
		{
			m_angularVelocityX += castor::Angle::fromDegrees( value[0] );
			m_angularVelocityY += castor::Angle::fromDegrees( value[1] );
		}

		void NodeState::addScalarVelocity( castor::Point3f const & value )noexcept
		{
			m_scalarVelocityX += value[0];
			m_scalarVelocityY += value[1];
			m_scalarVelocityZ += value[2];
		}
	}
}
