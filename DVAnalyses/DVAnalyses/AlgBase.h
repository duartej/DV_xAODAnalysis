#ifndef DV_ALGBASE_H
#define DV_ALGBASE_H


#ifdef ASGTOOL_ATHENA
#include "AthAnalysisBaseComps/AthAnalysisAlgorithm.h"
#elif defined(ASGTOOL_STANDALONE)
#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
#else
#  error "What environment are we in?!?"
#endif // Include headers depending environment

// System headers
#include <map>
#include <string>
#include <vector>

namespace DV
{
    class PlotsManagerTool;

#ifdef ASGTOOL_ATHENA
    class AlgBase : public AthAnalysisAlgorithm
#elif defined(ASGTOOL_STANDALONE)
    class AlgBase
#endif // Environment selection
    {
        public:
#ifdef ASGTOOL_ATHENA
            // Constructor in Athena
            AlgBase(const std::string& name, ISvcLocator* pSvcLocator) :
                AthAnalysisAlgorithm(name,pSvcLocator) { ; };
#endif
            virtual ~AlgBase() { };

            //! book histograms at DVEventLoop::histInitialize
            virtual void bookHists(PlotsManagerTool * pm) = 0;
#ifdef ASGTOOL_ATHENA
            // Allow to inheriting from Athena... not sure. To be overwritten
            // by the concrete
#elif defined(ASGTOOL_STANDALONE)
            virtual bool initialize() = 0;
            virtual bool execute(xAOD::TEvent*) = 0;
            virtual bool finalize() = 0;
            virtual void setEvent(xAOD::TEvent *event) { m_event = event; };
#endif // ASGTOOL_STANDALONE

            //! Return the concrete Cut classes needed by the algorithm
            virtual std::vector<std::string> getCutNames() { return m_cutnames; };
            //! Return the concrete tool classes needed by the algorithm
            virtual std::vector<std::string> getToolNames() { return m_toolnames; };

        protected:
            //! Description of the cuts and tools which should be prepare for this algorithm
            virtual void assignCutsAndTools() = 0;
#ifdef ASGTOOL_STANDALONE
            xAOD::TEvent * m_event;          //! Really needed?
            unsigned int   m_eventCounter;   //! Really needed?
#endif // ASGTOOL_STANDALONE
            //! cuts and tools to be used (XXX: should be tracked separately?)
            std::vector<std::string> m_cutnames;
            std::vector<std::string> m_toolnames;
    };
}
#endif
