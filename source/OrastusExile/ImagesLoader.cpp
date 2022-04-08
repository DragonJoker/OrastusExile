#include "ImagesLoader.hpp"

using namespace castor;

namespace orastus
{
	namespace main
	{
		ImagesLoader::ImageIdMap ImagesLoader::m_mapImages;
		std::mutex ImagesLoader::m_mutex;
		ImagesLoader::ThreadPtrArray ImagesLoader::m_arrayCurrentLoads;

		ImagesLoader::ImagesLoader()
		{
		}

		ImagesLoader::~ImagesLoader()
		{
			cleanup();
		}

		void ImagesLoader::cleanup()
		{
			waitAsyncLoads();
			m_mutex.lock();

			for ( auto pair : m_mapImages )
			{
				delete pair.second;
			}

			m_mapImages.clear();
			m_mutex.unlock();
		}

		wxImage * ImagesLoader::getBitmap( uint32_t id )
		{
			wxImage * result = nullptr;
			m_mutex.lock();
			auto it = m_mapImages.find( id );
			auto end = m_mapImages.end();

			if ( it != end )
			{
				result = it->second;
			}

			m_mutex.unlock();
			return result;
		}

		void ImagesLoader::addBitmap( uint32_t id, char const * const * bits )
		{
			m_mutex.lock();
			auto it = m_mapImages.find( id );
			auto end = m_mapImages.end();
			m_mutex.unlock();

			if ( it == end )
			{
				thread_sptr threadLoad = std::make_shared< std::thread >( [bits, id]()
				{
					wxImage * image = new wxImage;
					image->Create( bits );
					m_mutex.lock();
					m_mapImages.insert( std::make_pair( id, image ) );
					m_mutex.unlock();
				} );
				m_arrayCurrentLoads.push_back( threadLoad );
			}
		}

		void ImagesLoader::waitAsyncLoads()
		{
			for ( auto thread : m_arrayCurrentLoads )
			{
				thread->join();
				thread.reset();
			}

			m_arrayCurrentLoads.clear();
		}
	}
}
