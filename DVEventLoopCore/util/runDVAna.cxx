
#include "DVEventLoopCore/DVEventLoop.h"
#include "DVEventLoopCore/PyParser.h"

#include "Rtypes.h"

#include "xAODRootAccess/Init.h"
#include "SampleHandler/SampleHandler.h"
//#include "SampleHandler/ScanDir.h"
#include "SampleHandler/ToolsDiscovery.h"
#include "EventLoop/Job.h"
#include "EventLoop/DirectDriver.h"

#include<iostream>
#include<fstream>
#include<cstdio>

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

  // *********************************************************
  // Getting the configuration 
  PyParser parser(argv[1]);

  Info("runDVana","Extracting configuration from '%s'",argv[1]);
  // Take the submit directory from the input if provided:
  std::string submitDir = "submitDir";
  //if( argc > 1 ) submitDir = argv[ 1 ];
  if( parser.Check("submitDir") )
  {
      submitDir = parser.Get<std::string>("submitDir");
  }
  // Get the input datasample(s)
  std::vector<std::string> filesinput;
  if( parser.Check("filesinput") )
  {
      filesinput = parser.Get<std::vector<std::string> >("filesinput");
  }

  // Name of the output file
  std::string outputFilename("histograms.root");
  if( parser.Check("outputFilename") )
  {
      outputFilename = parser.Get<std::string>("outputFilename");
      Info("DVEventLoop",std::string("Output file: '"+outputFilename+"'").c_str());
  }
  // Number of events to be processed
  int evtsMax = -1;
  if( parser.Check("evtsMax") )
  {
      evtsMax = parser.Get<int>("evtsMax");
  }
  int skipEvts = -1;
  if( parser.Check("skipEvts") )
  {
      skipEvts = parser.Get<int>("skipEvts");
  }
  // End of configuration 
  // *********************************************************


  // Set up the job for xAOD access:
  xAOD::Init().ignore();

  // Construct the samples to run on:
  SH::SampleHandler sh;
  /* Will work with patterns and directoris
  for(auto & infile: filesinput)
  {
      SH::ScanDir().scan(sh,infile.c_str());
  }
  SH::ScanDir().sampleName("provisional_until_dict_in_Python"); */
  //Create tmp file
  const std::string tmpfilename("inputfile.txt");
  std::ofstream tmpfile(tmpfilename);
  for(auto & infile: filesinput)
  {
      tmpfile << infile << std::endl;
  }
  SH::readFileList(sh, "sample_name_provisional", tmpfilename);
  std::remove(tmpfilename.c_str());
  // Set the name of the input TTree. It's always "CollectionTree"
  // for xAOD files.
  sh.setMetaString( "nc_tree", "CollectionTree" );

  // Print what we found:
  sh.print();

  // Create an EventLoop job:
  EL::Job job;
  job.sampleHandler( sh );
  // some configurations...
  if( evtsMax > 0 )
  {
      job.options()->setDouble(EL::Job::optMaxEvents,evtsMax);
  }
  if( skipEvts > 0 )
  {
      job.options()->setDouble(EL::Job::optSkipEvents,skipEvts);
  }

  DVEventLoop* alg = new DVEventLoop();
  // some configurations to the looper ---
  alg->setOutputFilename(outputFilename);
  alg->setEvtsMax(evtsMax);
  
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
