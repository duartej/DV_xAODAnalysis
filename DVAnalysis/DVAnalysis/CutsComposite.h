// -*- C++ -*-
//
// Package:    DV_xAODAnalysis/DVAnalysis
// Class:      DVCutsComposite
// 
/**\class DVCutsComposite DVCutsComposite.h DV_xAODAnalysis/DVAnalysis/Root/DVCutsComposite.cxx
 Description: a container of cut algorithms (composite) 
 Implementation: The class inherits from DV::AlgBase in order to be treat by
        the DVEventLoop (the main actor) identically as a regular algorithm,
        therefore taking care of initialization and destruction, and providing 
        event-wise data from the input file. 
        This class is a container of cut algorithms which are going to be used
        by the DV::AlgBase algorithms scheduled. All the algorithms (clients) 
        shared the same instance of a particular cut.
        The main method, DVCutsComposite::add(string), initializes the 
        concrete DV::CutsBase given the name of the class.
*/
//
// Original Author: Jordi Duarte Campderros  
//         Created:  Sun Nov 03 15:32:34 CET 2015
// 
// jordi.duarte.campderros@cern.ch
//
//
#ifndef DV_CUTSCOMPOSITE_H
#define DV_CUTSCOMPOSITE_H

#include "DVAnalysis/AlgBase.h"
#include "DVAnalysis/CutsBase.h"

#include<string>
#include<map>

class TList;

// Enumerate to take account of the valid cut algorithms
// Maybe? By the moment string...

namespace DV
{
    class CutsComposite : public AlgBase, public CutsBase
    {
        public:
            ~CutsComposite();
            //! nothing to do ?
            virtual void bookHists() { };
            //! nothing to do ?
            virtual TList* getHists() { return 0; };
            //! Override init to initialize all the cut
            virtual void Init(const char * goodrunlist, const char* goodlistfile);
            //! Set the current event in order to access data
            virtual void execute(xAOD::TEvent* evt) { setEvent(evt); };
            //! nothing to do ?
            virtual void finalize() { };
    
            //! specific to the composite.
            //! add the cut to the container and return it
            const CutsBase * add(const std::string & cutname);

        private:
            //! list of cuts
            std::map<std::string,CutsBase *> m_cutAlgs;
    };
}
#endif
