#include "DVAnalysis/DVPlusElectronBasic.h"

#include "DVAnalysis/PlotsManager.h"
#include "DVAnalysis/DummyCuts.h"


// EDM includes: - if move to header file will not compile?
#include "xAODEventInfo/EventInfo.h"
#include "xAODTracking/VertexContainer.h"


#include <iostream>
#include <TMath.h>
#include <TLorentzVector.h>
#include <math.h>
#include "TH1F.h"
#include "TH2F.h"

DVPlusElectronBasic::DVPlusElectronBasic() 
{
    m_isBlind=false;
    //m_histList = new TList();
    // Needed cuts ?
    m_cutnames.push_back("DummyCuts");
}

DVPlusElectronBasic::~DVPlusElectronBasic() 
{
    //if(m_histList != 0)
    //{
        //delete m_histList;
        //m_histList = 0;
    //}
}


void DVPlusElectronBasic::bookHists(DV::PlotsManager * plotmanager) 
{
    plotmanager->setCurrentModule("DVPlusElectronBasic");

    m_nPV = plotmanager->bookTH1F("nPV","nPV",20,-0.5,19.5);
    m_PVx = plotmanager->bookTH1F("PVx","PVx",100,-2.,2.);
    m_PVy = plotmanager->bookTH1F("PVy","PVy",100,-2.,2.);
    m_PVxy = plotmanager->bookTH2F("PVxy","PVxy",50,-2.,2.,50,-2,2);
    m_PVz = plotmanager->bookTH1F("PVz","PVz",100,-300.,300.);
    m_PVz_zWeighted = plotmanager->bookTH1F("PVz_zWeighted","PVz",100,-300.,300.);
    m_PVrSpread = plotmanager->bookTH1F("PVrSpread","PVrSpread",100,0.,1.);
    m_DVmass = plotmanager->bookTH1F("DVmass","massHist",100,0.,20.);
    m_DVmassPtCorr = plotmanager->bookTH1F("DVmassPtCorr","massHist",100,0.,20.);
    m_DVmassBig = plotmanager->bookTH1F("DVmassBig","massHist",100,0.,600.);
    m_DVmassBigPtCorr = plotmanager->bookTH1F("DVmassBigPtCorr","massHist",100,0.,600.);
    m_DVnTrk = plotmanager->bookTH1F("DVnTrk","nTrk",15,1.5,16.5);
    m_DVnTrkVsMass = plotmanager->bookTH2F("DVnTrkVsMass","nTrkVsMass",15,1.5,16.5,100,0.,50.);
    m_DVnTrkVsMassLog = plotmanager->bookTH2F("DVnTrkVsMassLog","nTrkVsMass",50,1.5,51.5,50,0.,500.);
    m_DVnTrkVsMassLogAllCuts = plotmanager->bookTH2F("DVnTrkVsMassLogAllCuts","nTrkVsMass",50,1.5,51.5,50,0.,500.);
    m_DVnTrkVsMassLogAllCutsExceptEl = plotmanager->bookTH2F("DVnTrkVsMassLogAllCutsExceptEl","nTrkVsMass",50,1.5,51.5,50,0.,500.);
    m_DVnTrkVsMassLogAllCutsExceptElMatch = plotmanager->bookTH2F("DVnTrkVsMassLogAllCutsExceptElMatch","nTrkVsMass",50,1.5,51.5,50,0.,500.);
    m_DVnTrkVsMassLogAllCutsUpToNtrk = plotmanager->bookTH2F("DVnTrkVsMassLogAllCutsUpToNtrk","nTrkVsMass",50,1.5,51.5,50,0.,500.);
    m_DVrz = plotmanager->bookTH2F("DVrz","rDV",100,0.,300.,100,0.,180.);
    m_DVxy = plotmanager->bookTH2F("DVxy","xyDV",400,-200.,200.,400,-200.,200.);
    m_DVxy_KsGamVeto = plotmanager->bookTH2F("DVxy_KsGamVeto","xyDV",400,-200.,200.,400,-200.,200.);
    m_DVrphi = plotmanager->bookTH2F("DVrphi","rphi; #phi' r_{DV} [mm]",600,-3.15,3.15,400,0.,200.);
    m_DVrphi_KsGamVeto = plotmanager->bookTH2F("DVrphi_KsGamVeto","; #phi; r_{DV} [mm]",600,-3.15,3.15,400,0.,200.);
    m_DVr = plotmanager->bookTH1F("DVr","rDV",100,0.,180.);
    m_DVz = plotmanager->bookTH1F("DVz","zDV",100,-300.,300.);
    m_averageIntPerXing = plotmanager->bookTH1F("averageIntPerXing","averageInt",60,0.,30.);
    m_actualIntPerXing = plotmanager->bookTH1F("actualIntPerXing","averageInt",60,0.,30.);
    m_DVr_GE3Trk = plotmanager->bookTH1F("DVr_GE3Trk","DVr_GE3Trk",100,0.,180.);
    m_DVnTrk_KsGamVeto = plotmanager->bookTH1F("DVnTrk_KsGamVeto","DVnTrk",14,0.5,14.5);
    m_pileupWeights = plotmanager->bookTH1F("pileupWeights","; pileup weights",100,0.,3.);
    m_zpvWeights = plotmanager->bookTH1F("zpvWeights","; zPV weights",100,0.,3.);
    m_elecWeights = plotmanager->bookTH1F("elecWeights","; elec weights",100,0.,3.);

    /// rebin nTrk vs Mass for equal sized bins on log scale
    Float_t limits[50];
    for (int i=0; i<50; ++i) 
    {
        limits[i] = 500.0* TMath::Exp((i-49.0)/6.53);
    }
    m_DVnTrkVsMassLog->GetYaxis()->Set(49,limits);
    m_DVnTrkVsMassLogAllCuts->GetYaxis()->Set(49,limits);
    
    //m_histList->Add(m_nPV);
    //m_histList->Add(m_PVx);
    //m_histList->Add(m_PVxy);
    //m_histList->Add(m_PVy);
    //m_histList->Add(m_PVz);
    //m_histList->Add(m_PVz_zWeighted);
    //m_histList->Add(m_PVrSpread);
    //m_histList->Add(m_DVmass);
    //m_histList->Add(m_DVmassPtCorr);
    //m_histList->Add(m_DVmassBig);
    //m_histList->Add(m_DVmassBigPtCorr);
    //m_histList->Add(m_DVnTrk);
    //m_histList->Add(m_DVnTrkVsMass);
    //m_histList->Add(m_DVnTrkVsMassLog);
    //m_histList->Add(m_DVnTrkVsMassLogAllCuts);
    //m_histList->Add(m_DVnTrkVsMassLogAllCutsExceptEl);
    //m_histList->Add(m_DVnTrkVsMassLogAllCutsExceptElMatch);
    //m_histList->Add(m_DVnTrkVsMassLogAllCutsUpToNtrk);
    //m_histList->Add(m_DVrz);    
    //m_histList->Add(m_DVxy);  
    //m_histList->Add(m_DVxy_KsGamVeto);  
    //m_histList->Add(m_DVrphi);  
    //m_histList->Add(m_DVrphi_KsGamVeto);  
    //m_histList->Add(m_DVr);  
    //m_histList->Add(m_DVz);
    //m_histList->Add(m_averageIntPerXing);
    //m_histList->Add(m_actualIntPerXing);
    //m_histList->Add(m_DVr_GE3Trk);
    //m_histList->Add(m_DVnTrk_KsGamVeto);
    //m_histList->Add(m_pileupWeights);
    //m_histList->Add(m_zpvWeights);
    //m_histList->Add(m_elecWeights);
}

