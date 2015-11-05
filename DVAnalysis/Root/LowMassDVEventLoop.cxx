#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <DVAnalysis/LowMassDVEventLoop.h>

/// analysis algs that might be run
#include "DVAnalysis/BasicPlots.h"
#include "DVAnalysis/TrkBasicPlots.h"
#include "DVAnalysis/LowMassDVEff.h"

#include "xAODRootAccess/TStore.h"
#include "xAODCore/ShallowCopy.h"

#include "xAODRootAccess/tools/Message.h"

#include <TList.h>
#include <TSystem.h>

// this is needed to distribute the algorithm to the workers
ClassImp(LowMassDVEventLoop)

/// Helper macro for checking xAOD::TReturnCode return values
#define EL_RETURN_CHECK( CONTEXT, EXP )                     \
   do {                                                     \
      if( ! EXP.isSuccess() ) {                             \
         Error( CONTEXT,                                    \
                XAOD_MESSAGE( "Failed to execute: %s" ),    \
                #EXP );                                     \
         return EL::StatusCode::FAILURE;                    \
      }                                                     \
   } while( false )


LowMassDVEventLoop :: LowMassDVEventLoop ()
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  Note that you should only put
  // the most basic initialization here, since this method will be
  // called on both the submission and the worker node.  Most of your
  // initialization code will go into histInitialize() and
  // initialize(). 
  m_analysisAlgs = new std::vector<DV::AlgBase*>;
}

EL::StatusCode
LowMassDVEventLoop :: addAnalysisAlgs () { 

  /// add analysis code
  DV::BasicPlots* dvPlots=new DV::BasicPlots();
  m_analysisAlgs->push_back(dvPlots);

  DV::TrkBasicPlots* trkPlots=new DV::TrkBasicPlots();
  m_analysisAlgs->push_back(trkPlots);
  
  LowMassDVEff* vertEff = new LowMassDVEff();
  m_analysisAlgs->push_back(vertEff);

  
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode LowMassDVEventLoop :: setupJob (EL::Job& job)
{
  // Here you put code that sets up the job on the submission object
  // so that it is ready to work with your algorithm, e.g. you can
  // request the D3PDReader service or add output files.  Any code you
  // put here could instead also go into the submission script.  The
  // sole advantage of putting it here is that it gets automatically
  // activated/deactivated when you add/remove the algorithm from your
  // job, which may or may not be of value to you.
  
  std::cout<<" in LowMassDVEventLoop::setupJob"<<std::endl;
  
  job.useXAOD ();
  // avoid having to manually remove "submitDir" each time
  job.options()->setDouble (EL::Job::optRemoveSubmitDir, 1);
  // let's initialize the algorithm to use the xAODRootAccess package
  xAOD::Init( "LowMassDVEventLoop" ).ignore(); // call before opening first file
  


  return EL::StatusCode::SUCCESS;
}



EL::StatusCode LowMassDVEventLoop :: histInitialize ()
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



EL::StatusCode LowMassDVEventLoop :: fileExecute ()
{
  ////  std::cout<<" in LowMassDVEventLoop::fileExecute"<<std::endl;
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode LowMassDVEventLoop :: changeInput (bool firstFile)
{
  ///  std::cout<<" in LowMassDVEventLoop::changeInput"<<std::endl;
  // Here you do everything you need to do when we change input files,
  // e.g. resetting branch addresses on trees.  If you are using
  // D3PDReader or a similar service this method is not needed.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode LowMassDVEventLoop :: initialize ()
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
  
  // GRL
  m_grl = new GoodRunsListSelectionTool("GoodRunsListSelectionTool");
  const char* GRLFilePath = "$ROOTCOREBIN/data/DVAnalysis/data15_13TeV.periodAllYear_DetStatus-v67-pro19-02_DQDefects-00-01-02_PHYS_StandardGRL_All_Good.xml";
  const char* fullGRLFilePath = gSystem->ExpandPathName (GRLFilePath);
  std::vector<std::string> vecStringGRL;
  vecStringGRL.push_back(fullGRLFilePath);
  EL_RETURN_CHECK("initialize()",m_grl->setProperty( "GoodRunsListVec", vecStringGRL));
  EL_RETURN_CHECK("initialize()",m_grl->setProperty("PassThrough", false)); // if true (default) will ignore result of GRL and will just pass all events
  EL_RETURN_CHECK("initialize()",m_grl->initialize());
  
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode LowMassDVEventLoop :: execute () {
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.
  

  // print every 100 events, so we know where we are:
  if( (m_eventCounter % 100) == 0 )
  Info("execute()", "Event number = %i", m_eventCounter );
  m_eventCounter++;

  //----------------------------
  // Event information
  //--------------------------- 
  const xAOD::EventInfo* eventInfo = 0;
  EL_RETURN_CHECK("execute",m_event->retrieve( eventInfo, "EventInfo"));
  
  // check if the event is data or MC
  // (many tools are applied either to data or MC)
  bool isMC = false;
  // check if the event is MC
  if(eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION ) ){
    isMC = true; // can do something with this later
  }

  // if data check if event passes GRL
  if(!isMC){ // it's data!
    if(!m_grl->passRunLB(*eventInfo)){
      return EL::StatusCode::SUCCESS; // go to next event
    }
  } // end if not MC
  
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



EL::StatusCode LowMassDVEventLoop :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode LowMassDVEventLoop :: finalize ()
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

  // GRL
  if (m_grl) {
    delete m_grl;
    m_grl = 0;
  }
  
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode LowMassDVEventLoop :: histFinalize ()
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
    while (h=next()) {
      //      std::cout<<" Writing histogram "<<h->GetName()<<std::endl;
      
      h->Write();
    }
    
  }
  
  m_outputFile->Close();
  
  return EL::StatusCode::SUCCESS;
}
