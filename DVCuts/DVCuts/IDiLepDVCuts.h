#ifndef DV_IDILEPDVCUTS_H
#define DV_IDILEPDVCUTS_H

// STL
#include <memory>

// Framework includes
#include "AsgTools/IAsgTool.h"

// xAOD
#include "xAODEgamma/ElectronContainer.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODTracking/Vertex.h"

namespace DV
{
    enum class DiLepTypes;

    class IDiLepDVCuts : virtual public asg::IAsgTool
    {
        /// Declare the interface that the class provides
        ASG_TOOL_INTERFACE(DV::IDiLepDVCuts)
        public:
            virtual void PrepareVertex(xAOD::Vertex& dv,
                                       xAOD::ElectronContainer& elc,
                                       xAOD::MuonContainer& muc) const = 0;

            virtual const std::shared_ptr<xAOD::ElectronContainer> GetEl(const xAOD::Vertex& dv) const = 0;
            virtual const std::shared_ptr<xAOD::MuonContainer> GetMu(const xAOD::Vertex& dv) const = 0;
            virtual DV::DiLepTypes GetType(const xAOD::Vertex& dv) const = 0;

            virtual bool PassCentralEtaVeto(const xAOD::Vertex& dv) const = 0;
            virtual bool PassChargeRequirement(const xAOD::Vertex& dv) const = 0;
            virtual bool PassNLeptons(const xAOD::Vertex& dv) const = 0;

            virtual bool PassTriggerMatching(const xAOD::Vertex& dv) const = 0;
            virtual bool PassDESDMatching(const xAOD::Vertex& dv) const = 0;
    };
}

#endif  // DV_IDILEPDVCUTS_H
