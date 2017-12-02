
#include "Lib/include/Hasher.h"

#include "../include/FileTestBase.h"

#include <gtest/gtest.h>

#include <fstream>

struct FileHasherTest : FileTestBase { FileHasherTest() : FileTestBase("FileHasherTest") {} };

std::string const testContent{ R"until(
#include <iostream>

struct SpaceShip
{
   void Fly()
   {
      std::cout << ":::>>>---\n";
   }   
};

)until"};

TEST(StringHasherTest, NonEmpty)
{
   EXPECT_EQ("F572D396FAE9206628714FB2CE00F72E94F2258F", Utility::Hasher::getSHA1(std::string("hello\n")));
}

TEST(StringHasherTest, Empty)
{
   EXPECT_THROW(Utility::Hasher::getSHA1(std::string("")), std::invalid_argument);
}

TEST_F(FileHasherTest, NonEmpty)
{
   EXPECT_EQ("36665E0AEDCFC1F2A4B09E41CD51422C927CCAF7", Utility::Hasher::getSHA1(createFile("NonEmpty.cpp", testContent)));
}

TEST_F(FileHasherTest, Empty)
{
   EXPECT_THROW(Utility::Hasher::getSHA1(createEmptyFile("Empty.cpp")), std::invalid_argument);
}
