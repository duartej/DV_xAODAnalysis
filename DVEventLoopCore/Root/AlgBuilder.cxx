// -*- C++ -*-
//
// Package:    DV_xAODDVAnalysis/DVAnalysis
// Class:      DV::AlgBuilder
//
//
// Original Author: Jordi Duarte Campderros
//         Created: Sun Nov 1 13:59:11 CET 2015
//
// jordi.duarte.campderros@cern.ch
//


#include "DVEventLoopCore/AlgBuilder.h"
#include "DVAnalyses/AlgBase.h"

// analysis algs that might be run,
// Insert here the include of any new developed analysis
#include "DVAnalyses/BasicPlots.h"
#include "DVAnalyses/DiLepCutFlow.h"
#include "DVAnalyses/DiLepSignal.h"
//#include "DVAnalyses/TrkBasicPlots.h"
//#include "DVAnalyses/LowMassDVEff.h"
//#include "DVAnalyses/DVPlusElectronBasic.h"

// system libraries
#include<iostream>


// this is needed to distribute the algorithm to the workers
//ClassImp(DV::AlgBuilder)

DV::AlgBase * DV::AlgBuilder::Build(const std::string & dv_analysis)
{
    DV::AlgBase * p = 0;

    // Algorithm cases
    if( dv_analysis == "BasicPlots" )
    {
        p = new BasicPlots();
    }
    else if( dv_analysis == "DiLepCutFlow" )
    {
        p = new DiLepCutFlow();
    }
    else if( dv_analysis == "DiLepSignal" )
    {
        p = new DiLepSignal();
    }
    /*else if( dv_analysis == "TrkBasicPlots" )
    {
        p = new TrkBasicPlots();
    }*/
    /*else if( dv_analysis == "LowMassDVEff" )
    {
        p = new LowMassDVEff();
    }*/
    /*else if( dv_analysis == "DVPlusElectronBasic" )
    {
        p = new DVPlusElectronBasic();
    }*/
    else
    {
        std::cout << "DV::AlgBuilder ERROR: the '" << dv_analysis << "' algorithm is not defined."
                  << " If it is a valid algorithm (inheriting from DV::AlgBase), then this class (DV::AlgBuilder)"
                  << " should be extended to include it. YOu should:" << std::endl;
        std::cout << "\t* include the header of your new algorithm" << std::endl;
        std::cout << "\t* include a new case inside the DV::AlgBuilder::Build method" << std::endl;
    }

    return p;
}

