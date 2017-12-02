#pragma once

#include <set>
#include <regex>

#include <boost/filesystem/path.hpp>

namespace Utility
{
   struct FileMatcher
   {
      static std::set<boost::filesystem::path> getNonEmptyMatches(boost::filesystem::path root, std::regex filePattern);
   };
}
