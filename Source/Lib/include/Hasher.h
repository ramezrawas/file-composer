#pragma once

#include <string>

#include <boost/filesystem/path.hpp>

namespace Utility
{
   struct Hasher
   {
      static std::string getSHA1(std::string const& string);
      static std::string getSHA1(boost::filesystem::path const& path);
   };   
}
