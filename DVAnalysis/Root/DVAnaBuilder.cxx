// -*- C++ -*-
//
// Package:    DV_xAODDVAnalysis/DVAnalysis
// Class:      DVAnaBuilder
// 
//
// Original Author: Jordi Duarte Campderros  
//         Created: Sun Nov 1 13:59:11 CET 2015
// 
// jordi.duarte.campderros@cern.ch
//


#include "DVAnalysis/DVAnaBuilder.h"
#include "DVAnalysis/DVBase.h"

// analysis algs that might be run, 
// Insert here the include of any new developed analysis 
#include "DVAnalysis/DVBasicPlots.h"
#include "DVAnalysis/TrkBasicPlots.h"
#include "DVAnalysis/LowMassDVEff.h"

// system libraries
#include<iostream>


// this is needed to distribute the algorithm to the workers
//ClassImp(DVAnaBuilder)

// The known list of analysis, include your DVBase here...
const std::vector<std::string> DVAnaBuilder::_validNames{"TrkBasicPlots","DVBasicPlots","LowMassDVEff"};

DVBase * DVAnaBuilder::Build(const std::string & dv_analysis)
{
    DVBase * p = 0;
    
    // Algorithm cases
    if( dv_analysis == "TrkBasicPlots" )
    {
        p = new TrkBasicPlots();
    }
    else if( dv_analysis == "DVBasicPlots" )
    {
        p = new DVBasicPlots();
    }
    else if( dv_analysis == "LowMassDVEff" ) 
    {
        p = new LowMassDVEff();
    }
    else
    {
        std::cout << "DVAnaBuilder ERROR: the '" << dv_analysis << "' algorithm is not defined." 
            << " If it is a valid algorithm (inheriting from DVBase), then this class (DVAnaBuilder)" 
            << " should be extended to include it. YOu should:" << std::endl;
         std::cout << "\t* include the header of your new algorithm" << std::endl;
         std::cout << "\t* include the name of the new algorithm into the DVAnaBuilder::_validNames data-member" << std::endl;
         std::cout << "\t* include a new case inside the DVAnaBuilder::Build method" << std::endl;
    }

    return p;
}

