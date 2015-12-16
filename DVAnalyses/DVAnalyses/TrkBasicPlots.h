#ifndef DV_TRKBASICPLOTS_H
#define DV_TRKBASICPLOTS_H
#include "DVAnalyses/AlgBase.h"
#include "DVCuts/IDVCuts.h"

#include "GaudiKernel/ToolHandle.h" 

class TH1I;
class TH1F;
class TH2F;

namespace DV
{
    class TrkBasicPlots : public DV::AlgBase 
    {
        public:
            TrkBasicPlots(const std::string & name, ISvcLocator * pSvcLocator);
            ~TrkBasicPlots() = default;
            
            StatusCode initialize() override;
            StatusCode execute() override; 
            StatusCode finalize() override;
     private:
            ToolHandle<DV::IDVCuts> m_dvcuts; //!

            TH1I* m_nTracks; //!      
            TH1F* m_nSCTHitsOnTrack; //!
            TH2F* m_nPixVsSCTHitsOnTrack; //!
            TH1F* m_trkD0_all; //!
            TH1F* m_trkZ0_all; //!
            TH1F* m_trkD0_SiSeeded; //! 
            TH1F* m_trkD0_TRTSeeded; //! 
            TH1F* m_trkD0_TRTStandalone; //! 
            TH1F* m_trkD0_LargeD0; //!
            TH1F* m_trkZ0_LargeD0; //!
            //ClassDef(TrkBasicPlots, 1);    
    };
}

#endif
