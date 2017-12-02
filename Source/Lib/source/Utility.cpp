
#include "../include/Utility.h"

#include <boost/algorithm/string.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/interprocess/managed_mapped_file.hpp>

#include <fstream>
#include <cstddef>
#include <string>
#include <streambuf>

template<typename CharT, typename TraitsT = std::char_traits<CharT>>
struct basic_membuf : public std::basic_streambuf<CharT, TraitsT> 
{
    basic_membuf(CharT const* const buf, std::size_t const size) 
    {
        CharT* const p = const_cast<CharT*>(buf);
        this->setg(p, p, p + size);
    }
};

template<typename CharT, typename TraitsT = std::char_traits<CharT>>
struct basic_imemstream : virtual public basic_membuf<CharT, TraitsT>, public std::basic_istream<CharT, TraitsT> 
{
    basic_imemstream(CharT const* const buf, std::size_t const size)
      :basic_membuf<CharT, TraitsT>(buf, size)
      ,std::basic_istream<CharT, TraitsT>(static_cast<std::basic_streambuf<CharT, TraitsT>*>(this))
    { }
    
    basic_imemstream(basic_imemstream const&) = delete;
    basic_imemstream(basic_imemstream&&) = default;
    basic_imemstream& operator=(basic_imemstream const&) = delete;
    basic_imemstream& operator=(basic_imemstream&&) = default;
};

using imemstream = basic_imemstream<char>;

std::string toString(std::istream& is)
{      
   is.seekg(0, std::ios::end);
   auto const size(is.tellg());
   is.seekg(0, std::ios::beg);
   
   if (size == -1) { throw std::runtime_error("Stream does not support tellg or it has failed"); }
   else
   if (size == 0) { return std::string(); }
   
   std::string content;
   content.reserve(static_cast<size_t>(size) + 1);
   content.assign((std::istreambuf_iterator<char>(is)),
                   std::istreambuf_iterator<char>());
                   
   /** \todo This is ugly here, move it somewhere domain specific
   */
   if (!boost::ends_with(content, "\n")) 
   {  content += '\n'; }
   
   return content;
}

namespace Utility
{  
   std::string readFile(boost::filesystem::path const& path)
   {
      if(boost::filesystem::is_empty(path)) 
      {  return std::string(); }
      
      std::ifstream istream(path.string());
      return toString(istream);
   }
   
   struct Mapping::Impl
   {
      Impl(boost::filesystem::path const& path)
         :m_path(path)
         ,m_mapping(m_path.string().c_str(), boost::interprocess::read_only)
         ,m_region(m_mapping, boost::interprocess::read_only)
         //,m_stream(static_cast<char const*>(m_region.get_address()), m_region.get_size())
      {}
      
      boost::filesystem::path m_path;
      boost::interprocess::file_mapping m_mapping;
      boost::interprocess::mapped_region m_region;
      //imemstream m_stream;
   };
   
   Mapping::Mapping(boost::filesystem::path const& path) : m_impl() 
   {
      if(boost::filesystem::is_empty(path)) 
      {  throw std::invalid_argument("File to memory map cannot be empty: " + path.string()); }
      
      m_impl = std::make_unique<Impl>(path);
   }
   
   Mapping::~Mapping() = default; ///< Keep here to have Impl completely defined
   Mapping::Mapping(Mapping&&) = default;
   Mapping& Mapping::operator=(Mapping&&) = default;
   
   boost::filesystem::path const& Mapping::getPath() const
   {  return m_impl->m_path; }
   
   //std::istream& Mapping::getIStream()
   //{  return m_impl->m_stream; }
   
   std::string Mapping::getContent()
   {       
      std::string content;
      content.reserve(static_cast<size_t>(m_impl->m_region.get_size()) + 1);
      content.assign(static_cast<char const*>(m_impl->m_region.get_address()), m_impl->m_region.get_size());
      
      /** \todo This is ugly here, move it somewhere domain specific
      */
      if (!boost::ends_with(content, "\n")) 
      {  content += '\n'; }
      
      return content;
   }
   
   Mapping mapFile(boost::filesystem::path const& path)
   {  return Mapping(path); }
}
