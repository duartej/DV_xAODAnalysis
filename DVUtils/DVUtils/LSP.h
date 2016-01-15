#ifndef DV_LSP_H
#define DV_LSP_H

#include <cmath>
#include <vector>

#include "TVector2.h"
#include "TVector3.h"

#include "xAODEgamma/ElectronContainer.h"
#include "xAODEgamma/PhotonContainer.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODTruth/TruthParticleContainer.h"
#include "xAODTruth/TruthVertex.h"

namespace DV
{
    class LSP
    {
        public:
            static std::vector<DV::LSP*> GetLSPs(const xAOD::TruthParticleContainer& tpc);

            // min values for dR match (from max)
            static constexpr double DRMIN_PH = 0.2;
            static constexpr double DRMIN_EL = 0.1;
            static constexpr double DRMIN_MU = 0.1;

            // matches reconstructed particles to childs (using deltaR match)
            bool MatchReco(const xAOD::ElectronContainer& elc,
                           const xAOD::PhotonContainer& phc,
                           const xAOD::MuonContainer& muc);

            const xAOD::TruthVertex* GetVtx() const;

            const std::vector<const xAOD::TruthParticle*>& GetChilds() const;
            std::size_t GetNElChilds() const;
            std::size_t GetNMuChilds() const;

            const std::vector<const xAOD::Electron*>& GetEl() const;
            const std::vector<const xAOD::Muon*>& GetMu() const;
            const std::vector<const xAOD::Photon*>& GetPh() const;

        private:
            // constructor
            LSP(const xAOD::TruthParticle* t);

            // computes refined dR between reco particle and truth particle
            static double RefinedDR(const xAOD::IParticle& reco, const xAOD::TruthParticle& tru);

            // decorators
            SG::AuxElement::Decorator<const xAOD::Electron*> m_decElMatch;
            SG::AuxElement::Decorator<const xAOD::Photon*> m_decPhMatch;
            SG::AuxElement::Decorator<const xAOD::Muon*> m_decMuMatch;
            SG::AuxElement::Decorator<const xAOD::TruthParticle*> m_decTruMatch;
            SG::AuxElement::Decorator<double> m_decDR;

            // truth data of LSP
            const xAOD::TruthParticle* m_tp;

            // truth childs
            std::vector<const xAOD::TruthParticle*> m_childs;

            // number of electron and muon childs
            std::size_t m_nEl;
            std::size_t m_nMu;

            // matched reco childs
            std::vector<const xAOD::Electron*> m_el;
            std::vector<const xAOD::Muon*> m_mu;
            std::vector<const xAOD::Photon*> m_ph;
    };
}

#endif
