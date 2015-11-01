#include <iostream>
#include "DVAnalysis/LowMassDVEff.h"

// EDM includes: - if move to header file will not compile?
#include "xAODEventInfo/EventInfo.h"
#include "xAODTracking/TrackMeasurementValidationContainer.h"
#include "xAODTracking/TrackStateValidationContainer.h"
#include "xAODTracking/TrackParticleContainer.h"
#include "xAODTracking/TrackParticleAuxContainer.h"
#include "xAODTracking/VertexAuxContainer.h"
#include "xAODTracking/VertexContainer.h"
#include "xAODTruth/TruthParticleContainer.h"
#include "xAODTruth/TruthVertexContainer.h"

#include "xAODRootAccess/tools/Message.h"
#include "xAODRootAccess/TStore.h"
#include "xAODCore/ShallowCopy.h"

#include "PathResolver/PathResolver.h"

// Root
#include "TEnv.h"

LowMassDVEff::LowMassDVEff() 
{
  std::cout<<"in LowMassDVEff constructor"<<std::endl;
  m_histList = new TList();
  
  m_configFile = "DVAnalysis/LowMassDV.conf";
  m_configFile = (PathResolverFindCalibFile(m_configFile)).c_str();
  readConfig();
}
void LowMassDVEff::bookHists() 
{
  int nbinsR = 60; double minR = 0.; double maxR = 300.;
  h_recoVertPosR = new TH1D("h_recoVertPosR","Reconstructed Vertex Position in R;R [mm]", nbinsR,minR,maxR);
  m_histList->Add(h_recoVertPosR);
  h_truthVertPosR = new TH1D("h_truthVertPosR","Truth Vertex Position in R;R [mm]", nbinsR,minR,maxR);
  m_histList->Add(h_truthVertPosR);
  h_diffVertPosR = new TH1D("h_diffVertPosR",";truthVertR - recoVertR [mm]", 200,-10,10);
  m_histList->Add(h_diffVertPosR);
  h_diffVertPosZ = new TH1D("h_diffVertPosZ",";truthVertZ - recoVertZ [mm]", 300,-15,15);
  m_histList->Add(h_diffVertPosZ);
  h_diffVertPos3D = new TH1D("h_diffVertPos3D",";distance between truthVert & recoVert [mm]", 300,0,30);
  m_histList->Add(h_diffVertPos3D);
  h_recoMatchedVertMass = new TH1D("h_recoMatchedVertMass","; m [GeV]",100,0.,20.);
  m_histList->Add(h_recoMatchedVertMass);

  h_nChargedParticlesFromLLP = new TH1D("h_nChargedParticlesFromLLP","; nChargedParticlesFromLLP",50,0.,50.);
  m_histList->Add(h_nChargedParticlesFromLLP);
  h_nChargedParticlesFromLLP1GeVCut = new TH1D("h_nChargedParticlesFromLLP1GeVCut","; nChargedParticlesFromLLP",50,0.,50.);
  m_histList->Add(h_nChargedParticlesFromLLP1GeVCut);
  h_nChargedParticlesFromLLP1GeVCutVSradius = new TH2D("h_nChargedParticlesFromLLP1GeVCutVSradius","; R [mm];nChargedParticlesFromLLP",nbinsR,minR,maxR,50,0.,50.);
  m_histList->Add(h_nChargedParticlesFromLLP1GeVCutVSradius);
  h_PtFromLLP = new TH1D("h_PtFromLLP","; Pt [GeV]",100,0.,100.);
  m_histList->Add(h_PtFromLLP);
  h_nSumChargedParticlesVSradius = new TH1D("h_nSumChargedParticlesVSradius","; R [mm]",nbinsR,minR,maxR);
  m_histList->Add(h_nSumChargedParticlesVSradius);
  
  h_nTrkAssocVertMatched = new TH1D("h_nTrkAssocVertMatched","; nTrk",50,0.,50.);
  m_histList->Add(h_nTrkAssocVertMatched);
  h_PtAssocVertMatched = new TH1D("h_PtAssocVertMatched","; Pt [GeV]",100,0.,100.);
  m_histList->Add(h_PtAssocVertMatched);
  h_nSumTrkVSradius = new TH1D("h_nSumTrkVSradius","; R [mm]",nbinsR,minR,maxR);
  m_histList->Add(h_nSumTrkVSradius);
  
  h_nTrkMatchedVSradius = new TH2D("h_nTrkMatchedVSradius","; R [mm]; nTrk",nbinsR,minR,maxR,50,0,50);
  m_histList->Add(h_nTrkMatchedVSradius);
  h_DVmassVSnTrkMatched = new TH2D("h_DVmassVSnTrkMatched","; nTrk; m_{DV} [GeV]",50,0,50,100,0,100);
  m_histList->Add(h_DVmassVSnTrkMatched);
  //g_vertEffR = nullptr;
  //m_histList->Add(g_vertEffR);

}

