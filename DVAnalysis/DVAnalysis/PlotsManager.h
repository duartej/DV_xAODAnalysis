// -*- C++ -*-
//
// Package:    DV_xAODAnalysis/DVAnalysis
// Class:      PlotsManager
// 
/**\class PlotsManager PlotsManager.h DV_xAODAnalysis/DVAnalysis/Root/PlotsManager.cxx
 Description: 
 Implementation: 
 */
//
// Original Author: Jordi Duarte Campderros  
//         Created:  Sat Nov 07 22:59:35 CET 2015
// 
// jordi.duarte.campderros@cern.ch
//
//


#ifndef XAOD_STANDALONE
#define XAOD_STANDALONE
#endif


#ifndef DV_PLOTSMANAGER_H
#define DV_PLOTSMANAGER_H

#include<string>
#include<unordered_set> 

#include "TH1F.h"
#include "TH2F.h"


//class TH1F;
//class TH2F;
class TList;
class TFile;

namespace DV
{
    class PlotsManager
    {
        public:
            // Constructor
            PlotsManager();
            virtual ~PlotsManager(){};

            //! wrappers to the ROOT Functions
            TH1F * bookTH1F(const char * name, const char * title,
                    const int & xbin, const float & xmin, const float & xmax );
            TH2F * bookTH2F(const char * name, const char * title,
                    const int & xbin, const float & xmin, const float & xmax,
                    const int & ybin, const float & ymin, const float & ymax );
 
            void setCurrentModule(const std::string & name) { m_currentModule = name; };

            TList * getHists() { return m_histList; };
        private:
            std::string m_currentModule;
            //std::unordered_set<std::string> m_histNames;
            TList * m_histList;
            //TFile * m_outputfile;
            
            // this is needed to distribute the algorithm to the workers
            ClassDef(PlotsManager, 1);
    };
}

#endif
