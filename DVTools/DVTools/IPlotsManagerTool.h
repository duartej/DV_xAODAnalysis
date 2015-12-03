// -*- C++ -*-
//
// Package:    DV_xAODAnalysis/DVTools
// Class:      IPlotsManagerTool
//
/**\class IPlotsManager IPlotsManagerTool.h
 Description:  Interface to the PlotManagerTool
 Implementation:
 */
//
// Original Author: Jordi Duarte Campderros
//         Created:  Sat Nov 07 22:59:35 CET 2015
//
// jordi.duarte.campderros@cern.ch
//
//

#ifndef DV_IPLOTSMANAGERTOOL_H
#define DV_IPLOTSMANAGERTOOL_H

// Framework includes
#include "AsgTools/IAsgTool.h"

#include<string>

namespace DV
{
    class IPlotsManagerTool : public virtual asg::IAsgTool
    {
        /// Declare the interface that the class provides
        ASG_TOOL_INTERFACE( DV::IPlotManagerTool )
        public:

            //! Initialize the output file
            virtual void bookFile(const std::string & name,const std::string & mode) = 0;

            //! keep track of the module which is going to use the plot manager
            //! to book its histos
            virtual void setCurrentModule(const std::string & name) = 0 ;

            //! Store the histograms
            virtual bool saveResults() = 0;
    };
}

#endif  // DV_IPLOTSMANAGERTOOL_H
