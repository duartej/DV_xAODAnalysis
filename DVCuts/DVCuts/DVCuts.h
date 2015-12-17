// -*- C++ -*-
//
// Package:    DV_xAODAnalysis/DVCuts
// Class:      DVCuts
//
/**\class dvCuts DVCuts.h DV_xAODAnalysis/DVCuts/Root/DVCuts.cxx
 Description:
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

#ifndef DV_DVCUTS_H
#define DV_DVCUTS_H

// Framework includes
#include "AsgTools/AsgTool.h"

// Local
#include "DVCuts/IDVCuts.h"

// xAOD, ...
#include "xAODTracking/VertexContainer.h"

//system
#include<string>

// forward decl.
class TFile;
class TH3C;

namespace DV
{
  class DVCuts : public asg::AsgTool, virtual public DV::IDVCuts
  {
      // Creates a proper constructor for Athena
      ASG_TOOL_CLASS( DVCuts, DV::IDVCuts )

      public:
          // constructor for standalone usage
          DVCuts(const std::string& name);
          virtual ~DVCuts();

          StatusCode initialize() override;

          bool PassFiducialCuts(const xAOD::Vertex& dv) const override;
          bool PassChisqCut(const xAOD::Vertex& dv) const override;
          bool PassDistCut(const xAOD::Vertex& dv, const xAOD::VertexContainer& pvc) const override;
          bool PassMaterialVeto(const xAOD::Vertex& dv) const override;
          bool PassMassCut(const xAOD::Vertex& dv) const override;

          bool PassVertexCuts(const xAOD::Vertex& dv, const xAOD::VertexContainer& pvc) const override;

      private:
          //! fiducial cut for r_DV
          float m_rDVMax;
          //! fiducial cut for z_DV
          float m_zDVMax;
          //! cut on fit quality
          float m_chisqDof;
          //! cut on distance in transverse plane
          float m_distMin;
          //! minimal DV mass
          float m_DVMassMin;

          // accessor for vertex mass
          SG::AuxElement::ConstAccessor<double> m_accMass;

          std::string m_mapFile;
          TH3C* m_materialMap;
  };
}

#endif   // DV_DVCUTS_H
