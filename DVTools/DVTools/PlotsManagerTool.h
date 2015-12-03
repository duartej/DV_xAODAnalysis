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

#ifndef DV_PLOTSMANAGERTOOL_H
#define DV_PLOTSMANAGERTOOL_H

//Framework include
#include "AsgTools/AsgTool.h"

// Local
#include "DVTools/IPlotsManagerTool.h"

// Get the ClassDef macros
#include "Rtypes.h"

// system
#include<string>
#include<unordered_set>

// forward declarations
class TList;
class TFile;

namespace DV
{
    class PlotsManagerTool :
        public virtual IPlotsManagerTool, public asg::AsgTool
    {
        // Creates a proper constructor for Athena
        ASG_TOOL_CLASS( PlotsManagerTool, DV::IPlotsManagerTool )

        public:
            // Constructor for standalone usage
            PlotsManagerTool(const std::string & name);
            //virtual ~PlotsManager(){};

            // Function initializing the tool
            StatusCode initialize();

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
            //! Store the histograms
            bool saveResults();
        private:
            std::string m_outputFilename;
            std::string m_currentModule;
            //std::unordered_set<std::string> m_histNames;
            TList * m_histList;
            TFile * m_outputfile; //!

            // this is needed to distribute the algorithm to the workers
            //ClassDef(PlotsManagerTool, 0);
    };
}

#endif
