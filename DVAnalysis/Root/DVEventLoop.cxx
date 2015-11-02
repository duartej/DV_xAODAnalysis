#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <DVAnalysis/DVEventLoop.h>

/// analysis algs that might be run
//#include "DVAnalysis/DVBasicPlots.h"
//#include "DVAnalysis/TrkBasicPlots.h"
// The analysis builder encapsulates this information
#include "DVAnalysis/DVAnaBuilder.h"


#include "xAODRootAccess/TStore.h"
#include "xAODCore/ShallowCopy.h"
#include <TList.h>


// this is needed to distribute the algorithm to the workers
ClassImp(DVEventLoop)

DVEventLoop::DVEventLoop():
    m_event(0),
    m_eventCounter(0),
    m_analysisAlgs(0),//new std::vector<DVBase*>),
    m_outputFile(0)
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  Note that you should only put
  // the most basic initialization here, since this method will be
  // called on both the submission and the worker node.  Most of your
  // initialization code will go into histInitialize() and
  // initialize(). 
  m_analysisAlgs = new std::vector<DVBase*>;
}

DVEventLoop::~DVEventLoop()
{
    for(auto & analysis: *(this->m_analysisAlgs))
    {
        if( analysis != 0 )
        {
            delete analysis;
            analysis = 0;
        }
    }
    if( this->m_analysisAlgs != 0 )
    {
        delete this->m_analysisAlgs;
        this->m_analysisAlgs = 0;
    }
}


EL::StatusCode DVEventLoop::addAnalysisAlgs(const std::vector<std::string> & alg_names) 
{ 
    /* DVBasicPlots* dvPlots=new DVBasicPlots();
    m_analysisAlgs->push_back(dvPlots);
    TrkBasicPlots* trkPlots=new TrkBasicPlots();
    m_analysisAlgs->push_back(trkPlots); */
    
    // add analysis codes
    /*for(auto & alg: alg_names)
    {
        DVBase * dvAna = DVAnaBuilder::Build(alg);
        if( ! dvAna )
        {
            return EL::StatusCode::FAILURE;
        }
        this->m_analysisAlgs->push_back(dvAna);
    }*/
    for(auto & alg: alg_names)
    {
        this->m_algNames.push_back(alg);
    }
    
    return EL::StatusCode::SUCCESS;
}

EL::StatusCode DVEventLoop::addAnalysisAlgs() 
{ 
    /* DVBasicPlots* dvPlots=new DVBasicPlots();
    m_analysisAlgs->push_back(dvPlots);
    TrkBasicPlots* trkPlots=new TrkBasicPlots();
    m_analysisAlgs->push_back(trkPlots); */
    for(auto & alg: this->m_algNames)
    {
        DVBase * dvAna = DVAnaBuilder::Build(alg);
        if( ! dvAna )
        {
            return EL::StatusCode::FAILURE;
        }
        this->m_analysisAlgs->push_back(dvAna);
    }
    
    //- Obsolete
    /*std::cout << "This function is DEPRECATED, use the signature "
        << "'addAnalysisAlgs(vector<string>)'" << std::endl;*/

    return EL::StatusCode::FAILURE;
}

EL::StatusCode DVEventLoop :: setupJob (EL::Job& job)
{
  // Here you put code that sets up the job on the submission object
  // so that it is ready to work with your algorithm, e.g. you can
  // request the D3PDReader service or add output files.  Any code you
  // put here could instead also go into the submission script.  The
  // sole advantage of putting it here is that it gets automatically
  // activated/deactivated when you add/remove the algorithm from your
  // job, which may or may not be of value to you.
  
  std::cout<<" in DVEventLoop::setupJob"<<std::endl;
  
  job.useXAOD ();
  // avoid having to manually remove "submitDir" each time
  job.options()->setDouble (EL::Job::optRemoveSubmitDir, 1);
  // let's initialize the algorithm to use the xAODRootAccess package
  xAOD::Init( "DVEventLoop" ).ignore(); // call before opening first file
  


  return EL::StatusCode::SUCCESS;
}



