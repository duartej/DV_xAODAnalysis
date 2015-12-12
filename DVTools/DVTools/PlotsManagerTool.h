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
//#include "Rtypes.h"

// system
#include<string>
#include<vector>

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
            virtual ~PlotsManagerTool() = default;

            // Function initializing the tool
            StatusCode initialize() override;

            //! Initialize the output file
            void bookFile(const std::string & name, const std::string & mode) override;

            //! wrappers to the ROOT Functions
            template <class THist1Dim>
                THist1Dim * bookTH1(const std::string & name, const std::string & title,
                    const int & xbins, const double & xmin, const double & xmax );
            template <class THist1Dim>
                THist1Dim * bookTH1(const std::string & name, const std::string & title,
                    const int & xbins, const std::vector<double> & xbin_edges );

            template <class THist2Dim>
                THist2Dim * bookTH2(const std::string & name, const std::string & title,
                    const int & xbins, const double & xmin, const double & xmax,
                    const int & ybins, const double & ymin, const double & ymax );
            template <class THist2Dim>
                THist2Dim * bookTH2(const std::string & name, const std::string & title,
                    const int & xbins, const std::vector<double> & xbin_edges,
                    const int & ybins, const std::vector<double> & ybin_edges );

            //! keep track of the module which is going to use the plot manager
            //! to book its histos
            void setCurrentModule(const std::string & name) override { m_currentModule = name; };

            //! Store the histograms
            bool saveResults() override;
        private:
            std::string m_currentModule;
            TList * m_histList;
            TFile * m_outputfile; //!

            inline void checkHistName(const std::string & hist_name) const;

            // this is needed to distribute the algorithm to the workers
            //ClassDef(PlotsManagerTool, 0);
    };
}

#endif
