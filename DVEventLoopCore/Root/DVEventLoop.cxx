#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

#include "DVEventLoopCore/DVEventLoop.h"
// Builder for the analysis should be run
#include "DVEventLoopCore/AlgBuilder.h"
// tool instantiator/container
#include "DVEventLoopCore/ToolInstantiator.h"
// Plot Manager
#include "DVTools/PlotsManagerTool.h"

// this is needed to distribute the algorithm to the workers
ClassImp(DVEventLoop)

DVEventLoop::DVEventLoop():
    m_event(0),
    m_eventCounter(0),
    m_evtsMax(0),
    m_analysisAlgs(nullptr),
    m_plotmanager(nullptr),
    m_toolsContainer(nullptr),
    m_outputFilename("histograms.root")
{
    // Here you put any code for the base initialization of variables,
    // e.g. initialize all pointers to 0.  Note that you should only put
    // the most basic initialization here, since this method will be
    // called on both the submission and the worker node.  Most of your
    // initialization code will go into histInitialize() and
    // initialize().
    m_analysisAlgs = new std::vector<DV::AlgBase*>;
    //m_plotmanager  = new DV::PlotsManagerTool("PlotManagerTool");
}

DVEventLoop::~DVEventLoop()
{
    for(auto & analysis: *(this->m_analysisAlgs))
    {
        if( analysis != nullptr )
        {
            delete analysis;
            analysis = nullptr;
        }
    }
    if( this->m_analysisAlgs != nullptr )
    {
        delete this->m_analysisAlgs;
        this->m_analysisAlgs = nullptr;
    }
}

EL::StatusCode DVEventLoop::addCutAlgs(const std::vector<std::string> & cut_names)
{
    for(auto & cut: cut_names)
    {
        this->m_cutNames.push_back(cut);
    }

    return EL::StatusCode::SUCCESS;
}

EL::StatusCode DVEventLoop::addAnalysisAlgs(const std::vector<std::string> & alg_names)
{
    for(auto & alg: alg_names)
    {
        this->m_algNames.push_back(alg);
    }

    return EL::StatusCode::SUCCESS;
}

EL::StatusCode DVEventLoop::addAnalysisAlgs()
{
    // Create the tool-instantiator/container object to be populated
    // by the analyses needs
    m_toolsContainer = DV::ToolInstantiator::getInstance();

    // The 'regular' analysis algorithms
    for(auto & alg: this->m_algNames)
    {
        DV::AlgBase * dvAna = DV::AlgBuilder::Build(alg);
        if( ! dvAna )
        {
            return EL::StatusCode::FAILURE;
        }
        // Get the list of cut classes (tools) needed by the algorithm
        // and let know the ToolInstantiator
        for(auto & cut: dvAna->getCutNames())
        {
            EL::StatusCode sc = (EL::StatusCode)m_toolsContainer->instantiateTool(cut);
            if( sc != EL::StatusCode::SUCCESS )
            {
                return sc;
            }
        }
        this->m_analysisAlgs->push_back(dvAna);
    }

    return EL::StatusCode::SUCCESS;
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

    Info("histInitialize()","Scheduled %lu analysis algs:", m_analysisAlgs->size());
    std::string algList(" [");
    for(auto & algName : m_algNames)
    {
        algList += std::string(" "+algName);
    }
    algList += " ]";
    Info("histInitialize()","%s", algList.c_str());


    const std::vector<std::string> listOfTools = m_toolsContainer->getListOfTools();
    Info("histInitialize()","Available Tools [#%lu]:", listOfTools.size());
    std::string toolList(" [");
    for(auto & toolName : listOfTools)
    {
        toolList += std::string(" "+toolName);
    }
    toolList += " ]";
    Info("histInitialize()","%s", toolList.c_str());


    // Initialize plot tool
    m_plotmanager = new DV::PlotsManagerTool("PlotManagerTool");
    m_plotmanager->bookFile(m_outputFilename,"RECREATE");
    for (unsigned int i=0; i< m_analysisAlgs->size(); ++i)
    {
        m_analysisAlgs->at(i)->bookHists(m_plotmanager);
    }

    return EL::StatusCode::SUCCESS;
}

EL::StatusCode DVEventLoop :: fileExecute ()
{
    // Here you do everything that needs to be done exactly once for every
    // single file, e.g. collect a list of all lumi-blocks processed

    return EL::StatusCode::SUCCESS;
}

EL::StatusCode DVEventLoop :: changeInput (bool /*firstFile*/)
{
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
    m_eventCounter = 0;

    // just coherence
    if(m_evtsMax == 0)
    {
        m_evtsMax = m_event->getEntries();
    }

    // as a check, let's see the number of events in our xAOD
    Info("initialize()", "Number of available events = %lli", m_event->getEntries() );
    Info("initialize()", "Number of events to be processed= %lu", m_evtsMax );

    // FIXME:: Why is this defined here? Should be managed by the client who needs it
    // create TrigDecisionTool --> BLOCK MOVED TO THE EventCut class!!
    /*auto tct = new TrigConf::xAODConfigTool("xAODConfigTool");
    if(tct->initialize().isFailure()) return StatusCode::FAILURE;

    ToolHandle<TrigConf::ITrigConfigTool> tch(tct);

    auto tdt = new Trig::TrigDecisionTool("TrigDecisionTool");
    if(tdt->setProperty("ConfigTool", tch).isFailure()) return EL::StatusCode::FAILURE;
    if(tdt->setProperty("TrigDecisionKey", "xTrigDecision").isFailure()) return EL::StatusCode::FAILURE;
    if(tdt->initialize().isFailure()) return EL::StatusCode::FAILURE;
    // :: Why is this defined here? END-FIXME-!*/

    // first initialize analyses, which can change some properties of the tools
    for(unsigned int i = 0; i < m_analysisAlgs->size(); ++i)
    {
        if(!m_analysisAlgs->at(i)->initialize())
        {
            return EL::StatusCode::FAILURE;
        }
    }

    // then initialize tools 
    for(const std::string& toolName: DV::ToolInstantiator::getListOfTools())
    {
        if(!DV::ToolInstantiator::initializeTool(toolName))
        {
            return EL::StatusCode::FAILURE;
        }
    }

    return EL::StatusCode::SUCCESS;
}

EL::StatusCode DVEventLoop :: execute ()
{
    // Here you do everything that needs to be done on every single
    // events, e.g. read input variables, apply cuts, and fill
    // histograms and trees.  This is where most of your actual analysis
    // code will go.

    // print every 5% of processed events, so we know where we are:
    if( (m_eventCounter % int(float(m_evtsMax)*0.05) ) == 0 )
    {
        Info("execute()", "Event number = %lu", m_eventCounter );
    }
    ++m_eventCounter;

    // execute all the event
    for(unsigned int i = 0; i < m_analysisAlgs->size(); ++i)
    {
        if(!m_analysisAlgs->at(i)->execute(m_event))
        {
            return EL::StatusCode::FAILURE;
        }
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

    for(unsigned int i = 0; i < m_analysisAlgs->size(); ++i)
    {
        if(!m_analysisAlgs->at(i)->finalize())
        {
            return EL::StatusCode::FAILURE;
        }
    }

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

    // storing histograms
    if( ! m_plotmanager->saveResults() )
    {
         return EL::StatusCode::FAILURE;
    }
    // And free memory, nothing left to be done ...
    if( m_plotmanager != 0 )
    {
        delete m_plotmanager;
        m_plotmanager = 0;
    }

    return EL::StatusCode::SUCCESS;
}
