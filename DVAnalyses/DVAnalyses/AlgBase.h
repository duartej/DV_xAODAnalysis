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
#include <cstdint>
#include <string>
#include <vector>

namespace DV
{
    class PlotsManagerTool;

    class AlgBase
#ifdef ASGTOOL_ATHENA
    : public AthAnalysisAlgorithm
#endif
    {
        public:
#ifdef ASGTOOL_ATHENA
            // Constructor in Athena
            AlgBase(const std::string& name, ISvcLocator* pSvcLocator) :
                AthAnalysisAlgorithm(name, pSvcLocator) {};
#endif
            virtual ~AlgBase() = default;

            //! book histograms at DVEventLoop::histInitialize
            virtual void bookHists(PlotsManagerTool * pm) = 0;

#ifdef ASGTOOL_ATHENA
            StatusCode initialize() override = 0;
            StatusCode execute() override = 0;
            StatusCode finalize() override = 0;
#elif defined(ASGTOOL_STANDALONE)
            virtual bool initialize() = 0;
            virtual bool execute(xAOD::TEvent*) = 0;
            virtual bool finalize() = 0;
#endif

            //! Return the concrete tool classes needed by the algorithm
            virtual std::vector<std::string> getToolNames() { return m_toolnames; };

        protected:
            //! Description of the cuts and tools which should be prepare for this algorithm
            virtual void assignCutsAndTools() = 0;
#ifdef ASGTOOL_STANDALONE
            std::uint64_t m_eventCounter = 0;
#endif
            //! tools to be used
            std::vector<std::string> m_toolnames;
    };
}
#endif
