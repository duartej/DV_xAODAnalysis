#ifndef DV_DILEPDVCUTS_H
#define DV_DILEPDVCUTS_H

// Framework includes
#include "AsgTools/AsgTool.h"

// Local
#include "DVCuts/IDiLepDVCuts.h"

// xAOD, ...
#include "xAODTracking/VertexContainer.h"

//system
#include<string>

namespace DV
{
  class DiLepDVCuts : public asg::AsgTool, virtual public DV::IDiLepDVCuts
  {
      // Creates a proper constructor for Athena
      ASG_TOOL_CLASS(DiLepDVCuts, DV::IDiLepDVCuts)

      public:
          // constructor for standalone usage
          DiLepDVCuts(const std::string& name);

          StatusCode initialize() override;

          bool PassCentralEtaVeto(const xAOD::Vertex& dv) const override;
          bool PassChargeRequirement(const xAOD::Vertex& dv) const override;

      private:
          //! cut for central eta veto
          float m_centEta;
  };
}

#endif   // DV_DiLepDVCUTS_H
