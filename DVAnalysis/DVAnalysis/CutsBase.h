// -*- C++ -*-
//
// Package:    DV_xAODAnalysis/DVAnalysis
// Class:      DV::CutsBase
// 
/**\class DV::CutsBase DV::CutsBase.h DV_xAODAnalysis/DVAnalysis/Root/DV::CutsBase.cxx
 Description: Abstract class to define the cuts to be used in an analysis
 Implementation: Every analysis is the result of the application of some cuts.
        This cuts can be applied to the whole event or to some collections of
        the event, modifiying the event. 
        This base classes implements generic cuts (event-wise mostly) and defines
        some pure virtual analysis-dependent
 */
//
// Original Author: Jordi Duarte Campderros  
//         Created:  Tue Nov 03 11:23:32 CET 2015
// 
// jordi.duarte.campderros@cern.ch
//
//


#ifndef XAOD_STANDALONE
#define XAOD_STANDALONE
#endif


#ifndef DV_CUTSBASE_H
#define DV_CUTSBASE_H

#include "DVAnalysis/AlgBase.h"

#include<string>

// forward declarations
namespace Root
{
    class TGoodRunsList;
}

namespace xAOD
{
    class TEvent;
}

class TH3F;

namespace DV
{
    class CutsBase
    {
        public:
            CutsBase();
            virtual ~CutsBase();  
            //! Initialization: material map, Good run list
            virtual void Init(const char * goodrunlist,const char * goodlistfile);
            //! Generic cuts        
            virtual bool PassesMaterialVeto(const xAOD::TEvent * const evt, const int & i);
            
            //! Allowing composition of cuts
            virtual void add(const std::string & cutname) = 0;
    
        private:
            //! obtain the material map
            bool getMaterialMap(const char * filename);
            //! the good list reader
            Root::TGoodRunsList * m_GRL;
            //! the material map
            TH3F * m_materialMap;
            //! the cached cuts        
    };
}

#endif
