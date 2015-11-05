#ifndef TrkBasicPlots_h
#define TrkBasicPlots_h

#include "DVAnalysis/AlgBase.h"
#include <TProfile.h>
#include <TProfile2D.h>

namespace DV
{

    class TrkBasicPlots : public DV::AlgBase {
     public:
      TrkBasicPlots();
      virtual ~TrkBasicPlots(){};
    
      virtual void bookHists();
      virtual TList* getHists();
      virtual void execute(xAOD::TEvent*);
      virtual void finalize();
     private:
      TList* m_histList;
    
      TH1F* m_nSCTHitsOnTrack; //!
      TH2F* m_nPixVsSCTHitsOnTrack; //!
      TH1F* m_trkD0_all; //!
      TH1F* m_trkZ0_all; //!
      TH1F* m_trkD0_SiSeeded; //! 
      TH1F* m_trkD0_TRTSeeded; //! 
      TH1F* m_trkD0_TRTStandalone; //! 
      TH1F* m_trkD0_LargeD0; //!
      TH1F* m_trkZ0_LargeD0; //!
      ClassDef(TrkBasicPlots, 1);
    
    };
}

#endif
