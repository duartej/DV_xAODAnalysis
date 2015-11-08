#ifndef DV_BASICPLOTS_H
#define DV_BASICPLOTS_H

#include "DVAnalysis/AlgBase.h"
#include <TProfile.h>
#include <TProfile2D.h>

namespace DV
{
    class PlotsManager;

    class BasicPlots : public AlgBase {
     public:
      BasicPlots();
      virtual ~BasicPlots(){};
    
      virtual void bookHists(PlotsManager * pm);
      //virtual TList* getHists();
      virtual void execute(xAOD::TEvent*);
      virtual void finalize();

      virtual void assignCuts(){ ; };
     private:
      //TList* m_histList;
    
      TH2F* m_DVxy; //!
      TH1F* m_DVmass; //!
      TH1D* h_nTrkAssocVertAll; //!
      TH2D* h_DVmassVSnTrkAll; //!
    
      ClassDef(BasicPlots, 1);
    };
}

#endif
