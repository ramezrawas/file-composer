#pragma once

#include "ChunkDescriptor.h"

#include <boost/filesystem/path.hpp>

#include <log4cxx/logger.h>

#include <map>

namespace Utility
{
   struct FileComposite
   {
      static char const s_separator[];
      
      FileComposite(boost::filesystem::path const& path);
      
      void merge(std::vector<Utility::ChunkDescriptor> descriptors);
      
      void writeSorted();
      
   private:
      log4cxx::LoggerPtr m_logger;
      boost::filesystem::path m_path;
      std::map<boost::filesystem::path, std::string> m_chunks;
   };   
}
