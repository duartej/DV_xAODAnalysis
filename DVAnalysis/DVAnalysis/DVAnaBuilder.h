// -*- C++ -*-
//
// Package:    DV_xAODAnalysis/DVAnalysis
// Class:      DVAnaBuilder
// 
/**\class DVAnaBuilder DVAnaBuilder.h DV_xAODAnalysis/DVAnalysis/Root/DVAnaBuilder.cxx
 Description: Builder to create DVBase 
              (MTAtom concrete classes).
 Implementation: The Builder is called by the DVEventLoop class (addAnalysisAlgs method)
              acts as a client. The class calls the instances of DVBase
              selected by the user. Return a generic DVBase.     
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


#ifndef DVAna_DVAnaBuilder_H
#define DVAna_DVAnaBuilder_H

#include<vector>
#include<string>

class DVBase;

class DVAnaBuilder
{
public:
    static DVBase * Build(const std::string & dvnames);

private:
    const static std::vector<std::string> _validNames;
  
    // this is needed to distribute the algorithm to the workers
    //ClassDef(DVAnaBuilder, 1);
};

#endif
