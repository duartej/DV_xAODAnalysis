#include "DVAnalysis/DVCutsBase.h"

#include "GoodRunsLists/TGoodRunsListReader.h"
#include "GoodRunsLists/TGoodRunsList.h"

// ROOT related
#include "TFile.h"
#include "TH3F.h"

#include<iostream>


DVCutsBase::DVCutsBase():
    m_GRL(0),
    m_materialMap(0)
{
}

void DVCutsBase::Init(const char * materialfile,const char * goodlistfile)
{
    /// get the material map
    if( ! getMaterialMap(materialfile) )
    {
        std::cout<<"DVCutsBase WARNING: not using material map" << std::endl;
    }
  
    if(goodlistfile == NULL) 
    {
        return;
    }
    
    /// Get the Good List reader
    Root::TGoodRunsListReader * grl_R = new Root::TGoodRunsListReader();
    //std::string sname = "data12_8TeV.periodAllYear_DetStatus-v61-pro14-02_DQDefects-00-01-00_PHYS_StandardGRL_All_Good.xml";
    ///data12_8TeV.periodAllYear_DetStatus-v58-pro14-01_DQDefects-00-00-33_PHYS_StandardGRL_All_Good.xml";
    //  std::string sname = "data11_7TeV.periodAllYear_goodRPC.xml";
    ///data11_7TeV.periodAllYear_DetStatus-v36-pro09-03_CoolRunQuery-00-04-08_Muon.xml";
    std::cout << "DVCutsBase INFO: Good Run List XML to load '" << goodlistfile << std::endl;
    grl_R->SetXMLFile(goodlistfile);
    grl_R->Interpret();
    m_GRL = new Root::TGoodRunsList(grl_R->GetMergedGoodRunsList());
    m_GRL->Summary(false);
    delete grl_R;
    grl_R = 0;
}

bool DVCutsBase::getMaterialMap(const char *filename) 
{
    if( filename == NULL )
    {
        return false;
    }
    
    TFile * mapfile = new TFile(filename); 
    m_materialMap = (TH3F*)(mapfile->Get("map"));
    if(m_materialMap == NULL)
    {
        std::cout<<"DVCutsBase WARNING: PROBLEM GETTING MATERIAL MAP from"
            << " '" << filename << "'" << std::endl;
        mapfile->Close();
        return false;
    }
    m_materialMap->SetDirectory(0);
    mapfile->Close();
    return true;
}

