#ifndef DV_IELECCUTS_H
#define DV_IELECCUTS_H

// Framework includes
#include "AsgTools/IAsgTool.h"

// xAOD
#include "xAODEgamma/Electron.h"
#include "xAODTracking/TrackParticle.h"

namespace DV
{
    class IElecCuts : public virtual asg::IAsgTool
    {
        ASG_TOOL_INTERFACE(DV::IElecCuts)

        public:
            virtual const xAOD::TrackParticle* GetTrack(const xAOD::Electron& el) const = 0;

            virtual bool PassPtCut(const xAOD::Electron& el) const = 0;
            virtual bool PassEtaCut(const xAOD::Electron& el) const = 0;
            virtual bool PassKinematics(const xAOD::Electron& el) const = 0;

            virtual bool PassD0Cut(const xAOD::Electron& el) const = 0;

            virtual bool PassID(const xAOD::Electron& el) const = 0;

            virtual bool IsPreRetracking(const xAOD::Electron& el) const = 0;
    };
}

#endif
