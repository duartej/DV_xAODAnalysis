#ifndef DV_IDILEPDVCUTS_H
#define DV_IDILEPDVCUTS_H

// Framework includes
#include "AsgTools/IAsgTool.h"

#include "xAODTracking/Vertex.h"

namespace DV
{
    class IDiLepDVCuts : virtual public asg::IAsgTool
    {
        /// Declare the interface that the class provides
        ASG_TOOL_INTERFACE(DV::IDiLepDVCuts)
        public:
            virtual bool PassCentralEtaVeto(const xAOD::Vertex& dv) const = 0;
            virtual bool PassChargeRequirement(const xAOD::Vertex& dv) const = 0;
  };
}

#endif  // DV_IDILEPDVCUTS_H
