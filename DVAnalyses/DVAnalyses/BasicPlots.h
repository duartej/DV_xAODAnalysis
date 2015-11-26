#ifndef DV_BASICPLOTS_H
#define DV_BASICPLOTS_H

// For testing in RootCore..
#ifdef ASGTOOL_STANDALONE

#include "DVAnalyses/AlgBase.h"
#include <TProfile.h>
#include <TProfile2D.h>

namespace DV
{
    class BasicPlots : public AlgBase 
    {
        public:
            // Constructor
            BasicPlots();
            virtual ~BasicPlots(){};
            
            virtual void bookHists(PlotsManagerTool * pm);
            virtual void execute(xAOD::TEvent*);
            virtual void finalize();
            virtual void assignCuts(){ ; };
        private:
            TH2F* m_DVxy; //!
            TH1F* m_DVmass; //!
            TH1D* h_nTrkAssocVertAll; //!
            TH2D* h_DVmassVSnTrkAll; //!
            
            //ClassDef(BasicPlots, 1);
    };
}

#endif // ASGTOOL_STANDALONE

#endif 
