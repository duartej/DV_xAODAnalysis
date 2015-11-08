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


#include "DVAnalysis/AlgBuilder.h"
#include "DVAnalysis/AlgBase.h"

// analysis algs that might be run, 
// Insert here the include of any new developed analysis 
#include "DVAnalysis/BasicPlots.h"
#include "DVAnalysis/TrkBasicPlots.h"
//#include "DVAnalysis/LowMassDVEff.h"
#include "DVAnalysis/DVPlusElectronBasic.h"

// system libraries
#include<iostream>


// this is needed to distribute the algorithm to the workers
//ClassImp(DV::AlgBuilder)

// The known list of analysis, include your DV::AlgBase here...
// FIXME:: TO BE DEPRECATED, not needed anymore
const std::vector<std::string> DV::AlgBuilder::_validNames{"TrkBasicPlots","BasicPlots","LowMassDVEff","DVPlusElectronBasic"};

DV::AlgBase * DV::AlgBuilder::Build(const std::string & dv_analysis)
{
    DV::AlgBase * p = 0;
    
    // Algorithm cases
    if( dv_analysis == "TrkBasicPlots" )
    {
        p = new TrkBasicPlots();
    }
    else if( dv_analysis == "BasicPlots" )
    {
        p = new BasicPlots();
    }
    /*else if( dv_analysis == "LowMassDVEff" ) 
    {
        p = new LowMassDVEff();
    }*/
    else if( dv_analysis == "DVPlusElectronBasic" ) 
    {
        p = new DVPlusElectronBasic();
    }
    else
    {
        std::cout << "DV::AlgBuilder ERROR: the '" << dv_analysis << "' algorithm is not defined." 
            << " If it is a valid algorithm (inheriting from DV::AlgBase), then this class (DV::AlgBuilder)" 
            << " should be extended to include it. YOu should:" << std::endl;
         std::cout << "\t* include the header of your new algorithm" << std::endl;
         std::cout << "\t* include the name of the new algorithm into the DV::AlgBuilder::_validNames data-member" << std::endl;
         std::cout << "\t* include a new case inside the DV::AlgBuilder::Build method" << std::endl;
    }

    return p;
}

