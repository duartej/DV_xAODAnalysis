#include "DVAnalyses/TrkBasicPlots.h"

// framework includes
#include "GaudiKernel/ServiceHandle.h"
#include "GaudiKernel/ITHistSvc.h"


// EDM includes: - if move to header file will not compile?
#include "xAODEventInfo/EventInfo.h"
#include "xAODTracking/TrackMeasurementValidationContainer.h"
#include "xAODTracking/TrackStateValidationContainer.h"
#include "xAODTracking/TrackParticleContainer.h"
#include "xAODTracking/TrackParticleAuxContainer.h"

//ROOT
#include "TH1I.h"
#include "TH1F.h"
#include "TH2F.h"

#include<iostream>
#include<bitset>
#include<vector>
#include<string>

DV::TrkBasicPlots::TrkBasicPlots(const std::string & name, ISvcLocator * pSvcLocator) :
    DV::AlgBase(name,pSvcLocator),
    m_dvcuts("DV::DVCuts/DVCuts"),
    m_nTracks(0),
    m_nSCTHitsOnTrack(0),
    m_nPixVsSCTHitsOnTrack(0),
    m_trkD0_all(0),
    m_trkZ0_all(0),
    m_trkD0_SiSeeded(0),
    m_trkD0_TRTSeeded(0),
    m_trkD0_TRTStandalone(0),
    m_trkD0_LargeD0(0),
    m_trkZ0_LargeD0(0)
{  
  
}

StatusCode DV::TrkBasicPlots::initialize()
{
    // Histograming...
    ServiceHandle<ITHistSvc> histSvc("THistSvc",name()); 
    CHECK( histSvc.retrieve() );
    
    // Creating
    m_nTracks = new TH1I("nTracks", "; Number of tracks",2501,0,2500);  
    m_nSCTHitsOnTrack = new TH1F("nSCTHitsOnTrack", "; Number of SCT hits", 20,-0.5,19.5);  
    m_nPixVsSCTHitsOnTrack = new TH2F("nPixVsSCTHitsOnTrack", "; Number of Pixel hits; Number of SCT hits", 10,-0.5,9.5,20,-0.5,19.5);
    m_trkD0_all = new TH1F("trkD0_all","; Track d_{0} [mm]; ",100,-300,300.);  
    m_trkZ0_all = new TH1F("trkZ0_all","; Track z_{0} [mm]; ",500,-1500,1500.);  
    m_trkD0_SiSeeded = new TH1F("trkD0_SiSeeded","; Track d_{0} [mm]; ",100,-300,300.);
    m_trkD0_TRTSeeded = new TH1F("trkD0_TRTSeeded","; Track d_{0} [mm]; ",100,-300,300.);
    m_trkD0_TRTStandalone = new TH1F("trkD0_TRTStandalone","; Track d_{0} [mm]; ",100,-300,300.);
    m_trkD0_LargeD0 = new TH1F("trkD0_LargeD0","; Track d_{0} [mm]; ",100,-300,300.);
    m_trkZ0_LargeD0 = new TH1F("trkZ0_LargeD0","; Track z_{0} [mm]; ",500,-1500,1500.);
    
    // registing them in the file handler
    CHECK( histSvc->regHist("/TrkBasicPlots/m_nTracks",m_nTracks) );
    CHECK( histSvc->regHist("/TrkBasicPlots/nPixVsSCTHitsOnTrack",m_nPixVsSCTHitsOnTrack) );
    
    // let me be lazy... we have a lot of TH1Fs, easy to build map than do the registry...
    const std::vector<TH1F*> _histos = {m_nSCTHitsOnTrack, m_trkD0_all, m_trkZ0_all,
        m_trkD0_SiSeeded, m_trkD0_TRTSeeded, m_trkD0_TRTStandalone, m_trkD0_LargeD0, m_trkZ0_LargeD0 
    };

    for(auto & _h: _histos)
    {
        std::string _hpath("/TrkBasicPlots/"+std::string(_h->GetName()));
        CHECK( histSvc->regHist(_hpath,_h) );
    }

    // Cuts
    CHECK( m_dvcuts.retrieve() );

    return StatusCode::SUCCESS;
}


StatusCode DV::TrkBasicPlots::execute()  
{
    // get track container of interest
    const xAOD::TrackParticleContainer* recoTracks = 0;
    CHECK( evtStore()->retrieve( recoTracks, "InDetTrackParticles" ) );

    // Would you use the m_dvcuts tool ?

    // number of reco. tracks
    m_nTracks->Fill(recoTracks->size());
    // loop over the tracks in the container
    for( xAOD::TrackParticleContainer::const_iterator recoTrk_itr = recoTracks->begin();
            recoTrk_itr != recoTracks->end(); recoTrk_itr++) 
    {
        // Make your track selection
        if ( (*recoTrk_itr)->pt() < 1000 )
        {
            continue;
        }

        ATH_MSG_DEBUG("Track  (pT, eta, phi)  = (" << (*recoTrk_itr)->pt() 
                << "," << (*recoTrk_itr)->eta() << "," << (*recoTrk_itr)->phi() << ")" );
        
        int nPixHits=(int)((*recoTrk_itr)->auxdata<uint8_t>("numberOfPixelHits"));
        int nSCTHits=(int)((*recoTrk_itr)->auxdata<uint8_t>("numberOfSCTHits"));
    
        m_nSCTHitsOnTrack->Fill(nSCTHits);
        m_nPixVsSCTHitsOnTrack->Fill(nPixHits,nSCTHits);

        /// find the track author
        const std::bitset<xAOD::NumberOfTrackRecoInfo> patternReco = (*recoTrk_itr)->patternRecoInfo();
    
        if (patternReco.test(0))   /// Si-seeded
        {
            m_trkD0_SiSeeded->Fill((*recoTrk_itr)->d0());
        } 
        if (patternReco.test(4)) ///TRT-seeded (i.e. back-tracking)
        {
            m_trkD0_TRTSeeded->Fill((*recoTrk_itr)->d0());
        } 
        if (patternReco.test(20))  /// TRT Standalone
        {
            m_trkD0_TRTStandalone->Fill((*recoTrk_itr)->d0());
        } 
        if (patternReco.test(49))  /// Large d0
        {
            m_trkD0_LargeD0->Fill((*recoTrk_itr)->d0());
            m_trkZ0_LargeD0->Fill((*recoTrk_itr)->z0());
        }
        m_trkD0_all->Fill((*recoTrk_itr)->d0());
        m_trkZ0_all->Fill((*recoTrk_itr)->z0());
    }
    return StatusCode::SUCCESS;  
}

StatusCode DV::TrkBasicPlots::finalize() 
{
  return StatusCode::SUCCESS;  
}



