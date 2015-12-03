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

// Root
#include "TLorentzVector.h"

// xAOD, ...
#include "xAODTracking/Vertex.h"

//system 
#include<string>
#include<vector>

// forward decl.
class TH3C;

namespace DV
{

  class DVCuts : public asg::AsgTool, virtual public DV::IDVCuts
  {
      // Creates a proper constructor for Athena
      ASG_TOOL_CLASS( DVCuts, DV::IDVCuts )
  
      public:
          // constructor for standalone usage
          DVCuts(const std::string& type);
    
          StatusCode initialize() override;
     
          bool GetMaterialMap(const std::string& path) override;
          
          // single dv cuts
          bool PassFiducialCuts(const xAOD::Vertex& dv) const override;      
          bool PassTrackd0(const xAOD::Vertex& dv, const Amg::Vector3D& pv) const override;
          bool PassDVDist(const xAOD::Vertex& dv, const std::vector<Amg::Vector3D>& pvs) const override;
          bool PassMaterialVeto(const xAOD::Vertex& dv) const override;
          bool PassCentralEtaVeto(const xAOD::Vertex& dv) const override;
          bool PassChargeRequirement(const xAOD::Vertex& dv) const override;
          bool PassMassCut(const xAOD::Vertex& dv) const override;
          
          // all cuts combined
          bool PassVertexCuts(const xAOD::Vertex& dv, const std::vector<Amg::Vector3D>& pvs) const override;
          
      private:
          //! fiducial cut for d0
          float m_d0Max;
          //! fiducial cut for z0
          float m_z0Max;

          //asg::ToolHandle<DDL::IParticleCuts> m_pc;
          // accessor for vertex momentum
          SG::AuxElement::ConstAccessor<TLorentzVector> m_acc_vp4;
          TH3C* m_materialMap;
  };
}

#endif   // DV_DVCUTS_H
