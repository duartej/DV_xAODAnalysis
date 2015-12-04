#ifndef DV_IDILEPDVCUTS_H
#define DV_IDILEPDVCUTS_H

// Framework includes
#include "AsgTools/IAsgTool.h"

// xAOD
#include "xAODEgamma/ElectronContainer.h"
#include "xAODEgamma/PhotonContainer.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODTracking/Vertex.h"

namespace DV
{
    class IDiLepDVCuts : virtual public asg::IAsgTool
    {
        /// Declare the interface that the class provides
        ASG_TOOL_INTERFACE(DV::IDiLepDVCuts)
        public:
            virtual void ApplyLeptonMatching(xAOD::Vertex& dv,
                                             const xAOD::ElectronContainer& elc,
                                             const xAOD::PhotonContainer& phc,
                                             const xAOD::MuonContainer& muc) const = 0;
            virtual void ApplyOverlapRemoval(const xAOD::Vertex& dv) const = 0;
            virtual void ApplyTriggerMatching(xAOD::Vertex& dv) const = 0;

            virtual bool PassCentralEtaVeto(const xAOD::Vertex& dv) const = 0;
            virtual bool PassChargeRequirement(const xAOD::Vertex& dv) const = 0;

            virtual bool PassTriggerMatching(const xAOD::Vertex& dv) const = 0;
            virtual bool PassDESDMatching(const xAOD::Vertex& dv) const = 0;
    };
}

#endif  // DV_IDILEPDVCUTS_H
