
#include<iostream>

#include "xAODRootAccess/Init.h"
#include "SampleHandler/SampleHandler.h"
#include "SampleHandler/ToolsDiscovery.h"
#include "EventLoop/Job.h"
#include "EventLoop/DirectDriver.h"

#include "DVAnalysis/PyParser.h"

#include "DVAnalysis/DVEventLoop.h"

void display_usage()
{
  std::cout << "\033[37musage:\033[m runDVAna config.py [OPTIONS]" << std::endl;
  std::cout << std::endl;
}

int main( int argc, char* argv[] ) 
{
  if(argc < 2)
  {
      display_usage();
      return -1;
  }

  // Getting the configuration
  PyParser parser(argv[1]);

  // Take the submit directory from the input if provided:
  std::string submitDir = "submitDir";
  //if( argc > 1 ) submitDir = argv[ 1 ];
  if( parser.Check("submitDir") )
  {
      submitDir = parser.Get<std::string>("submitDir");
  }

  // Set up the job for xAOD access:
  xAOD::Init().ignore();

  // Construct the samples to run on:
  SH::SampleHandler sh;
  ////  SH::scanDir( sh, "/r03/atlas/nbarlow/xAOD/TestInDetDxAOD");
  SH::readFileList (sh, "sample", "inputfiles.txt");
  // Set the name of the input TTree. It's always "CollectionTree"
  // for xAOD files.
  sh.setMetaString( "nc_tree", "CollectionTree" );

  // Print what we found:
  sh.print();

  // Create an EventLoop job:
  EL::Job job;
  job.sampleHandler( sh );

  DVEventLoop* alg = new DVEventLoop();
  // Add the available cuts algorithms
  //std::vector<std::string> cuts = 
  //    parser.Get<std::vector<std::string> >("cut_algs"); 
  //alg->addCutAlgs( cuts );
  // Add our analysis to the job:
  std::vector<std::string> analyses = 
      parser.Get<std::vector<std::string> >("analyses"); 
  alg->addAnalysisAlgs( analyses );
  job.algsAdd( alg );

  // Run the job using the local/direct driver:
  EL::DirectDriver driver;

  
  driver.submit( job, submitDir );

  return 0;
}
