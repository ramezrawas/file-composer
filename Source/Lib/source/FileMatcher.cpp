
#include "../include/FileMatcher.h"

#include <boost/filesystem.hpp>

namespace bfs = boost::filesystem;

namespace Utility
{   
   std::set<bfs::path> FileMatcher::getNonEmptyMatches(bfs::path root, std::regex filePattern)
   {
      std::set<bfs::path> paths;
      for(auto&& entry : bfs::recursive_directory_iterator(root)) 
      {
         auto path(entry.path());
         
         if       (bfs::is_directory(entry)) { continue; }
         else if  (bfs::is_empty(path))      { continue; }
         
         if (std::regex_match(path.string(), filePattern)) { paths.insert(path); }
      }
      return paths;
   }
}
