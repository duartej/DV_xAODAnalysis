// -*- C++ -*-
//
// Package:    DV_xAODAnalysis/DVAnalysis
// Class:      DVDummyCuts
// 
/**\class DVDummyCuts DVDummyCuts.h DV_xAODAnalysis/DVAnalysis/Root/DVDummyCuts.cxx
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


#ifndef DV_DUMMYCUTS_H
#define DV_DUMMYCUTS_H

#include "DVAnalysis/CutsBase.h"

namespace DV
{
    class DummyCuts : public CutsBase
    {
        public:
            virtual void add(const std::string & /*cutname*/) { /*do nothing*/ };
    };
}
#endif
