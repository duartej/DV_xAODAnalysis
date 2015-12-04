#ifndef DV_ELECCUTS_H
#define DV_ELECCUTS_H

// STL
#include <string>

// Tools
#include "AsgTools/AsgTool.h"
#include "AsgTools/ToolHandle.h"
#include "ElectronPhotonSelectorTools/AsgElectronLikelihoodTool.h"

// xAOD
#include "xAODEgamma/Electron.h"
#include "xAODEgamma/PhotonContainer.h"
#include "xAODTracking/TrackParticle.h"

// DVCuts
#include "DVCuts/IElecCuts.h"

namespace DV
{
    class ElecCuts : virtual public IElecCuts, public asg::AsgTool
    {
        ASG_TOOL_CLASS(ElecCuts, DV::IElecCuts)

        public:
            ElecCuts(const std::string& name);
            virtual ~ElecCuts() = default;

            // retrieves Tools
            StatusCode initialize() override;

            bool PassPtCut(const xAOD::Electron& el) const override;
            bool PassEtaCut(const xAOD::Electron& el) const override;
            bool PassD0Cut(const xAOD::Electron& el) const override;
            bool PassID(const xAOD::Electron& el) const override;

            bool IsPreRetracking(const xAOD::Electron& el) const override;

            const xAOD::Photon* FindMatchingPhoton(const xAOD::Electron& el, const xAOD::PhotonContainer& phc) const override;

        private:
            // electron identification tool
            ToolHandle<IAsgElectronLikelihoodTool> m_elt;

            double m_ptMin;
            double m_etaMax;
            double m_d0Min;
            std::string m_elecID;

            inline const xAOD::TrackParticle* GetTrack(const xAOD::Electron& el) const;
    };
}

#endif
