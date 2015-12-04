#ifndef DV_DILEPCOSMICS_H
#define DV_DILEPCOSMICS_H

// STL
#include <string>

// ROOT
#include "TLorentzVector.h"

// Tools
#include "AsgTools/AsgTool.h"
#include "AsgTools/ToolHandle.h"

// xAOD
#include "xAODEgamma/ElectronContainer.h"
#include "xAODMuon/MuonContainer.h"

// DVCuts
#include "DVCuts/IElecCuts.h"
#include "DVCuts/IDiLepCosmics.h"
#include "DVCuts/IMuonCuts.h"

namespace DV
{
    class DiLepCosmics : virtual public IDiLepCosmics, public asg::AsgTool
    {
        ASG_TOOL_CLASS(DiLepCosmics, DV::IDiLepCosmics)

        public:
            DiLepCosmics(const std::string& name);
            virtual ~DiLepCosmics() = default;

            // retrieves Tools
            StatusCode initialize() override;

            bool PassCosmicVeto(const TLorentzVector& lv1, const TLorentzVector& lv2) const override;
            bool PassCosmicEventVeto(const xAOD::ElectronContainer& elc, const xAOD::MuonContainer& muc) const override;

            bool PassMuPairCosmicVeto(const xAOD::Muon& mu1, const xAOD::Muon& mu2) const override;
            bool PassElMuPairCosmicVeto(const xAOD::Electron& el, const xAOD::Muon& mu) const override;
            bool PassElPairCosmicVeto(const xAOD::Electron& el1, const xAOD::Electron& el2) const override;

        private:
            ToolHandle<DV::IElecCuts> m_ec;
            ToolHandle<DV::IMuonCuts> m_mc;

            double m_Rcos;
            double m_deltaR;
    };
}

#endif
