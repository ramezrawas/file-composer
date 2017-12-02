
#include "Lib/include/Utility.h"

#include "../include/FileTestBase.h"

#include <gtest/gtest.h>

#include <fstream>

struct FileReaderTest : public FileTestBase { FileReaderTest() : FileTestBase("FileReaderTest") {} };
struct FileMappingTest : public FileTestBase { FileMappingTest() : FileTestBase("FileMappingTest") {} };
  
std::string const testContent{R"until(
#include <iostream>

struct Car
{
   void drive()
   {
      std::cout << "brum\n";
   }   
};
)until"};

TEST_F(FileMappingTest, String)
{
   auto mapping(Utility::mapFile(createFile("NonEmpty.file", testContent)));
   EXPECT_EQ(mapping.getContent(), testContent);
   EXPECT_EQ(mapping.getContent(), testContent); ///< Twice by intention
}

TEST_F(FileMappingTest, Empty)
{
   EXPECT_THROW(Utility::mapFile(createEmptyFile("Empty.file")), std::invalid_argument);
}

TEST_F(FileReaderTest, String)
{
   EXPECT_EQ(Utility::readFile(createFile("NonEmpty.file", testContent)), testContent);
}

TEST_F(FileReaderTest, Empty)
{
   EXPECT_EQ(Utility::readFile(createEmptyFile("Empty.file")), std::string());
}