TList* 
LowMassDVEff::getHists() 
{
  return m_histList;
}


void 
LowMassDVEff::execute(xAOD::TEvent* thisEvent) 
{
   // get DV container of interest
  const xAOD::VertexContainer* recoVertices = 0;
  if ( !thisEvent->retrieve( recoVertices, "RPVSecVertices" ).isSuccess() ){ // retrieve arguments: container type, container key
    Error("execute()", "Failed to retrieve Reconstructed DV container. Exiting." );
    return;
  }
  // truth Particle container
  const xAOD::TruthParticleContainer* truthParticles = 0;
  if ( !thisEvent->retrieve( truthParticles, "TruthParticles" ).isSuccess() ){ // retrieve arguments: container type, container key
    Error("execute()", "Failed to retrieve truth particle container. Exiting." );
    return;
  }
  // get track container of interest
  const xAOD::TrackParticleContainer* recoTracks = 0;
  if ( !thisEvent->retrieve( recoTracks, "InDetTrackParticles" ).isSuccess() ){ // retrieve arguments: container type, container key
    Error("execute()", "Failed to retrieve Reconstructed Track container. Exiting." );
    return;//// EL::StatusCode::FAILURE;
  }
  
  typedef ElementLink< xAOD::TruthParticleContainer > Link_t;
  for(const auto& truthParticle : *truthParticles){
    if(isLLP(truthParticle->pdgId()) && truthParticle->hasDecayVtx()){
      //std::cout << "----------------------------" << truthParticle->pdgId() << std::endl;
      int nChargedParticles = 0;
      int nChargedParticles1GeVCut = 0;
      for(size_t ii = 0; ii < truthParticle->decayVtx()->nOutgoingParticles(); ii++){
        if(isLLP(truthParticle->decayVtx()->outgoingParticle(ii)->pdgId())){
          continue;
        }
        if(truthParticle->decayVtx()->outgoingParticle(ii)->isCharged()){
          h_PtFromLLP->Fill(truthParticle->decayVtx()->outgoingParticle(ii)->pt() * 0.001);
          nChargedParticles++;
          if(truthParticle->decayVtx()->outgoingParticle(ii)->pt() * 0.001 > 1.){
            nChargedParticles1GeVCut++;
          }
        }
        //std::cout << truthParticle->decayVtx()->outgoingParticle(ii)->pdgId() << std::endl;
      } // LLP is not included in outgoingParticles
      double truthVertX = truthParticle->decayVtx()->x();
      double truthVertY = truthParticle->decayVtx()->y();
      double truthVertZ = truthParticle->decayVtx()->z();
      double truthVertR = TMath::Sqrt(truthVertX * truthVertX + truthVertY * truthVertY);
      h_truthVertPosR->Fill(truthVertR);
      h_nChargedParticlesFromLLP->Fill(nChargedParticles);
      h_nChargedParticlesFromLLP1GeVCut->Fill(nChargedParticles1GeVCut);
      h_nChargedParticlesFromLLP1GeVCutVSradius->Fill(truthVertR,nChargedParticles1GeVCut); // TH2
      h_nSumChargedParticlesVSradius->Fill(truthVertR, nChargedParticles);
      for(const auto& recoVertex : *recoVertices){
        double recoVertX = recoVertex->x();
        double recoVertY = recoVertex->y();
        double recoVertZ = recoVertex->z();
        double recoVertR = TMath::Sqrt(recoVertX * recoVertX + recoVertY * recoVertY);
        //if (getDistance3D(truthVertX, truthVertY, truthVertZ,
        //                  recoVertX, recoVertY, recoVertZ) < 10.) { // if distance < 1 cm
        int nMatched = 0;
        for(const auto trackIndex : recoVertex->auxdataConst< std::vector<long int> >("trackIndices") ){
          const xAOD::TrackParticle *recoTrack = recoTracks->at(trackIndex);
          if (recoTrack->auxdataConst<float>("truthMatchProbability") < m_truthMatchProbCut) continue;
          const Link_t& link = recoTrack->auxdata< Link_t >("truthParticleLink");
          if(! link.isValid() ){
            continue;
          }
          const xAOD::TruthParticle* truth = *link;
          //std::cout << truth->parent()->pdgId() << std::endl;
          //bool fromLLP = false;
          //for(size_t jj = 0; jj < truth->prodVtx()->nIncomingParticles(); jj++){
          //  if(isLLP(truth->prodVtx()->incomingParticle(jj)->pdgId())) {
          //    fromLLP = true;
          //  }
          //}
          if(isLLP(truth->parent()->pdgId()))
          //if(fromLLP)
          {
            h_PtAssocVertMatched->Fill(truth->pt() * 0.001);
            nMatched++;
          }
          //} // end if prob > 0.99
        } // end for loop over trackIndices
        if (nMatched >= 2) {
          h_diffVertPosR->Fill(truthVertR - recoVertR);
          h_diffVertPosZ->Fill(truthVertZ - recoVertZ);
          h_diffVertPos3D->Fill(getDistance3D(truthVertX, truthVertY, truthVertZ, recoVertX, recoVertY, recoVertZ));
          //h_diffVertPosR->Fill(truthVertR - recoVertR);
          if (getDistance3D(truthVertX, truthVertY, truthVertZ,
                            recoVertX, recoVertY, recoVertZ) < 5.) { // if distance < 5 mm
            h_recoVertPosR->Fill(truthVertR);
            double mass = recoVertex->auxdataConst<double>("massPionHypo");
            h_recoMatchedVertMass->Fill(mass/1000.);
            int nTrk = recoVertex->auxdataConst< std::vector<long int> >("trackIndices").size();
            h_nTrkAssocVertMatched->Fill(nTrk);
            h_nSumTrkVSradius->Fill(truthVertR,nTrk); // TH1
            h_nTrkMatchedVSradius->Fill(truthVertR,nTrk); // TH2
            h_DVmassVSnTrkMatched->Fill(nTrk,mass/1000.);
            break;
          } // end if distance < 3 mm
        }
        //} // end if distance < 1 cm
      } // end loop over recoVertices
    } // endl if isLLP && hasDecayVtx
  } // end loop over truthParticles

}

