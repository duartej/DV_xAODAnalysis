// -*- C++ -*-
//
// Package:    DV_xAODAnalysis/DVCuts
// Class:      IDVCuts
//
/**\class IDVCuts IIDVCuts.h
 Description:  Interface to the DVCuts
 Implementation:
 */
//
// Original Author: Dominik Krauss
//         modified (for DV_xAODAnalysis dual-use) by Jordi Duarte-Campderros
//         Modified: Tue Dec 01 19:06:10 CET 2015
//
// jordi.duarte.campderros@cern.ch
//
//
#ifndef DV_IDVCUTS_H
#define DV_IDVCUTS_H

// Framework includes
#include "AsgTools/IAsgTool.h"

#include "xAODTracking/Vertex.h"

// STL
#include <string>
#include <vector>

namespace DV
{
    class IDVCuts : virtual public asg::IAsgTool
    {
        /// Declare the interface that the class provides
        ASG_TOOL_INTERFACE( DV::IDVCuts )
        public:
            virtual bool PassFiducialCuts(const xAOD::Vertex& dv) const = 0;
            virtual bool PassChisqCut(const xAOD::Vertex& dv) const = 0;
            virtual bool PassDistCut(const xAOD::Vertex& dv, const xAOD::VertexContainer& pvc) const = 0;
            virtual bool PassMaterialVeto(const xAOD::Vertex& dv) const = 0;
            virtual bool PassMassCut(const xAOD::Vertex& dv) const = 0;

            virtual bool PassVertexCuts(const xAOD::Vertex& dv, const xAOD::VertexContainer& pvc) const = 0;
  };
}

#endif  // DV_IDVCUTS_H
