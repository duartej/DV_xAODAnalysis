#ifndef DV_IOVERLAPREMOVAL_H
#define DV_IOVERLAPREMOVAL_H

// Framework includes
#include "AsgTools/IAsgTool.h"

// xAOD
#include "xAODEgamma/ElectronContainer.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODTracking/TrackParticleContainer.h"

namespace DV
{
    class IOverlapRemoval : public virtual asg::IAsgTool
    {
        ASG_TOOL_INTERFACE(DV::IOverlapRemoval)
        public:
            virtual void RemoveOverlap(xAOD::ElectronContainer& elc,
                                       xAOD::MuonContainer& muc) const = 0;
            virtual void RemoveLeptonTracks(const xAOD::ElectronContainer& elc,
                                            const xAOD::MuonContainer& muc,
                                            xAOD::TrackParticleContainer& trc) const = 0;

            virtual bool IsOverlap(const xAOD::IParticle& p) const = 0;
            virtual bool IsLeptonTrack(const xAOD::TrackParticle& tr) const = 0;
     };
}

#endif
