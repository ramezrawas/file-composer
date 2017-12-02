
#include <boost/filesystem.hpp>

#include <gtest/gtest.h>

#include <fstream>

struct FileTestBase : public ::testing::Test 
{   
   FileTestBase(std::string directoryNamePostFix) 
      :m_uniquePath(boost::filesystem::temp_directory_path() / boost::filesystem::unique_path("%%%%_%%%%_%%%%_%%%%." + directoryNamePostFix))
   {  boost::filesystem::create_directories(m_uniquePath); }
   
   ~FileTestBase()
   {
      boost::system::error_code ec;
      boost::filesystem::remove_all(m_uniquePath, ec);
   }
   
   boost::filesystem::path createEmptyFile(boost::filesystem::path relativePath) const
   {
      return createFile(relativePath, "");
   }
   
   boost::filesystem::path createFile(boost::filesystem::path relativePath, std::string content) const
   {
      auto const path(m_uniquePath / relativePath);
      boost::filesystem::create_directories(path.parent_path());
      std::ofstream ofs(path.string());
      ofs << content;
      return path;
   }
   
   boost::filesystem::path getRootPath() const { return m_uniquePath; }
   
private:
   boost::filesystem::path m_uniquePath;
};
