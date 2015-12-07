#ifndef DV_OVERLAPREMOVAL_H
#define DV_OVERLAPREMOVAL_H

// STL
#include <string>

// Tools
#include "AsgTools/AsgTool.h"

// xAOD
#include "xAODEgamma/ElectronContainer.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODTracking/TrackParticleContainer.h"

// DVCuts
#include "DVTools/IOverlapRemoval.h"

namespace DV
{
    class OverlapRemoval : virtual public IOverlapRemoval, public asg::AsgTool
    {
        ASG_TOOL_CLASS(OverlapRemoval, DV::IOverlapRemoval)
        public:
            OverlapRemoval(const std::string& name);
            virtual ~OverlapRemoval() = default;

            StatusCode initialize() override;

            // the particles are decorated and not removed
            void RemoveOverlap(const xAOD::ElectronContainer& elc,
                               const xAOD::MuonContainer& muc) const override;
            void RemoveLeptonTracks(const xAOD::ElectronContainer& elc,
                                    const xAOD::MuonContainer& muc,
                                    const xAOD::TrackParticleContainer& trc) const override;

            bool IsOverlap(const xAOD::IParticle& p) const override;
            bool IsLeptonTrack(const xAOD::TrackParticle& tr) const override;

        private:
            // decorator for isOverlap flag
            SG::AuxElement::Decorator<char> m_decOver;
            // decorator for isLeptonTrack flag
            SG::AuxElement::Decorator<char> m_decLepTrk;
    };
}

#endif
