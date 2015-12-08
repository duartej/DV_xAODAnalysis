#ifndef XAOD_STANDALONE
#define XAOD_STANDALONE
#endif

#ifndef DVEVENTLOOP_H
#define DVEVENTLOOP_H

// STL
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
        //! entry point of the available cut algorithms
        EL::StatusCode addCutAlgs(const std::vector<std::string> & cut_algs);
        //! entry point for the analysis algorithms to be executed
        EL::StatusCode addAnalysisAlgs(const std::vector<std::string> & ana_names);

        // these are the functions inherited from Algorithm
        virtual EL::StatusCode setupJob (EL::Job& job);
        virtual EL::StatusCode fileExecute ();
        virtual EL::StatusCode histInitialize ();
        virtual EL::StatusCode addAnalysisAlgs ();
        virtual EL::StatusCode changeInput (bool firstFile);
        virtual EL::StatusCode initialize ();
        virtual EL::StatusCode execute ();
        virtual EL::StatusCode postExecute ();
        virtual EL::StatusCode finalize ();
        virtual EL::StatusCode histFinalize ();


        xAOD::TEvent *m_event;          //!
        unsigned int  m_eventCounter;   //!
        int m_evtsMax;                  //!

        std::vector<std::string> m_cutNames;
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
