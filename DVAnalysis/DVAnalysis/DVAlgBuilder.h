// -*- C++ -*-
//
// Package:    DV_xAODAnalysis/DVAnalysis
// Class:      DVAnaBuilder
// 
/**\class DVAnaBuilder DVAnaBuilder.h DV_xAODAnalysis/DVAnalysis/Root/DVAnaBuilder.cxx
 Description: Builder to create DVAlgBase 
              (MTAtom concrete classes).
 Implementation: The Builder is called by the DVEventLoop class (addAnalysisAlgs method)
              acts as a client. The class calls the instances of DVAlgBase
              selected by the user. Return a generic DVAlgBase.     
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

class DVAlgBase;

class DVAnaBuilder
{
    public:
        static DVAlgBase * Build(const std::string & dvnames);
        
    private:
        const static std::vector<std::string> _validNames;  
};

#endif
