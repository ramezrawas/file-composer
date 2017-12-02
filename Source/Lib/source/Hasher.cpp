
#include "../include/Hasher.h"

#include <boost/uuid/sha1.hpp>
#include <boost/interprocess/managed_mapped_file.hpp>
#include <boost/filesystem/operations.hpp>

#include <sstream>
#include <iomanip>
#include <stdexcept>

namespace
{
   std::string getSHA1(char const* buffer, size_t size)
   {
      boost::uuids::detail::sha1 sha1;
      sha1.process_bytes(buffer, size);
      unsigned int hash[5] = {0};
      sha1.get_digest(hash);
      std::ostringstream os;
      for (int i = 0; i < 5; i++)
      {  os << std::hex << std::setw(8) << std::setfill('0') << hash[i]; }
      auto result(os.str());
      std::transform(result.begin(), result.end(), result.begin(), ::toupper);
      return result;
   }
}

namespace Utility
{
   std::string Hasher::getSHA1(std::string const& string)
   {
      if (string.empty()) 
      { throw std::invalid_argument("String to retrieve hash value from cannot be empty"); }
      
      return ::getSHA1(string.data(), string.size());
   }

   std::string Hasher::getSHA1(boost::filesystem::path const& path)
   {     
      if(boost::filesystem::is_empty(path)) 
      { throw std::invalid_argument("File to retrieve hash value from cannot be empty: " + path.string()); }
      
      boost::interprocess::file_mapping mapping(path.string().c_str(), boost::interprocess::read_only);
      boost::interprocess::mapped_region region(mapping, boost::interprocess::read_only);
      return ::getSHA1(static_cast<char const*>(region.get_address()), region.get_size());
   }
}
