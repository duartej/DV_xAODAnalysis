#include <iostream>
#include <vector>
#include "DVAnalysis/BasicPlots.h"

#include "DVAnalysis/PlotsManager.h"

// EDM includes: - if move to header file will not compile?
#include "xAODEventInfo/EventInfo.h"
#include "xAODTracking/TrackMeasurementValidationContainer.h"
#include "xAODTracking/TrackStateValidationContainer.h"
#include "xAODTracking/TrackParticleContainer.h"
#include "xAODTracking/TrackParticleAuxContainer.h"
#include "xAODTracking/VertexAuxContainer.h"
#include "xAODTracking/VertexContainer.h"

#include "xAODRootAccess/TStore.h"
#include "xAODCore/ShallowCopy.h"

DV::BasicPlots::BasicPlots() 
{
  //m_histList = new TList();
}
void DV::BasicPlots::bookHists(DV::PlotsManager * plotmanager) 
{
  plotmanager->setCurrentModule("BasicPlots");

  m_DVxy   = plotmanager->bookTH2F("DVxy","; x [mm]; y [mm]", 300,-300.,300.,300,-300.,300.);
  m_DVmass = plotmanager->bookTH1F("DVmass","; m [GeV]",100,0.,20.);
  
  h_nTrkAssocVertAll = plotmanager->bookTH1F("h_nTrkAssocVertAll","; nTrk",48,2,50);
  h_DVmassVSnTrkAll  = plotmanager->bookTH2F("h_DVmassVSnTrkAll","; nTrk; m_{DV} [GeV]",48,2,50,100,0,100);
}

/*TList* 
DV::BasicPlots::getHists() 
{
  return m_histList;
}*/


void 
DV::BasicPlots::execute(xAOD::TEvent* thisEvent) 
{
   // get DV container of interest
  const xAOD::VertexContainer* recoVertices = 0;
  if ( !thisEvent->retrieve( recoVertices, "RPVSecVertices" ).isSuccess() ){ // retrieve arguments: container type, container key
    Error("execute()", "Failed to retrieve Reconstructed DV container. Exiting." );
    return;
  }
  
  // loop over the tracks in the container
  for( xAOD::VertexContainer::const_iterator dv_itr = recoVertices->begin();
       dv_itr != recoVertices->end(); dv_itr++) {
    float dvx = (*dv_itr)->x();
    float dvy = (*dv_itr)->y();
    m_DVxy->Fill(dvx,dvy);

    double mass = (*dv_itr)->auxdataConst<double>("massPionHypo");
    m_DVmass->Fill(mass/1000.);

    long int nTrk = (*dv_itr)->auxdataConst< std::vector<long int> >("trackIndices").size();
    h_nTrkAssocVertAll->Fill(nTrk);
    
    h_DVmassVSnTrkAll->Fill(nTrk, mass/1000.);
  }
}

void 
DV::BasicPlots::finalize() 
{
  
}



