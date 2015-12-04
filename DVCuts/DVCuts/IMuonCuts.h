#ifndef DV_IMUONCUTS_H
#define DV_IMUONCUTS_H

// Framework includes
#include "AsgTools/IAsgTool.h"

// xAOD
#include "xAODMuon/Muon.h"
#include "xAODTracking/TrackParticle.h"

namespace DV
{
    class IMuonCuts : public virtual asg::IAsgTool
    {
        ASG_TOOL_INTERFACE(DV::IMuonCuts)

        public:
            virtual const xAOD::TrackParticle* GetTrack(const xAOD::Muon& mu) const = 0;

            virtual bool PassPtCut(const xAOD::Muon& mu) const = 0;
            virtual bool PassEtaCut(const xAOD::Muon& mu) const = 0;
            virtual bool PassD0Cut(const xAOD::Muon& mu) const = 0;
            virtual bool PassID(const xAOD::Muon& mu) const = 0;

            virtual bool IsPreRetracking(const xAOD::Muon& mu) const = 0;
    };
}

#endif
