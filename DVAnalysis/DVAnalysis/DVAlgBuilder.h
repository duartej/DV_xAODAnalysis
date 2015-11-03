// -*- C++ -*-
//
// Package:    DV_xAODAnalysis/DVAnalysis
// Class:      DVAlgBuilder
// 
/**\class DVAlgBuilder DVAlgBuilder.h DV_xAODAnalysis/DVAnalysis/Root/DVAlgBuilder.cxx
 Description: Builder to create DVAlgBase 
 Implementation: The Builder is called by the DVEventLoop class (through the 
      DVEventLoop::addAnalysisAlgs(vector<string>) method), which acts as a client. 
      The class calls the instances of DVAlgBase selected by the user. Returns a 
      generic DVAlgBase.     
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


#ifndef DVAna_DVAlgBuilder_H
#define DVAna_DVAlgBuilder_H

#include<vector>
#include<string>

class DVAlgBase;

class DVAlgBuilder
{
    public:
        static DVAlgBase * Build(const std::string & dvnames);
        
    private:
        const static std::vector<std::string> _validNames;  
};

#endif