#ifndef DV_ALGBASE_H
#define DV_ALGBASE_H


#ifdef ASGTOOL_ATHENA
#include "AthAnalysisBaseComps/AthAnalysisAlgorithm.h"
#elif defined(ASGTOOL_STANDALONE)
#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
using namespace EL;
#else
#  error "What environment are we in?!?"
#endif // Include headers depending environment


// System headers
#include <vector>
#include <map>
#include <string>

namespace DV 
{
    class PlotsManagerTool;
    class CutsBase;

#ifdef ASGTOOL_ATHENA
    class AlgBase : public AthAnalysisAlgorithm
#elif defined(ASGTOOL_STANDALONE)
    class AlgBase 
#else
#  error "What environment are we in?!?"
#endif // Environment selection
    {
        public:
#ifdef ASGTOOL_ATHENA
            // Constructor in Athena
            AlgBase(const std::string& name, ISvcLocator* pSvcLocator);
#endif
            virtual ~AlgBase() { };
            
            //! book histograms at DVEventLoop::histInitialize
            virtual void bookHists(PlotsManagerTool * pm) = 0;
#ifdef ASGTOOL_ATHENA
            // Allow to inheriting from Athena... not sure. To be overwritten
            // by the concrete
/*            virtual StatusCode initialize() { return StatusCode::SUCCESS; };
            virtual StatusCode execute() { return StatusCode::SUCCESS; };        
            virtual StatusCode finalize() { return StatusCode::SUCCESS; };        */
#elif defined(ASGTOOL_STANDALONE)
            //! to be called byt the DVEventLoop::histFinalize
            virtual StatusCode execute(xAOD::TEvent*)=0;        
            virtual StatusCode finalize() =0;        
            virtual StatusCode setEvent(xAOD::TEvent *event) { m_event=event; };
#endif // ASGTOOL_STANDALONE

            //! Return the concrete Cut classes needed by the algorithm
            virtual std::vector<std::string> getCutNames() { return m_cutnames; };
            //! Return the concrete tool classes needed by the algorithm
            virtual std::vector<std::string> getToolNames() { return m_toolnames; };
            //! Get access to the needed cut classes
            virtual void attachCut(const std::string & cutname,const CutsBase * cutObject) { m_cutsMap[cutname] = cutObject; };
            //! Concrete implementation of the cuts.. explain that more
            virtual void assignCuts() = 0;
        
        protected:
            virtual void checkCutAvailability(const std::string & cut, const std::string & algname);
#ifdef ASGTOOL_STANDALONE
            xAOD::TEvent * m_event;          //! Really needed?
            unsigned int   m_eventCounter;   //! Really needed?
#endif // ASGTOOL_STANDALONE
            std::vector<std::string> m_cutnames;
            std::vector<std::string> m_toolnames;

            //! cuts to be used
            std::map<std::string,const CutsBase *> m_cutsMap;
    };
}
#endif
