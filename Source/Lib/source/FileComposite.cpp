
#include "../include/FileComposite.h"
#include "../include/ChunkDescriptor.h"
#include "../include/Hasher.h"
#include "../include/Utility.h"

#include <boost/filesystem.hpp>

#include <fstream>
#include <streambuf>
#include <regex>

namespace Utility
{
   char const FileComposite::s_separator[]{ '#','#','#','F','I','L','E','#','#','#',':',0 };
   
   FileComposite::FileComposite(boost::filesystem::path const& path) 
      :m_logger(log4cxx::Logger::getLogger("FileComposite"))
      ,m_path(path)
      ,m_chunks()
   {
      if (boost::filesystem::exists(m_path))
      {
         LOG4CXX_DEBUG(m_logger, "Start reading existing composed file: " << m_path.string());
         std::string content(Utility::mapFile(m_path).getContent());
         std::regex regex(s_separator);
         std::sregex_token_iterator current(content.begin(), content.end(), regex, -1);
         std::sregex_token_iterator end;
         for (; current != end; ++current)
         {
            std::string const& chunk(*current);
            if (chunk.empty())
            {  continue; }
            
            std::string pathString;
            std::istringstream is(chunk);
            std::getline(is, pathString);
            
            auto const size(chunk.size() - pathString.size() - 1);
            std::string residual(size, ' ');
            is.read(&residual[0], size);
            
            m_chunks[pathString] = residual;
         }
         LOG4CXX_DEBUG(m_logger, "Finished reading composed file: " << m_path.string());
      }
   }
   
   void FileComposite::merge(std::vector<Utility::ChunkDescriptor> descriptors)
   {
      for (auto const& descriptor : descriptors)
      {
         auto entry(m_chunks.find(descriptor.getPath()));
         if (entry == m_chunks.end())
         {
            m_chunks[descriptor.getPath()] = descriptor.getContent();
            continue;
         }
            
         auto& chunk(*entry);
         auto chunkSHA1(Utility::Hasher::getSHA1(chunk.second));
         
         auto const& fileString(descriptor.getContent());
         auto fileSHA1(Utility::Hasher::getSHA1(fileString));
            
         if (chunkSHA1.compare(fileSHA1) != 0)
         {  LOG4CXX_INFO(m_logger, "Differences found in: " << chunk.first); }
         
         chunk.second = fileString; ///< We overwrite here;
      }
   }
   
   void FileComposite::writeSorted()
   {
      std::vector<Utility::ChunkDescriptor> descriptors;
      
      for (auto&& entry : m_chunks)
      {  descriptors.emplace_back(Utility::ChunkDescriptor(std::move(entry.first), std::move(entry.second), Utility::orderByFirstNonEmptyLine)); }
      
      std::sort(descriptors.begin(), descriptors.end());
      std::ofstream destination(m_path.string());
      for (auto const& descriptor : descriptors)
      {        
         destination << s_separator << descriptor.getPath().string() << '\n';
         std::ifstream source(descriptor.getPath().string());      
         for (std::string line; std::getline(source, line);) 
         {  destination << line << '\n'; }
      }
   } 
}
