#include <iostream>
#include <vector>
#include "DVAnalysis/DVBasicPlots.h"


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

DVBasicPlots::DVBasicPlots() 
{
  std::cout<<"in DVBasicPlots constructor"<<std::endl;
  m_histList = new TList();
  
}
void DVBasicPlots::bookHists() 
{
  m_DVxy = new TH2F("DVxy","; x [mm]; y [mm]", 300,-300.,300.,300,-300.,300.);
  m_histList->Add(m_DVxy);
  m_DVmass = new TH1F("DVmass","; m [GeV]",100,0.,20.);
  m_histList->Add(m_DVmass);
  
  h_nTrkAssocVertAll = new TH1D("h_nTrkAssocVertAll","; nTrk",48,2,50);
  m_histList->Add(h_nTrkAssocVertAll);
  h_DVmassVSnTrkAll = new TH2D("h_DVmassVSnTrkAll","; nTrk; m_{DV} [GeV]",48,2,50,100,0,100);
  m_histList->Add(h_DVmassVSnTrkAll);
}

TList* 
DVBasicPlots::getHists() 
{
  return m_histList;
}


void 
DVBasicPlots::execute(xAOD::TEvent* thisEvent) 
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
    //std::cout << "================================" << std::endl;
    //for (const auto& index : (*dv_itr)->auxdataConst< std::vector<long int> >("trackIndices")){
    //  std::cout << index << std::endl;
    //}
    h_nTrkAssocVertAll->Fill(nTrk);
    
    h_DVmassVSnTrkAll->Fill(nTrk, mass/1000.);
  }
}

void 
DVBasicPlots::finalize() 
{
  
}



