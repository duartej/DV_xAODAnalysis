#ifndef DV_BASICPLOTS_H
#define DV_BASICPLOTS_H

// For testing in RootCore..
#ifdef ASGTOOL_STANDALONE

#include "DVAnalyses/AlgBase.h"
#include <TProfile.h>
#include <TProfile2D.h>

#include "AsgTools/ToolHandle.h"
#include "DVCuts/IDVCuts.h"

namespace DV
{
    class BasicPlots : public AlgBase 
    {
        public:
            // Constructor
            BasicPlots();
            virtual ~BasicPlots(){};
            
            virtual void bookHists(PlotsManagerTool * pm) override;
            virtual void execute(xAOD::TEvent*) override;
            virtual void finalize() override;
        private:
            virtual void assignCutsAndTools() override; 
            
            TH2F* m_DVxy; //!
            TH1F* m_DVmass; //!
            TH1D* h_nTrkAssocVertAll; //!
            TH2D* h_DVmassVSnTrkAll; //!
            
            ToolHandle<DV::IDVCuts> m_handle_dvcuts; //!
            //ClassDef(BasicPlots, 1);
    };
}

#endif // ASGTOOL_STANDALONE

#endif 
