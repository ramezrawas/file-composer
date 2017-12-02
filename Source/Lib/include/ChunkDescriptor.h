#pragma once

#include <string>
#include <set>
#include <functional>

#include <boost/filesystem/path.hpp>

namespace Utility
{
   std::string orderByFirstNonEmptyLine(std::string const& content);
   
   struct ChunkDescriptor
   {
      static std::vector<ChunkDescriptor> create(
          std::set<boost::filesystem::path> paths
         ,std::function<std::string(std::string const&)> orderStrategy);
      
      ChunkDescriptor(boost::filesystem::path const& path, std::function<std::string(std::string const&)> orderStrategy);
      
      ChunkDescriptor(boost::filesystem::path path, std::string content, std::function<std::string(std::string const&)> orderStrategy);
      
      boost::filesystem::path const& getPath() const { return m_path; }
      
      std::string const& getContent() const { return m_content; }
      
      std::string const& getContentHash() const { return m_contentHash; }
      
      std::string const& getOrderCriterion() const { return m_orderCriterion; }
   
   private:
      boost::filesystem::path m_path;
      std::string m_content;
      std::string m_contentHash;
      std::string m_orderCriterion;      
   };
   
   bool operator<(ChunkDescriptor const& a, ChunkDescriptor const& b);   
}
