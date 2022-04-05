/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_ImagesLoader_HPP___
#define ___EFO_ImagesLoader_HPP___

#include "ExiledFromOrastusPrerequisites.hpp"

#include <thread>

#include <wx/image.h>

namespace orastus
{
	namespace main
	{
		class ImagesLoader
		{
		public:
			using ImageIdMap = std::map< uint32_t, wxImage * >;
			using thread_sptr = std::shared_ptr< std::thread >;
			using ThreadPtrArray = std::vector< thread_sptr >;

		private:
			static ImageIdMap m_mapImages;
			static std::mutex m_mutex;
			static ThreadPtrArray m_arrayCurrentLoads;

		public:
			ImagesLoader();
			~ImagesLoader();

			static void cleanup();
			static void addBitmap( uint32_t p_id, char const * const * p_pBits );
			static wxImage * getBitmap( uint32_t p_id );
			static void waitAsyncLoads();
		};
	}
}

#endif