EL::StatusCode DVEventLoop :: histInitialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.

  addAnalysisAlgs();
  
  std::cout<<"Will run "<<m_analysisAlgs->size()<<" analysis algs"<<std::endl;

  m_outputFile= new TFile("histograms.root","RECREATE");
  for (unsigned int i=0; i< m_analysisAlgs->size(); ++i) {
    m_analysisAlgs->at(i)->bookHists();
  }
  

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode DVEventLoop :: fileExecute ()
{
  ////  std::cout<<" in DVEventLoop::fileExecute"<<std::endl;
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode DVEventLoop :: changeInput (bool /*firstFile*/)
{
  ///  std::cout<<" in DVEventLoop::changeInput"<<std::endl;
  // Here you do everything you need to do when we change input files,
  // e.g. resetting branch addresses on trees.  If you are using
  // D3PDReader or a similar service this method is not needed.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode DVEventLoop :: initialize ()
{

  // Here you do everything that you need to do after the first input
  // file has been connected and before the first event is processed,
  // e.g. create additional histograms based on which variables are
  // available in the input files.  You can also create all of your
  // histograms and trees in here, but be aware that this method
  // doesn't get called if no events are processed.  So any objects
  // you create here won't be available in the output if you have no
  // input events.
  
  m_event = wk()->xaodEvent();
  m_eventCounter=0;
  
  // as a check, let's see the number of events in our xAOD
  Info("initialize()", "Number of events = %lli", m_event->getEntries() ); // print long long int
  
  
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode DVEventLoop :: execute () {
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.
  

  // print every 100 events, so we know where we are:
  // if( (m_eventCounter % 1000) ==0 ) 
  Info("execute()", "Event number = %i", m_eventCounter );
  m_eventCounter++;

  //----------------------------
  // Event information
  //--------------------------- 
  // const xAOD::EventInfo* eventInfo = 0;
  // if( ! m_event->retrieve( eventInfo, "EventInfo").isSuccess() ){
  //  Error("execute()", "Failed to retrieve event info collection. Exiting." );
  //  return EL::StatusCode::FAILURE;
  // }

  ////  if (m_eventCounter%1000 ==0) std::cout<<"Lumi block"<<eventInfo->lumiBlock()<<std::endl;
 
  //// execute all the event
    
  for (unsigned int i=0; i< m_analysisAlgs->size(); ++i) {
    m_analysisAlgs->at(i)->execute(m_event);
    
  }
  
  
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode DVEventLoop :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode DVEventLoop :: finalize ()
{
  // This method is the mirror image of initialize(), meaning it gets
  // called after the last event has been processed on the worker node
  // and allows you to finish up any objects you created in
  // initialize() before they are written to disk.  This is actually
  // fairly rare, since this happens separately for each worker node.
  // Most of the time you want to do your post-processing on the
  // submission node after all your histogram outputs have been
  // merged.  This is different from histFinalize() in that it only
  // gets called on worker nodes that processed input events.

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode DVEventLoop :: histFinalize ()
{
  // This method is the mirror image of histInitialize(), meaning it
  // gets called after the last event has been processed on the worker
  // node and allows you to finish up any objects you created in
  // histInitialize() before they are written to disk.  This is
  // actually fairly rare, since this happens separately for each
  // worker node.  Most of the time you want to do your
  // post-processing on the submission node after all your histogram
  // outputs have been merged.  This is different from finalize() in
  // that it gets called on all worker nodes regardless of whether
  // they processed input events.
  
  //m_allHistograms

  m_outputFile->cd();

  
  for (unsigned int i=0; i< m_analysisAlgs->size(); ++i) {
    TList* hists = m_analysisAlgs->at(i)->getHists();
    TObject* h(0);
    TIter next(hists);
    while( (h=next()) ) {
      //      std::cout<<" Writing histogram "<<h->GetName()<<std::endl;
      
      h->Write();
    }
    
  }
  
  m_outputFile->Close();
  
  return EL::StatusCode::SUCCESS;
}