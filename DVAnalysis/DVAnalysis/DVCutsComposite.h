// -*- C++ -*-
//
// Package:    DV_xAODAnalysis/DVAnalysis
// Class:      DVCutsComposite
// 
/**\class DVCutsComposite DVCutsComposite.h DV_xAODAnalysis/DVAnalysis/Root/DVCutsComposite.cxx
 Description: a container of cut algorithms (composite) 
 Implementation: The class inherits from DVAlgBase in order to be treat by
        the DVEventLoop (the main actor) identically as a regular algorithm,
        therefore taking care of initialization and destruction, and providing 
        event-wise data from the input file. 
        This class is a container of cut algorithms which are going to be used
        by the DVAlgBase algorithms scheduled. All the algorithms (clients) 
        shared the same instance of a particular cut.
        The main method, DVCutsComposite::add(string), initializes the 
        concrete DVCutsBase given the name of the class.
*/
//
// Original Author: Jordi Duarte Campderros  
//         Created:  Sun Nov 03 15:32:34 CET 2015
// 
// jordi.duarte.campderros@cern.ch
//
//
#ifndef DVCutsComposite_h
#define DVCutsComposite_h

#include "DVAnalysis/DVAlgBase.h"
#include "DVAnalysis/DVCutsBase.h"

#include<string>
#include<map>

class TList;

// Enumerate to take account of the valid cut algorithms
// Maybe? By the moment string...

class DVCutsComposite : public DVAlgBase, public DVCutsBase
{
    public:
        ~DVCutsComposite();
        //! nothing to do ?
        virtual void bookHists() { };
        //! nothing to do ?
        virtual TList* getHists() { return 0; };
        virtual void execute(xAOD::TEvent* evt) { setEvent(evt); };
        //! nothing to do ?
        virtual void finalize() { };

        //! specific to the composite
        void add(const std::string & cutname);

    private:
        //! list of valid cut algorithms names
        const static std::vector<std::string> _validNames;
        //! list of cuts
        std::map<std::string,DVCutsBase *> m_cutAlgs;
};
#endif