/*TList* DVPlusElectronBasic::getHists() 
{
    return m_histList;
}*/


void DVPlusElectronBasic::execute(xAOD::TEvent * thisEvent) 
{
    // Event info
    const xAOD::EventInfo * eventInfo = 0;
    if( ! (thisEvent->retrieve( eventInfo,"EventInfo" ).isSuccess()) )
    {
        Error("execute()","Failed to retrieved EventInfo. Exiting...");
        return;
    }
    if( eventInfo == 0 )
    {
        Error("execute()","eventInfo is a NULL pointer. Exiting...");
        return;
    }


    bool isMC = eventInfo->eventType(xAOD::EventInfo::IS_SIMULATION);

    // Retrieve primary vertex collection
    const xAOD::VertexContainer * primVtx = 0;
    if( ! (thisEvent->retrieve(primVtx,"PrimaryVertices" ).isSuccess()) )
    {
        Error("execute()","Failed to retrieved primary vertices container. Exiting...");
        return;
    }
    if( primVtx == 0 )
    {
        Error("execute()","primary vertices is a NULL pointer. Exiting...");
        return;
    }

    m_nPV->Fill(primVtx->size()-1);
    if(primVtx->size() == 1)
    {
        return;
    }
    // Primary vertex ordered? - FIXME:: check it !!
    float firstX=(*primVtx)[0]->x();
    float firstY=(*primVtx)[0]->y();
    float maxDist=0.;

    float zPVWeight=1.0;
    float pileupWeight=1.0;

    /*if(isMC) 
    {
        zPVWeight = m_utils->GetZPVWeight((*primVtx)[0].z());
        m_zpvWeights->Fill(zPVWeight);
        pileupWeight=m_utils->GetPileupWeight();
        m_pileupWeights->Fill(pileupWeight);
    }*/
    m_PVx->Fill((*primVtx)[0]->x());
    m_PVy->Fill((*primVtx)[0]->y());
    m_PVxy->Fill((*primVtx)[0]->x(),(*primVtx)[0]->y());
    m_PVz->Fill((*primVtx)[0]->z());
    m_PVz_zWeighted->Fill((*primVtx)[0]->z(),zPVWeight);

    for(unsigned int i=0; i < primVtx->size()-1; ++i) 
    {
        // Distance between the first PV and the rest
        float dist = TMath::Sqrt(((firstX-(*primVtx)[i]->x())*(firstX-(*primVtx)[i]->x()))
                +((firstY-(*primVtx)[i]->y())*(firstY-(*primVtx)[i]->y())));
        if(dist > maxDist) 
        {
            maxDist = dist;
        }
    }
    m_PVrSpread->Fill(maxDist);
  
    m_averageIntPerXing->Fill(eventInfo->averageInteractionsPerCrossing());
    m_actualIntPerXing->Fill(eventInfo->actualInteractionsPerCrossing());

    /*bool hasGoodElectron=false;
  std::vector<int> goodElectronIndices;
  for (int i=0; i< m_elec->n(); ++i) {
    
    if (m_cuts->PassesElectronCuts(i)) {
      hasGoodElectron=true;
      goodElectronIndices.push_back(i);
      if (m_eventInfo->isSimulation()) m_elecWeights->Fill(m_utils->GetElectronEffWeight(i));
    }
  }

  if (! m_cuts->PassesGRL()) return;
  if (! m_cuts->PassesPVCuts()) return;
  for (int i=0; i< m_dispVtx->n(); ++i) {
    if ((m_isBlind) && (m_cuts->ApplyBlinding(i))) continue;
    if (! m_cuts->PassesChisqCut(i)) continue;
    if (! m_cuts->PassesFiducialCuts(i)) continue;
    if (! m_cuts->PassesRDVDistCut(i)) continue;

    m_DVmass->Fill((*m_dispVtx)[i].m_PionHypo()/1000.);
        Double_t ptCorrMass = CalcPtCorrMass(i);
    m_DVmassPtCorr->Fill(ptCorrMass/1000.);
        if ((*m_dispVtx)[i].nTracks()>=5) {
      m_DVmassBig->Fill((*m_dispVtx)[i].m_PionHypo()/1000.);
      m_DVmassBigPtCorr->Fill(ptCorrMass/1000.);
    }
    m_DVnTrk->Fill((*m_dispVtx)[i].nTracks());
    m_DVnTrkVsMass->Fill((*m_dispVtx)[i].nTracks(), (*m_dispVtx)[i].m_PionHypo()/1000.);
    m_DVnTrkVsMassLog->Fill((*m_dispVtx)[i].nTracks(), (*m_dispVtx)[i].m_PionHypo()/1000.);
    m_DVxy->Fill((*m_dispVtx)[i].x(),(*m_dispVtx)[i].y());

    float rDV = pow((*m_dispVtx)[i].x()*(*m_dispVtx)[i].x()+(*m_dispVtx)[i].y()*(*m_dispVtx)[i].y(),0.5);
    float phiDV = TMath::ATan2((*m_dispVtx)[i].y(),(*m_dispVtx)[i].x());

    m_DVrphi->Fill(phiDV,rDV);
    m_DVrz->Fill(fabs((*m_dispVtx)[i].z()),rDV);
    m_DVr->Fill(rDV);
    m_DVz->Fill((*m_dispVtx)[i].z());

    if ((*m_dispVtx)[i].nTracks() > 2)
      m_DVr_GE3Trk->Fill(rDV);
    if (m_cuts->PassesKsGamVeto(i)) {
      m_DVxy_KsGamVeto->Fill((*m_dispVtx)[i].x(),(*m_dispVtx)[i].y());
      m_DVrphi_KsGamVeto->Fill(phiDV,rDV);
      m_DVnTrk_KsGamVeto->Fill((*m_dispVtx)[i].nTracks());
    }
    if (!m_cuts->PassesTrigger()) continue;

        if (hasGoodElectron) {
      m_DVnTrkVsMassLogAllCutsUpToNtrk->Fill((*m_dispVtx)[i].nTracks(), (*m_dispVtx)[i].m_PionHypo()/1000.);
    }
    if (!m_cuts->PassesMaterialVeto(i)) continue;

    m_DVnTrkVsMassLogAllCutsExceptEl->Fill((*m_dispVtx)[i].nTracks(), (*m_dispVtx)[i].m_PionHypo()/1000.);
    if (hasGoodElectron) {
      m_DVnTrkVsMassLogAllCutsExceptElMatch->Fill((*m_dispVtx)[i].nTracks(), (*m_dispVtx)[i].m_PionHypo()/1000.);
      std::vector<int>::iterator goodElIt = goodElectronIndices.begin();
      for ( ; goodElIt != goodElectronIndices.end(); ++goodElIt) {
        // if ((m_cuts->PassesVtxElecCut(i,*goodElIt))&&(m_cuts->PassesCosmicsVeto(i,*goodElIt)))
        if ((m_cuts->PassesVtxElectronCut(i,*goodElIt)))
          m_DVnTrkVsMassLogAllCuts->Fill((*m_dispVtx)[i].nTracks(), (*m_dispVtx)[i].m_PionHypo()/1000.);
      }
    }   



  } 
  return;*/
}

/*Double_t DVPlusElectronBasic::CalcPtCorrMass(int i) {
  TVector3 PVtoDV((*m_dispVtx)[i].x() - (*m_primVtx)[0].x(),
                  (*m_dispVtx)[i].y() - (*m_primVtx)[0].y(),
                  (*m_dispVtx)[i].z() - (*m_primVtx)[0].z());
  TVector3 DVp3((*m_dispVtx)[i].px(),(*m_dispVtx)[i].py(),(*m_dispVtx)[i].pz());
  
  TVector3 DVpT = PVtoDV.Unit().Cross(DVp3);

  Double_t pTcorrMass = TMath::Sqrt((*m_dispVtx)[i].m_PionHypo()*(*m_dispVtx)[i].m_PionHypo()
                                    +DVpT.Mag2()) + DVpT.Mag();
  return pTcorrMass;

}*/

void DVPlusElectronBasic::finalize()
{
}

void DVPlusElectronBasic::assignCuts()
{
    checkCutAvailability("DummyCuts","DVPlusElectronBasic");
    m_cuts = static_cast<const DV::DummyCuts*>(this->m_cutsMap["DummyCuts"]);
}
