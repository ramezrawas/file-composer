
#include <boost/filesystem/path.hpp>

#include <string>
#include <istream>
#include <memory>

namespace Utility
{
   std::string readFile(boost::filesystem::path const& path);
   
   struct Mapping
   {
      Mapping(boost::filesystem::path const& path);
      ~Mapping();
      
      Mapping(Mapping&&);
      Mapping& operator=(Mapping&&);
      
      Mapping(Mapping const&) = delete;
      Mapping& operator=(Mapping const&) = delete;
      
      boost::filesystem::path const& getPath() const;
      //std::istream& getIStream();
      std::string getContent();
      
   private:
      struct Impl;
      std::unique_ptr<Impl> m_impl;
   };
   
   Mapping mapFile(boost::filesystem::path const& path);
}