void 
LowMassDVEff::finalize() 
{
  
}

void LowMassDVEff::readConfig() {
  Info("LowMassDVEff::readConfig()","*****     *****     *****     *****" );
  Info("readConfig()", "Configuring from file %s", m_configFile.c_str() );
  
  TEnv rEnv;
  int success = -1;
  success = rEnv.ReadFile(m_configFile.c_str(), kEnvAll);
  if(success != 0) {
    Error("readConfig()", "Config file cannot be opened");
    return;
  }
  
  Info("readConfig()", "Config file was opened successfully" );
  
  // basic configuration
  m_longlivedParticle = rEnv.GetValue( "longlivedParticle", "gluino" );
  //m_vertexContainer = rEnv.GetValue( "vertexContainer", "RPVSecVertices" );
  //m_trackContainer = rEnv.GetValue( "trackContainer", "InDetTrackParticles" );
  //m_jetContainer = rEnv.GetValue( "jetContainer", "AntiKt4EMTopoJets" );
  Info("readConfig()", "m_longlivedParticle is set as %s", m_longlivedParticle.c_str());
  //Info("readConfig()", "m_vertexContainer is set as %s", m_vertexContainer.c_str());
  //Info("readConfig()", "m_trackContainer is set as %s", m_trackContainer.c_str());
  //Info("readConfig()", "m_jetContainer is set as %s", m_jetContainer.c_str());
  // cut config
  m_truthMatchProbCut = rEnv.GetValue( "truthMatchProbCut", 0.8 );
  //m_jetPtCut = rEnv.GetValue( "jetPtCut", 20. );
  //m_jetEtaCut = rEnv.GetValue( "jetEtaCut", 4.5 );
  //m_MeffCut = rEnv.GetValue( "MeffCut", 100. );
  //m_METoverMeffLowerCut = rEnv.GetValue( "METoverMeffLowerCut", 0.3 );
  //m_METoverMeffUpperCut = rEnv.GetValue( "METoverMeffUpperCut", 0.7 );
  //m_METCut = rEnv.GetValue( "METCut", 80. );
  Info("readConfig()", "m_truthMatchProbCut is set as %f", m_truthMatchProbCut);
  //Info("readConfig()", "m_jetPtCut is set as %f", m_jetPtCut);
  //Info("readConfig()", "m_jetEtaCut is set as %f", m_jetEtaCut);
  //Info("readConfig()", "m_MeffCut is set as %f", m_MeffCut);
  //Info("readConfig()", "m_METoverMeffLowerCut is set as %f", m_METoverMeffLowerCut);
  //Info("readConfig()", "m_METoverMeffUpperCut is set as %f", m_METoverMeffUpperCut);
  //Info("readConfig()", "m_METCut is set as %f", m_METCut);
  // which analysis to be done
  //m_doVertexingEfficiency = rEnv.GetValue( "doVertexingEfficiency", false );
  //m_doTrackingEfficiency = rEnv.GetValue( "doTrackingEfficiency", false );
  //m_doDESDFilterStudy = rEnv.GetValue( "doDESDFilterStudy", false );
  //m_doTriggerStudy = rEnv.GetValue( "doTriggerStudy", false );
  //Info("readConfig()", "m_doVertexingEfficiency is set as %d", m_doVertexingEfficiency);
  //Info("readConfig()", "m_doTrackingEfficiency is set as %d", m_doTrackingEfficiency);
  //Info("readConfig()", "m_doDESDFilterStudy is set as %d", m_doDESDFilterStudy);
  //Info("readConfig()", "m_doTriggerStudy is set as %d", m_doTriggerStudy);
  //// trigger collection
  //m_triggerCollectionName = rEnv.GetValue( "triggerCollectionName", "jetPlusMET" );
  //Info("readConfig()", "m_triggerCollectionName is set as %s", m_triggerCollectionName.c_str());
  
  Info("LowMassDVEff::readConfig()","*****     *****     *****     *****" );
  
  //if (m_triggerCollectionName == "jetPlusMET") {
  //  m_triggerCollection.push_back("HLT_j100_xe80");
  //}
  //if (m_triggerCollectionName == "jetPlusMET" || m_triggerCollectionName == "MET") {
  //  m_triggerCollection.push_back("HLT_xe100");
  //  m_triggerCollection.push_back("HLT_xe100_tc_lcw");
  //  m_triggerCollection.push_back("HLT_xe100_tc_lcw_wEFMu");
  //  m_triggerCollection.push_back("HLT_xe100_wEFMu");
  //}
}