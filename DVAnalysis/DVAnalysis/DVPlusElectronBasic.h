// -*- C++ -*-
//
// Package:    DV_xAODAnalysis/DVAnalysis
// Class:      DVPlusElectronBasic
// 
/**\class DVPlusElectronBasic DVPlusElectronBasic.h DV_xAODAnalysis/DVAnalysis/Root/DVPlusElectronBasic.cxx
 Description: ....
 Implementation: ....
 */
//
// Original Author: Jordi Duarte Campderros  
//         Created:  Sun Nov 03 22:13:32 CET 2015
// 
// jordi.duarte.campderros@cern.ch
//
//
#ifndef DVPlusElectronBasic_h
#define DVPlusElectronBasic_h

#include "DVAnalysis/DVAlgBase.h"

class TH1F;
class TH2F;

class DVPlusElectronBasic : public DVAlgBase
{
    public:
        DVPlusElectronBasic();
        virtual ~DVPlusElectronBasic();
        //! book histograms at DVEventLoop::histInitialize
        virtual void bookHists();
        //! to be called byt the DVEventLoop::histFinalize
        virtual TList* getHists();
        virtual void execute(xAOD::TEvent*);        
        virtual void finalize();

    private:
        TList* m_histList;
        bool m_isBlind;
      
        TH1F* m_nPV;
        TH1F* m_PVx;
        TH1F* m_PVy;
        TH2F* m_PVxy;
        TH1F* m_PVz;
        TH1F* m_PVz_zWeighted;
        TH1F* m_PVrSpread;
      
        TH1F* m_DVmass;
        TH1F* m_DVmassPtCorr;
        TH1F* m_DVmassBig;
        TH1F* m_DVmassBigPtCorr;
        TH1F* m_DVnTrk;
        TH2F* m_DVxy;
        TH2F* m_DVxy_KsGamVeto;
        TH2F* m_DVrphi;
        TH2F* m_DVrphi_KsGamVeto;
        TH2F* m_DVrz;
        TH2F* m_DVnTrkVsMass;
        TH2F* m_DVnTrkVsMassLog;
        TH2F* m_DVnTrkVsMassLogAllCuts;
        TH2F* m_DVnTrkVsMassLogAllCutsExceptEl;
        TH2F* m_DVnTrkVsMassLogAllCutsExceptElMatch;
        TH2F* m_DVnTrkVsMassLogAllCutsUpToNtrk;
        TH1F* m_averageIntPerXing;
        TH1F* m_actualIntPerXing;
        TH1F* m_DVr;
        TH1F* m_DVz;
        TH1F* m_DVr_GE3Trk;
        TH1F* m_DVnTrk_KsGamVeto;
        TH1F* m_pileupWeights;
        TH1F* m_zpvWeights;
        TH1F* m_elecWeights;
        
        //DVPlusElectronCuts* m_cuts;
};
#endif
