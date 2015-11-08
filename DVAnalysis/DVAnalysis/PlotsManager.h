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

// Get the ClassDef macros
#include "Rtypes.h"

//#include "TH1F.h"
//#include "TH2F.h"

//class TH1F;
//class TH2F;
// forward declarations
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

            //! Initialize the output file
            void bookFile(const std::string & name,const std::string & mode);            

            //! wrappers to the ROOT Functions
            template <class THist1Dim,typename TYPE>
                THist1Dim * bookTH1(const char * name, const char * title,
                    const int & xbin, const TYPE & xmin, const TYPE & xmax );
            
            template <class THist2Dim,typename TYPE> 
                THist2Dim * bookTH2(const char * name, const char * title,
                    const int & xbin, const TYPE & xmin, const TYPE & xmax,
                    const int & ybin, const TYPE & ymin, const TYPE & ymax );
            
            //! keep track of the module which is going to use the plot manager
            //! to book its histos
            void setCurrentModule(const std::string & name) { m_currentModule = name; };
            //! Get the booked histograms in a TList
            //TList * getHists() { return m_histList; };
            //! Store the histograms
            bool saveResults();
        private:
            std::string m_currentModule;
            //std::unordered_set<std::string> m_histNames;
            TList * m_histList;
            TFile * m_outputfile; //!
            
            // this is needed to distribute the algorithm to the workers
            ClassDef(PlotsManager, 0);
    };
}

#endif
