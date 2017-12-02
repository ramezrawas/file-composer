
#include "Lib/include/FileMatcher.h"
#include "Lib/include/ChunkDescriptor.h"
#include "Lib/include/FileComposite.h"

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include <log4cxx/xml/domconfigurator.h>

#include <iostream>
#include <regex>
#include <string>
#include <cstdlib>

namespace bfs = boost::filesystem;
namespace bpo = boost::program_options;

int main(int argc, char** argv)
{
   auto const executablePath(bfs::path(argv[0]).parent_path());
   
   bpo::options_description desc("Options");
   desc.add_options()
      ("help,h"
         ,"Print this help message")
      ("pattern,p"      
         ,bpo::value<std::string>()->default_value(".*[.]cpp$")
         ,"Pattern for file matching")
      ("destination,d"  
         ,bpo::value<bfs::path>()->default_value(bfs::current_path() / "destination.txt")
         ,"Destination file path")
      ("root,r"
         ,bpo::value<bfs::path>()->default_value(bfs::current_path())
         ,"Root directory path")
      ("log-file,l"
         ,bpo::value<bfs::path>()->default_value(bfs::current_path() / "fileComposite.log")
         ,"Path to log-file")   
      ("logger-config-file"
         ,bpo::value<bfs::path>()->default_value(executablePath / "etc" / "loggerConfig.xml")
         ,"Path to logger configuration file")
      ;
   
   try
   {
      bpo::variables_map vm;
      bpo::store(bpo::parse_command_line(argc, argv, desc), vm);
      bpo::notify(vm);

      if (vm.count("help"))
      {
         std::cout << desc << '\n';
         return 0;
      }
      
      auto const filePattern{       vm["pattern"].as<std::string>() };
      auto const rootPath{          vm["root"].as<bfs::path>() };
      auto const destination{       vm["destination"].as<bfs::path>() };
      auto const logFilePath{       vm["log-file"].as<bfs::path>() };
      auto const loggerConfigFile{  vm["logger-config-file"].as<bfs::path>() };
      
      setenv("logfile.path", logFilePath.string().c_str(), 1);
      log4cxx::xml::DOMConfigurator::configure(loggerConfigFile.string());
      
      auto logger{ log4cxx::Logger::getLogger("FileComposer") };
      
      auto matchingPaths{ Utility::FileMatcher::getNonEmptyMatches(rootPath, std::regex(filePattern)) };
      LOG4CXX_INFO(logger, "Matching non-empty files: " << matchingPaths.size());
      
      auto fileComposite{ Utility::FileComposite(destination) };
      fileComposite.merge(Utility::ChunkDescriptor::create(std::move(matchingPaths), Utility::orderByFirstNonEmptyLine));
      fileComposite.writeSorted();
      LOG4CXX_INFO(logger, "Files joined in: " << destination.string());
      
      return 0;
   }
   catch (std::exception const& e)
   {
      std::cerr << "Error: " << e.what() << '\n';
      return -1;
   }
}
