#ifndef DV_ALGBASE_H
#define DV_ALGBASE_H

#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
#include "TList.h"


// System headers
#include <vector>
#include <map>
#include <string>

namespace DV 
{
    class CutsBase;

    class AlgBase 
    {
        public:
            virtual ~AlgBase() { };
            virtual void init() {bookHists();};
            //! book histograms at DVEventLoop::histInitialize
            virtual void bookHists() = 0;
            //! to be called byt the DVEventLoop::histFinalize
            virtual TList* getHists() = 0;
            virtual void execute(xAOD::TEvent*)=0;        
            virtual void finalize() =0;        
            virtual void setEvent(xAOD::TEvent *event) { m_event=event; };
            //! Return the concrete Cut classes needed by the algorithm
            virtual std::vector<std::string> getCutNames() { return m_cutnames; };
            //! Return the concrete tool classes needed by the algorithm
            virtual std::vector<std::string> getToolNames() { return m_toolnames; };
            //! Get access to the needed cut classes
            virtual void attachCut(const std::string & cutname,const CutsBase * cutObject) { m_cutsMap[cutname] = cutObject; };

        protected:
            xAOD::TEvent * m_event;          //!
            unsigned int   m_eventCounter;   //!
            //////  std::vector<TH1*> m_histList;   //!
            std::vector<std::string> m_cutnames;
            std::vector<std::string> m_toolnames;

            //! cuts to be used
            std::map<std::string,const CutsBase *> m_cutsMap;
    };
}
#endif
