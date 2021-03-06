#ifndef XAOD_STANDALONE
#define XAOD_STANDALONE
#endif

#ifndef DVEVENTLOOP_H
#define DVEVENTLOOP_H

// STL
#include<cstdint>
#include<string>
#include<vector>

// RootCore
#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"

// EventLoop
#include <EventLoop/Algorithm.h>

// DV
#include "DVAnalyses/AlgBase.h"

namespace DV
{
    class PlotsManagerTool;
    class ToolInstantiator;
}

class DVEventLoop : public EL::Algorithm
{
    // variables that don't get filled at submission time should be
    // protected from being send from the submission node to the worker
    // node (done by the //!)
    public:
        DVEventLoop();
        ~DVEventLoop();

        //! set the maximum number of events to be processed
        void setEvtsMax(const int & evtmax){ m_evtsMax = evtmax; };
        //! name of the output file
        void setOutputFilename(const std::string & name) { m_outputFilename = name; };
        //! entry point for the analysis algorithms to be executed
        EL::StatusCode addAnalysisAlgs(const std::vector<std::string> & ana_names);

        EL::StatusCode addAnalysisAlgs ();
        EL::StatusCode addAnalysisTools ();

        // these are the functions inherited from Algorithm
        EL::StatusCode setupJob (EL::Job& job) override;
        EL::StatusCode fileExecute () override;
        EL::StatusCode histInitialize () override;
        EL::StatusCode changeInput (bool firstFile) override;
        EL::StatusCode initialize () override;
        EL::StatusCode execute () override;
        EL::StatusCode postExecute () override;
        EL::StatusCode finalize () override;
        EL::StatusCode histFinalize () override;

    //private:

        xAOD::TEvent *m_event;        //!
        std::uint64_t m_eventCounter; //!
        std::uint64_t m_evtsMax;      //!

        std::vector<std::string> m_algNames;

        std::vector<DV::AlgBase*>* m_analysisAlgs;

        DV::PlotsManagerTool * m_plotmanager; //!

        DV::ToolInstantiator * m_toolsContainer; //!

        std::string m_outputFilename;

        // this is needed to distribute the algorithm to the workers
        // 0: you dont want to use I/O for this class, right? FIXME
        ClassDef(DVEventLoop, 1);
};

#endif
