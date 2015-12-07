#ifndef DV_IPHOTONMATCH_H
#define DV_IPHOTONMATCH_H

// Framework includes
#include "AsgTools/IAsgTool.h"

// xAOD
#include "xAODEgamma/ElectronContainer.h"
#include "xAODEgamma/PhotonContainer.h"

namespace DV
{
    class IPhotonMatch : public virtual asg::IAsgTool
    {
        ASG_TOOL_INTERFACE(DV::IPhotonMatch)
        public:
            virtual void MatchPhotons(const xAOD::PhotonContainer& phc, const xAOD::ElectronContainer& elc) const = 0;

            virtual const xAOD::Photon* GetPhoton(const xAOD::Electron& el) const = 0;
     };
}

#endif
