
#include "Lib/include/FileMatcher.h"

#include "../include/FileTestBase.h"

#include <boost/filesystem.hpp>

#include <gtest/gtest.h>

#include <fstream>

struct FileMatcherTest : FileTestBase
{   
   FileMatcherTest() : FileTestBase("FileMatcherTest")
   {  
      std::vector<boost::filesystem::path> paths;
      paths.emplace_back("A");
      paths.emplace_back("B");
      paths.emplace_back(boost::filesystem::path("B") / "C");
      for (auto const& path : paths)
      {        
         createFile(path / "NonEmpty.cpp", "!!cpp!!\n\n!!cpp!!\n");
         createFile(path / "NonEmpty.txt", "txt\ntxt\n");
         createEmptyFile(path / "Empty.cpp");
      }
      createFile(boost::filesystem::path("C") / "D" / "Different.txt", "txt\ntxt\n");
   }
};

TEST_F( FileMatcherTest, MatchingNonEmptyCpp )
{
   auto paths(Utility::FileMatcher::getNonEmptyMatches(getRootPath(), std::regex(".*[.]cpp$")));
   EXPECT_EQ((size_t)3, paths.size());
   EXPECT_EQ((size_t)1 , paths.count(getRootPath() / "A" / "NonEmpty.cpp"));
   EXPECT_EQ((size_t)1 , paths.count(getRootPath() / "B" / "NonEmpty.cpp"));
   EXPECT_EQ((size_t)1 , paths.count(getRootPath() / "B" / "C" / "NonEmpty.cpp"));
}

TEST_F( FileMatcherTest, MatchingNonEmptyTxt )
{
   auto paths(Utility::FileMatcher::getNonEmptyMatches(getRootPath(), std::regex(".*[.]txt$")));
   EXPECT_EQ((size_t)4, paths.size());
   EXPECT_EQ((size_t)1 , paths.count(getRootPath() / "A" / "NonEmpty.txt"));
   EXPECT_EQ((size_t)1 , paths.count(getRootPath() / "B" / "NonEmpty.txt"));
   EXPECT_EQ((size_t)1 , paths.count(getRootPath() / "B" / "C" / "NonEmpty.txt"));
   EXPECT_EQ((size_t)1 , paths.count(getRootPath() / "C" / "D" / "Different.txt"));
}

TEST_F( FileMatcherTest, NotMatching )
{
   auto paths(Utility::FileMatcher::getNonEmptyMatches(getRootPath(), std::regex(".*[.]blub$")));
   EXPECT_EQ((size_t)0, paths.size());
}
