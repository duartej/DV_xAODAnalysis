
// To be used for RootCore ONLY
#ifdef ASGTOOL_STANDALONE

#include "DVAnalyses/BasicPlots.h"

#include "DVTools/PlotsManagerTool.h"

// EDM includes:
#include "xAODTracking/VertexAuxContainer.h"
#include "xAODTracking/VertexContainer.h"

#include <iostream>
#include <vector>

DV::BasicPlots::BasicPlots() :
    m_DVxy(nullptr),
    m_DVmass(nullptr),
    h_nTrkAssocVertAll(nullptr),
    h_DVmassVSnTrkAll(nullptr),
    m_handle_dvcuts("DV::IDVCuts/DVCuts")
{
    //declareProperty( "DVCuts", m_handle_dvcuts, "The name of the displaced vertices cuts (DVCuts) tool");
    //! just provisional uuntil we create an initialize method
    /*StatusCode sc = m_handle_dvcuts.retrieve();
    if( sc != StatusCode::SUCCESS )
    { std::cout << "Your in troubles! The DVCuts tool has not been retrieved" << std::endl; }*/

    // Declare the necessary cuts this method needs
    assignCutsAndTools();
}

void DV::BasicPlots::bookHists(DV::PlotsManagerTool * plotmanager)
{
    plotmanager->setCurrentModule("BasicPlots");

    m_DVxy   = plotmanager->bookTH2<TH2F>("DVxy","; x [mm]; y [mm]", 300,-300.,300.,300,-300.,300.);
    m_DVmass = plotmanager->bookTH1<TH1F>("DVmass","; m [GeV]",100,0.,20.);

    h_nTrkAssocVertAll = plotmanager->bookTH1<TH1D>("h_nTrkAssocVertAll","; nTrk",48,2,50);
    h_DVmassVSnTrkAll  = plotmanager->bookTH2<TH2D>("h_DVmassVSnTrkAll","; nTrk; m_{DV} [GeV]",48,2,50,100,0,100);
}

bool DV::BasicPlots::initialize()
{
    return true;
}

bool DV::BasicPlots::execute(xAOD::TEvent* thisEvent)
{
    // get DV container of interest
    const xAOD::VertexContainer* recoVertices = 0;
    if( !thisEvent->retrieve( recoVertices, "RPVSecVertices" ).isSuccess() )
    {
        Error("execute()", "Failed to retrieve Reconstructed DV container. Exiting." );
        return false;
    }

    // loop over the tracks in the container
    for( xAOD::VertexContainer::const_iterator dv_itr = recoVertices->begin();
            dv_itr != recoVertices->end(); dv_itr++)
    {
        // Testing some Cuts tools
        if( ! m_handle_dvcuts->PassFiducialCuts( (*(*dv_itr)) ) )
        {
            continue;
        }

        float dvx = (*dv_itr)->x();
        float dvy = (*dv_itr)->y();
        m_DVxy->Fill(dvx,dvy);

        double mass = (*dv_itr)->auxdataConst<double>("massPionHypo");
        m_DVmass->Fill(mass/1000.);

        long int nTrk = (*dv_itr)->auxdataConst< std::vector<long int> >("trackIndices").size();
        h_nTrkAssocVertAll->Fill(nTrk);

        h_DVmassVSnTrkAll->Fill(nTrk, mass/1000.);
    }

    return true;
}

bool DV::BasicPlots::finalize()
{
    return true;
}

void DV::BasicPlots::assignCutsAndTools()
{
    // Which cuts are needed? Remember the format ToolType/ToolName
    // (XXX:: so it can be retrieved from a property... )
    m_toolnames.push_back("DV::IDVCuts/DVCuts");
}
#endif  // ASGTOOL_STANDALONE


