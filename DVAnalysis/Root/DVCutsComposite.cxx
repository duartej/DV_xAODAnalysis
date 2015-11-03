// -*- C++ -*-
//
// Package:    DV_xAODDVAnalysis/DVAnalysis
// Class:      DVCutsComposite
// 
//
// Original Author: Jordi Duarte Campderros  
//         Created: Sun Nov 3 13:59:11 CET 2015
// 
// jordi.duarte.campderros@cern.ch
//


#include "DVAnalysis/DVCutsComposite.h"

// Insert here the include of any cut algorithm you want to use
#include "DVAnalysis/DVDummyCuts.h"

// system libraries
#include<iostream>
#include<stdexcept>


// The known list of analysis, include your DVAlgBase here... not needed?
const std::vector<std::string> DVCutsComposite::_validNames{"DVDummyCuts"};

DVCutsComposite::~DVCutsComposite()
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

void DVCutsComposite::Init(const char * goodrunlist,const char * goodlistfile)
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

void DVCutsComposite::add(const std::string & cutname)
{
    DVCutsBase * p = 0;
    
    // Algorithm cases
    if( cutname == "DVDummyCuts" )
    {
        p = new DVDummyCuts();
    }
    else
    {
        std::string message("DVCutsComposite ERROR: the '"+cutname+"' cut algorithm is not defined."\
                " If it is a valid cut (inheriting from DVCutsBase), then this class (DVCutsComposite)"\
                " should be extended to include it. You should:\n"\
                "\t* include the header of your new algorithm\n"\
                "\t* include the name of the new algorithm into the DVCutsComposite::_validNames data-member"\
                "\t* include a new case inside the DVCutsComposite::add method");
         throw std::runtime_error(message);
    }

    m_cutAlgs[cutname] = p;
}

