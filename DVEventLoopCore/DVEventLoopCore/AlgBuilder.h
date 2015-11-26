// -*- C++ -*-
//
// Package:    DV_xAODAnalysis/DVAnalysis
// Class:      DV::AlgBuilder
// 
/**\class DV::AlgBuilder DV::AlgBuilder.h DV_xAODAnalysis/DVAnalysis/Root/DV::AlgBuilder.cxx
 Description: Builder to create DV::AlgBase 
 Implementation: The Builder is called by the DVEventLoop class (through the 
      DVEventLoop::addAnalysisAlgs(vector<string>) method), which acts as a client. 
      The class calls the instances of DV::AlgBase selected by the user. Returns a 
      generic DV::AlgBase.     
*/
//
// Original Author: Jordi Duarte Campderros  
//         Created:  Sun Nov 01 13:59:11 CET 2015
// 
// jordi.duarte.campderros@cern.ch
//
//

#ifndef XAOD_STANDALONE
#define XAOD_STANDALONE
#endif


#ifndef DV_ALGBUILDER_H
#define DV_ALGBUILDER_H

#include<vector>
#include<string>

namespace DV
{
    // forward declaration
    class AlgBase;
    
    class AlgBuilder
    {
        public:
            static AlgBase * Build(const std::string & algorithm_name);
    };
}

#endif
