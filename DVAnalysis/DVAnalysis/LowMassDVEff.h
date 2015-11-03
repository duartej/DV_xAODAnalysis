#ifndef LowMassDVEff_h
#define LowMassDVEff_h

#include "DVAnalysis/DVAlgBase.h"
#include <TProfile.h>
#include <TProfile2D.h>
#include <TGraphAsymmErrors.h>
#include <TMath.h>

#include <string>

class LowMassDVEff : public DVAlgBase {
 public:
  LowMassDVEff();
  virtual ~LowMassDVEff(){};

  virtual void bookHists();
  virtual TList* getHists();
  virtual void execute(xAOD::TEvent*);
  virtual void finalize();
 private:
  TList* m_histList;

  //TH2F* m_DVxy; //!
  //TH1F* m_DVmass; //!
  TH1D *h_truthVertPosR; //!
  TH1D *h_recoVertPosR; //!
  TH1D *h_diffVertPosR; //!
  TH1D *h_diffVertPosZ; //!
  TH1D *h_diffVertPos3D; //!
  TH1D *h_recoMatchedVertMass; //!

  TH1D *h_nChargedParticlesFromLLP; //!
  TH1D *h_nChargedParticlesFromLLP1GeVCut; //!
  TH2D *h_nChargedParticlesFromLLP1GeVCutVSradius; //!
  TH1D *h_PtFromLLP; //!
  TH1D *h_nSumChargedParticlesVSradius; //!

  TH1D *h_nTrkAssocVertMatched; //!
  TH1D *h_PtAssocVertMatched; //!
  TH1D *h_nSumTrkVSradius; //!

  TH2D *h_DVmassVSnTrkMatched; //!
  TH2D *h_nTrkMatchedVSradius; //!
  //TGraphAsymmErrors *g_vertEffR; //!

  ClassDef(LowMassDVEff, 1);

  // config
  std::string m_configFile;
  std::string m_longlivedParticle;
  double m_truthMatchProbCut;
  void readConfig();
  
  bool isRhadron(int pdgid){
    //pdgid ==  1000021 // gluino
    if(pdgid ==  1000993 || // glueball
       pdgid ==  1009213 ||
       pdgid ==  1009313 ||
       pdgid ==  1009323 ||
       pdgid ==  1009113 ||
       pdgid ==  1009223 ||
       pdgid ==  1009333 ||
       pdgid ==  1091114 ||
       pdgid ==  1092114 ||
       pdgid ==  1092214 ||
       pdgid ==  1092224 ||
       pdgid ==  1093114 ||
       pdgid ==  1093214 ||
       pdgid ==  1093224 ||
       pdgid ==  1093314 ||
       pdgid ==  1093324 ||
       pdgid ==  1093334 ||
       pdgid == -1009213 ||
       pdgid == -1009313 ||
       pdgid == -1009323 ||
       pdgid == -1091114 ||
       pdgid == -1092114 ||
       pdgid == -1092214 ||
       pdgid == -1092224 ||
       pdgid == -1093114 ||
       pdgid == -1093214 ||
       pdgid == -1093224 ||
       pdgid == -1093314 ||
       pdgid == -1093324 ||
       pdgid == -1093334) {
      //pdgid ==  1000039    // Gravitino
      //pdgid ==  1000022   // ~chi_10
      return true;
    } else {
      return false;
    }
  };
  bool isLN(int pdgid) { // Lightest Neutralino
    if (pdgid == 1000022) {
      return true;
    } else {
      return false;
    }
  };
  bool isNLN(int pdgid) { // Next to Lightest Neutralino
    if (pdgid == 1000023) {
      return true;
    } else {
      return false;
    }
  };
  bool isLLP(int pdgid) { // Long Lived Particle
    if (m_longlivedParticle == "gluino") return isRhadron(pdgid);
    else if (m_longlivedParticle == "neutralino_1") return isLN(pdgid);
    else if (m_longlivedParticle == "neutralino_2") return isNLN(pdgid);
    else {
      std::cout << "Warning! m_longlivedParticle is not set properly" << std::endl;
      return false;
    }
  };
  double getDistance3D(double x1, double y1, double z1, double x2, double y2, double z2){
    return TMath::Sqrt(TMath::Power(x1 - x2, 2)
                       + TMath::Power(y1 - y2, 2)
                       + TMath::Power(z1 - z2, 2));
  };
  

};


#endif
