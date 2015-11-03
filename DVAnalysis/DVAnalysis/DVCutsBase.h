// -*- C++ -*-
//
// Package:    DV_xAODAnalysis/DVAnalysis
// Class:      DVCutsBase
// 
/**\class DVCutsBase DVCutsBase.h DV_xAODAnalysis/DVAnalysis/Root/DVCutsBase.cxx
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


#ifndef DVCutsBase_H
#define DVCutsBase_H

#include "DVAnalysis/DVAlgBase.h"

#include<string>

// forward declarations
namespace Root
{
    class TGoodRunsList;
}

class TH3F;

class DVCutsBase
{
    public:
        DVCutsBase();
        virtual ~DVCutsBase() { };  
        //! Initialization: material map, Good run list
        virtual void Init(const char * goodrunlist,const char * goodlistfile);
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

#endif