//Bool_t DVCutsBase::PassesFiducialCuts(int i) {
//
//  Float_t xDV = (*m_dispVtx)[i].x();
//  Float_t yDV = (*m_dispVtx)[i].y();
//  Float_t zDV = (*m_dispVtx)[i].z();
//
//  //  if (TMath::Sqrt((xDV*xDV)+(yDV*yDV)) > 180.) return false;
//  if (TMath::Sqrt((xDV*xDV)+(yDV*yDV)) > 300.) return false;
//  
//  if (fabs(zDV) > 300.) return false;
//
//  return true;
//}
//
//Bool_t DVCutsBase::PassesChisqCut(int i) {
//  float chisqPerDOF = (*m_dispVtx)[i].chi2() / (*m_dispVtx)[i].ndof();
//  if (chisqPerDOF > 5.0) return false;
//  return true;
//}
//
//Bool_t DVCutsBase::PassesRDVDistCut(int i) {
//  Float_t xDV = (*m_dispVtx)[i].x();
//  Float_t yDV = (*m_dispVtx)[i].y();
//  for (int j=0; j< m_primVtx->n()-1; ++j) {
//    Float_t xPV = (*m_primVtx)[j].x();
//    Float_t yPV = (*m_primVtx)[j].y();    
//    if (TMath::Sqrt(((xPV-xDV)*(xPV-xDV))+((yPV-yDV)*(yPV-yDV))) < 4.0) return kFALSE;
//  }
//  return kTRUE;
//  //  Float_t xPV = (*m_primVtx)[0].x();
//  // Float_t yPV = (*m_primVtx)[0].y();    
//  // return (TMath::Sqrt(((xPV-xDV)*(xPV-xDV))+((yPV-yDV)*(yPV-yDV))) > 4.0);
//}
//
//
//
//
//Bool_t DVCutsBase::PassesGRL() {
//  
//  if ((! m_eventInfo->isSimulation()) && (!m_useGRL)) {
//    std::cout<<"Job is not configured to use GRL!"<<std::endl;
//    return false;
//  }
//  return (m_eventInfo->isSimulation() || m_grl.HasRunLumiBlock(m_eventInfo->RunNumber(),m_eventInfo->lbn()));
//
//}
//
////For Jets + MET
////LAr noise burst, tile corrupted events, incomplete events and 
////https://twiki.cern.ch/twiki/bin/viewauth/Atlas/DataPreparationCheckListForPhysicsAnalysis
//Bool_t DVCutsBase::PassesEventCleaning() {
//  if (m_eventInfo->larError()==2) return false;
//  if (m_eventInfo->tileError()==2) return false;
//  if ((m_eventInfo->coreFlags()&0x40000) != 0) return false;
//  if (!m_utils->PassesTileTripEvent(m_eventInfo->RunNumber(), m_eventInfo->lbn(), m_eventInfo->EventNumber())) return false;
//  return true;
//}
//
//Bool_t DVCutsBase::PassesPVCuts() {
//
//  if (! PassesNPVCut()) return false;
//  if (! PassesZPVCut()) return false;
//  if (! PassesPVnTrkCut() ) return false;
//  return true;
//}
//
//Bool_t DVCutsBase::PassesNPVCut() {
//  return (m_primVtx->n() > 1);
//}
//
//Bool_t DVCutsBase::PassesZPVCut() {
//  return (fabs((*m_primVtx)[0].z()) < 200.);
//}
//
//Bool_t DVCutsBase::PassesPVnTrkCut() {
//  return ((*m_primVtx)[0].nTracks() > 5);
//}
//
//
//
//Bool_t DVCutsBase::PassesMaterialVeto(int i) { ////Float_t zDV, Float_t rDV) {
//
//  //// TEMPORARY ////
//  //  return kTRUE;
//
//  if (m_materialMap==0) std::cout<<"NULL MATERIAL MAP!!"<<std::endl;
//
//  float rDV = pow((*m_dispVtx)[i].x()*(*m_dispVtx)[i].x()+(*m_dispVtx)[i].y()*(*m_dispVtx)[i].y(),0.5);
//  float zDV = (*m_dispVtx)[i].z();
//  float phiDV=TMath::ATan2((*m_dispVtx)[i].y(),(*m_dispVtx)[i].x());
//  if (phiDV < 0) phiDV = 2*TMath::Pi() + phiDV;
//  
//  if (m_materialMap->GetBinContent(m_materialMap->FindBin(rDV,phiDV,zDV)) > 0)
//    return kFALSE;
// 
//  return kTRUE;
//}
//
//int DVCutsBase::GetRegion(int i) {
//  float rDV = pow((*m_dispVtx)[i].x()*(*m_dispVtx)[i].x()+(*m_dispVtx)[i].y()*(*m_dispVtx)[i].y(),0.5);
//  Bool_t nonMaterial = PassesMaterialVeto(i);
//  int rIndex = -1;
//  if (rDV<26. && nonMaterial) rIndex=0;
//  else if (rDV<37. && !nonMaterial) rIndex=1;
//  else if (rDV<46. && nonMaterial) rIndex=2;
//  else if (rDV<73. && !nonMaterial) rIndex=3;
//  else if (rDV<84. && nonMaterial) rIndex=4;
//  else if (rDV<111. && !nonMaterial) rIndex=5;
//  else if (rDV<120. && nonMaterial) rIndex=6;
//  else if (rDV<145. && !nonMaterial) rIndex=7;
//  else if (rDV<180. && nonMaterial) rIndex=8;
//  else if (rDV<300. && nonMaterial) rIndex=9;
//  return rIndex;
//}
