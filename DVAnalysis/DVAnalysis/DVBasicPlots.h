#ifndef DVBasicPlots_h
#define DVBasicPlots_h

#include "DVAnalysis/DVAnaBase.h"
#include <TProfile.h>
#include <TProfile2D.h>


class DVBasicPlots : public DVAnaBase {
 public:
  DVBasicPlots();
  virtual ~DVBasicPlots(){};

  virtual void bookHists();
  virtual TList* getHists();
  virtual void execute(xAOD::TEvent*);
  virtual void finalize();
 private:
  TList* m_histList;

  TH2F* m_DVxy; //!
  TH1F* m_DVmass; //!
  TH1D* h_nTrkAssocVertAll; //!
  TH2D* h_DVmassVSnTrkAll; //!

  ClassDef(DVBasicPlots, 1);

};


#endif
