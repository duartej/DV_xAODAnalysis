#include <iostream>
#include "DVAnalysis/TrkBasicPlots.h"


// EDM includes: - if move to header file will not compile?
#include "xAODEventInfo/EventInfo.h"
#include "xAODTracking/TrackMeasurementValidationContainer.h"
#include "xAODTracking/TrackStateValidationContainer.h"
#include "xAODTracking/TrackParticleContainer.h"
#include "xAODTracking/TrackParticleAuxContainer.h"

#include "xAODRootAccess/TStore.h"
#include "xAODCore/ShallowCopy.h"

#include <bitset>

DV::TrkBasicPlots::TrkBasicPlots() 
{
  m_histList = new TList();
  
}
void DV::TrkBasicPlots::bookHists() 
{

  m_nSCTHitsOnTrack = new TH1F("nSCTHitsOnTrack", "; Number of SCT hits", 20,-0.5,19.5);
  m_histList->Add(m_nSCTHitsOnTrack);
  
  m_nPixVsSCTHitsOnTrack = new TH2F("nPixVsSCTHitsOnTrack", "; Number of Pixel hits; Number of SCT hits", 10,-0.5,9.5,20,-0.5,19.5);
  m_histList->Add(m_nPixVsSCTHitsOnTrack);

  m_trkD0_all = new TH1F("trkD0_all","; Track d_{0} [mm]; ",100,-300,300.);
  m_histList->Add(m_trkD0_all);
  
  m_trkZ0_all = new TH1F("trkZ0_all","; Track z_{0} [mm]; ",500,-1500,1500.);
  m_histList->Add(m_trkZ0_all);
  
  m_trkD0_SiSeeded = new TH1F("trkD0_SiSeeded","; Track d_{0} [mm]; ",100,-300,300.);
  m_histList->Add(m_trkD0_SiSeeded);

  m_trkD0_TRTSeeded = new TH1F("trkD0_TRTSeeded","; Track d_{0} [mm]; ",100,-300,300.);
  m_histList->Add(m_trkD0_TRTSeeded);

  m_trkD0_TRTStandalone = new TH1F("trkD0_TRTStandalone","; Track d_{0} [mm]; ",100,-300,300.);
  m_histList->Add(m_trkD0_TRTStandalone);

  m_trkD0_LargeD0 = new TH1F("trkD0_LargeD0","; Track d_{0} [mm]; ",100,-300,300.);
  m_histList->Add(m_trkD0_LargeD0);

  m_trkZ0_LargeD0 = new TH1F("trkZ0_LargeD0","; Track z_{0} [mm]; ",500,-1500,1500.);
  m_histList->Add(m_trkZ0_LargeD0);
}

TList* 
DV::TrkBasicPlots::getHists() 
{
  return m_histList;
}


void 
DV::TrkBasicPlots::execute(xAOD::TEvent* thisEvent)  {
  
  // get track container of interest
  const xAOD::TrackParticleContainer* recoTracks = 0;
  if ( !thisEvent->retrieve( recoTracks, "InDetTrackParticles" ).isSuccess() ){ // retrieve arguments: container type, container key
    Error("execute()", "Failed to retrieve Reconstructed Track container. Exiting." );
    return;//// EL::StatusCode::FAILURE;
  }
  
  // loop over the tracks in the container
  for( xAOD::TrackParticleContainer::const_iterator recoTrk_itr = recoTracks->begin();
          recoTrk_itr != recoTracks->end(); recoTrk_itr++) {

    // Make your track selection
    if ( (*recoTrk_itr)->pt() < 1000 )
      continue;

    //        Info("execute()",  "Track  (pT, eta, phi)  = (%f, %f, %f)", (*recoTrk_itr)->pt(), (*recoTrk_itr)->eta(), (*recoTrk_itr)->phi() );
    
    int nPixHits=(int)((*recoTrk_itr)->auxdata<uint8_t>("numberOfPixelHits"));


    int nSCTHits=(int)((*recoTrk_itr)->auxdata<uint8_t>("numberOfSCTHits"));
    m_nSCTHitsOnTrack->Fill(nSCTHits);
    m_nPixVsSCTHitsOnTrack->Fill(nPixHits,nSCTHits);

    /// find the track author
    const std::bitset<xAOD::NumberOfTrackRecoInfo> patternReco = (*recoTrk_itr)->patternRecoInfo();
    
    if (patternReco.test(0)) {  /// Si-seeded
      m_trkD0_SiSeeded->Fill((*recoTrk_itr)->d0());
    } else if (patternReco.test(4)) { ///TRT-seeded (i.e. back-tracking)
      m_trkD0_TRTSeeded->Fill((*recoTrk_itr)->d0());
    } else if (patternReco.test(20)) { /// TRT Standalone
      m_trkD0_TRTStandalone->Fill((*recoTrk_itr)->d0());
    } else if (patternReco.test(49)) { /// Large d0
      m_trkD0_LargeD0->Fill((*recoTrk_itr)->d0());
      m_trkZ0_LargeD0->Fill((*recoTrk_itr)->z0());
    }
    m_trkD0_all->Fill((*recoTrk_itr)->d0());
    m_trkZ0_all->Fill((*recoTrk_itr)->z0());
  }
}

void 
DV::TrkBasicPlots::finalize() 
{
  
}



