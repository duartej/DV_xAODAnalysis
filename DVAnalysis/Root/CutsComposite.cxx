// -*- C++ -*-
//
// Package:    DV_xAODDVAnalysis/DVAnalysis
// Class:      DV::CutsComposite
// 
//
// Original Author: Jordi Duarte Campderros  
//         Created: Sun Nov 3 13:59:11 CET 2015
// 
// jordi.duarte.campderros@cern.ch
//


#include "DVAnalysis/CutsComposite.h"

// Insert here the include of any cut algorithm you want to use
#include "DVAnalysis/DummyCuts.h"

// system libraries
#include<iostream>
#include<stdexcept>


// The known list of analysis, include your DV::AlgBase here... not needed?
const std::vector<std::string> DV::CutsComposite::_validNames{"DummyCuts"};

DV::CutsComposite::~CutsComposite()
{
    //de-alloc the created Cuts-algs in the add method
    for(auto & name_alg: m_cutAlgs)
    {
        if( name_alg.second != 0 )
        {
            delete name_alg.second;
            name_alg.second = 0;
        }
    }
}

void DV::CutsComposite::Init(const char * goodrunlist,const char * goodlistfile)
{
    for(auto & name_alg: m_cutAlgs)
    {
        // Probably not needed.... FIXME
        if( name_alg.second == 0)
        {
            continue;
        }
        name_alg.second->Init(goodrunlist,goodlistfile);
    }
}

void DV::CutsComposite::add(const std::string & cutname)
{
    DV::CutsBase * p = 0;
    
    // Algorithm cases
    if( cutname == "DummyCuts" )
    {
        p = new DummyCuts();
    }
    else
    {
        std::string message("DV::CutsComposite ERROR: the '"+cutname+"' cut algorithm is not defined."\
                " If it is a valid cut (inheriting from DV::CutsBase), then this class (DV::CutsComposite)"\
                " should be extended to include it. You should:\n"\
                "\t* include the header of your new algorithm\n"\
                "\t* include the name of the new algorithm into the DV::CutsComposite::_validNames data-member\n"\
                "\t* include a new case inside the DV::CutsComposite::add method");
         throw std::runtime_error(message);
    }

    m_cutAlgs[cutname] = p;
}

