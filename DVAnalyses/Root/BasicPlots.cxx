
// To be used for RootCore ONLY
#ifdef ASGTOOL_STANDALONE

#include "DVAnalyses/BasicPlots.h"

#include "DVTools/PlotsManagerTool.h"

// EDM includes: 
#include "xAODTracking/VertexAuxContainer.h"
#include "xAODTracking/VertexContainer.h"

#include "xAODRootAccess/TStore.h"

#include <iostream>
#include <vector>

DV::BasicPlots::BasicPlots() 
{
}

void DV::BasicPlots::bookHists(DV::PlotsManagerTool * plotmanager) 
{
    plotmanager->setCurrentModule("BasicPlots");
    
    m_DVxy   = plotmanager->bookTH2<TH2F,float>("DVxy","; x [mm]; y [mm]", 300,-300.,300.,300,-300.,300.);
    m_DVmass = plotmanager->bookTH1<TH1F,float>("DVmass","; m [GeV]",100,0.,20.);
    
    h_nTrkAssocVertAll = plotmanager->bookTH1<TH1D,double>("h_nTrkAssocVertAll","; nTrk",48,2,50);
    h_DVmassVSnTrkAll  = plotmanager->bookTH2<TH2D,double>("h_DVmassVSnTrkAll","; nTrk; m_{DV} [GeV]",48,2,50,100,0,100);
}

void DV::BasicPlots::execute(xAOD::TEvent* thisEvent) 
{
    // get DV container of interest
    const xAOD::VertexContainer* recoVertices = 0;
    if( !thisEvent->retrieve( recoVertices, "RPVSecVertices" ).isSuccess() )
    {
        Error("execute()", "Failed to retrieve Reconstructed DV container. Exiting." );
        return;
    }
  
    // loop over the tracks in the container
    for( xAOD::VertexContainer::const_iterator dv_itr = recoVertices->begin();
            dv_itr != recoVertices->end(); dv_itr++) 
    {
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

void DV::BasicPlots::finalize() 
{  
}
#endif  // ASGTOOL_STANDALONE


