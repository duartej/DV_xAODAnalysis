#ifndef DV_DILEPDESD_H
#define DV_DILEPDESD_H

// STL
#include <string>

// Tools
#include "AsgTools/AsgTool.h"
#include "AsgTools/ToolHandle.h"

// xAOD
#include "xAODEgamma/ElectronContainer.h"
#include "xAODEgamma/Photon.h"
#include "xAODMuon/MuonContainer.h"

// DV
#include "DVCuts/IDiLepDESD.h"
#include "DVCuts/IElecCuts.h"
#include "DVCuts/IMuonCuts.h"
#include "DVTools/IPhotonMatch.h"

namespace DV
{
    class DiLepDESD : virtual public IDiLepDESD, public asg::AsgTool
    {
        ASG_TOOL_CLASS(DiLepDESD, DV::IDiLepDESD)

        public:
            DiLepDESD(const std::string& name);
            virtual ~DiLepDESD() = default;

            // retrieves Tools
            StatusCode initialize() override;

            // CAUTION!: has to be called for every DV before the filters can be used!
            void SetTriggerFlags(bool siph, bool diph, bool simu) override;

            // single filter implementations
            bool PassSiEl(const xAOD::Electron& el) const override;
            bool PassSiPhX(const xAOD::Photon& ph, const xAOD::Electron& el) const override;
            bool PassSiPhX(const xAOD::Photon& ph1, const xAOD::Photon& ph2) const override;
            bool PassSiPhX(const xAOD::Photon& ph, const xAOD::Muon& mu) const override;
            bool PassSiMu(const xAOD::Muon& mu) const override;

            // di filter implementations
            bool PassDiEl(const xAOD::Electron& el1, const xAOD::Electron& el2) const override;
            bool PassDiPh(const xAOD::Photon& ph1, const xAOD::Photon& ph2) const override;
            bool PassDiElPh(const xAOD::Electron& el, const xAOD::Photon& ph) const override;
            bool PassDiLoElPh(const xAOD::Electron& el, const xAOD::Photon& ph) const override;

            bool PassAny(const xAOD::ElectronContainer& elc, const xAOD::MuonContainer& muc) const override;

        private:
            ToolHandle<DV::IElecCuts> m_ec;
            ToolHandle<DV::IMuonCuts> m_mc;
            ToolHandle<DV::IPhotonMatch> m_phMatch;

            // trigger flags
            bool m_pass_siphtrig;
            bool m_pass_diphtrig;
            bool m_pass_simutrig;

            // cut values of the filters
            double m_el_eta;
            double m_ph_eta;
            double m_mu_eta;
            double m_mu_beta;

            double m_el_d0;
            double m_mu_d0;

            double m_siel_pt;
            double m_siph_pt;
            double m_siph_xpt;
            double m_simu_pt;
            double m_diel_pt;
            double m_diph_pt;
            double m_dielph_pt;
            double m_diloelph_pt;

            // small helper functions
            inline bool PassEta(const xAOD::IParticle& part, double eta_cut) const;
            inline bool Passd0(const xAOD::Electron& el, double d0_cut) const;
            inline bool Passd0(const xAOD::Muon& mu, double d0_cut) const;
            inline bool IsLoose(const xAOD::Egamma& eg) const;
            inline bool SameCluster(const xAOD::Egamma& eg1, const xAOD::Egamma& eg2) const;

            inline bool PassCuts(const xAOD::Electron& el, double pt_cut, bool loose = false) const;
            inline bool PassCuts(const xAOD::Photon& ph, double pt_cut) const;
            inline bool PassCuts(const xAOD::Muon& mu, double pt_cut, double eta_cut) const;
    };
}

#endif
