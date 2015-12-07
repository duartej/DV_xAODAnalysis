#ifndef DV_PHOTONMATCH_H
#define DV_PHOTONMATCH_H

// STL
#include <string>

// Tools
#include "AsgTools/AsgTool.h"

// xAOD
#include "xAODEgamma/ElectronContainer.h"
#include "xAODEgamma/PhotonContainer.h"

// DVTools
#include "DVTools/IPhotonMatch.h"

namespace DV
{
    class PhotonMatch : virtual public IPhotonMatch, public asg::AsgTool
    {
        ASG_TOOL_CLASS(PhotonMatch, DV::IPhotonMatch)
        public:
            PhotonMatch(const std::string& name);
            virtual ~PhotonMatch() = default;

            StatusCode initialize() override;

            void MatchPhotons(const xAOD::PhotonContainer& phc, const xAOD::ElectronContainer& elc) const override;

            const xAOD::Photon* GetPhoton(const xAOD::Electron& el) const override;

        private:
            // decorator for photon pointer
            SG::AuxElement::Decorator<const xAOD::Photon*> m_decPhot;
    };
}

#endif
