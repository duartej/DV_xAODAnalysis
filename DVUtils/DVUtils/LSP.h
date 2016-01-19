#ifndef DV_LSP_H
#define DV_LSP_H

#include <cmath>
#include <memory>
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
            // constructor
            LSP(const xAOD::TruthParticle* t);
            // destructor
            ~LSP() = default;

            // builds LSP objects using truth particle container
            static std::vector<std::shared_ptr<DV::LSP>> GetLSPs(const xAOD::TruthParticleContainer& tpc);

            // computes refined dR between calo cluster and truth particle
            static double RefinedDR(const xAOD::CaloCluster& reco, const xAOD::TruthParticle& tru);

            // min values for dR match
            static constexpr double DRMIN_PH = 0.1;
            static constexpr double DRMIN_EL = 0.1;
            static constexpr double DRMIN_MU = 0.1;

            // matches reconstructed particles to childs (using deltaR match)
            void MatchReco(const xAOD::ElectronContainer& elc,
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

            // number of electron and muon truth childs
            std::size_t m_nEl;
            std::size_t m_nMu;

            // matched reco particles
            std::vector<const xAOD::Electron*> m_el;
            std::vector<const xAOD::Muon*> m_mu;
            std::vector<const xAOD::Photon*> m_ph;
    };
}

#endif
