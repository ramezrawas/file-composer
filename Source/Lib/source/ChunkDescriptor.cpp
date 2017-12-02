
#include "../include/ChunkDescriptor.h"
#include "../include/Hasher.h"
#include "../include/Utility.h"

#include <boost/algorithm/string.hpp>

#include <sstream>

namespace Utility
{
   std::string orderByFirstNonEmptyLine(std::string const& content)
   {
      std::istringstream is(content);
      for (std::string line; std::getline(is, line);)
      {
         boost::algorithm::trim(line);
         if (!line.empty()) { return line; }
      }
      return std::string();
   }
     
   std::vector<ChunkDescriptor> ChunkDescriptor::create(
          std::set<boost::filesystem::path> paths
         ,std::function<std::string(std::string const&)> orderStrategy)
   {
      std::vector<ChunkDescriptor> descriptors;
      for (auto const& path : paths)
      {  descriptors.emplace_back(ChunkDescriptor(path, orderStrategy)); }
      return descriptors;
   }
   
   ChunkDescriptor::ChunkDescriptor(boost::filesystem::path const& path, std::function<std::string(std::string const&)> orderStrategy)
      :m_path(path)
      ,m_content(mapFile(getPath()).getContent())
      ,m_contentHash(Utility::Hasher::getSHA1(m_content))
      ,m_orderCriterion(orderStrategy(m_content))
   {}
   
   ChunkDescriptor::ChunkDescriptor(boost::filesystem::path path, std::string content, std::function<std::string(std::string const&)> orderStrategy)
      :m_path(std::move(path))
      ,m_content(std::move(content))
      ,m_contentHash(Utility::Hasher::getSHA1(m_content))
      ,m_orderCriterion(orderStrategy(m_content))
   {}
   
   bool operator<(ChunkDescriptor const& a, ChunkDescriptor const& b)
   {  return a.getOrderCriterion().compare(b.getOrderCriterion()) < 0; }
}
