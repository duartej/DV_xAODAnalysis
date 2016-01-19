#include "DVUtils/LSP.h"

constexpr double DV::LSP::DRMIN_PH;
constexpr double DV::LSP::DRMIN_EL;
constexpr double DV::LSP::DRMIN_MU;

DV::LSP::LSP(const xAOD::TruthParticle* t)
  : m_decElMatch("DV_MatchedEl"),
    m_decPhMatch("DV_MatchedPh"),
    m_decMuMatch("DV_MatchedMu"),
    m_decTruMatch("DV_MatchedTruPart"),
    m_decDR("DV_dRTruPart"),
    m_tp(t),
    m_nEl(0),
    m_nMu(0)
{
    // find final state children
    for(std::size_t c = 0; c < m_tp->nChildren(); c++)
    {
        const xAOD::TruthParticle* LSP_child = m_tp->child(c);
        int mc_status = m_tp->child(c)->status();

        while(mc_status != 1 && LSP_child->nChildren() > 0)
        {
            LSP_child = LSP_child->child();
            mc_status = LSP_child->status();
        }
        // no final state child found (should not happen)
        if(mc_status != 1) continue;

        if(LSP_child->isElectron())
        {
            m_nEl++;

            m_decElMatch(*LSP_child) = nullptr;
            m_decPhMatch(*LSP_child) = nullptr;
        }
        else if(LSP_child->isMuon())
        {
            m_nMu++;

            m_decMuMatch(*LSP_child) = nullptr;
        }

        m_childs.push_back(LSP_child);
    }
}

std::vector<std::shared_ptr<DV::LSP>> DV::LSP::GetLSPs(const xAOD::TruthParticleContainer& tpc)
{
    std::vector<std::shared_ptr<DV::LSP>> LSP_store;

    // works only for trilinear couplings
    for(const xAOD::TruthParticle* tp: tpc)
    {
        if(tp->absPdgId() != 1000022) continue;
        if(!tp->hasDecayVtx()) continue;
        if(tp->nChildren() != 3) continue;

        auto LSP = std::make_shared<DV::LSP>(tp);
        LSP_store.push_back(LSP);
    }

    return LSP_store;
}

double DV::LSP::RefinedDR(const xAOD::CaloCluster& reco, const xAOD::TruthParticle& tru)
{
    // compute "endcap" radius, compare to "barrel" radius
    double radius = 3900.0 / std::fabs(std::sinh(reco.eta()));
    if(radius > 1950.0) radius = 1950.0;

    // compute refined direction of reco particle
    TVector3 refdir;
    refdir.SetPtEtaPhi(radius, reco.eta(), reco.phi());

    const xAOD::TruthVertex* DV = tru.prodVtx();
    TVector3 DVpos(DV->x(), DV->y(), DV->z());

    refdir = refdir - DVpos;

    // compute deltaR of refined reco particle and LSP child
    double deta = refdir.Eta() - tru.eta();
    double dphi = TVector2::Phi_mpi_pi(refdir.Phi() - tru.phi());

    return std::sqrt(deta*deta + dphi*dphi);
}

void DV::LSP::MatchReco(const xAOD::ElectronContainer& elc,
                        const xAOD::PhotonContainer& phc,
                        const xAOD::MuonContainer& muc)
{
    std::size_t nLep = m_nEl + m_nMu;
    if(nLep == 0) return;

    std::vector<const xAOD::TruthParticle*> el_childs;
    std::vector<const xAOD::TruthParticle*> mu_childs;
    for(const xAOD::TruthParticle* child: m_childs)
    {
        if(child->isElectron())
        {
            el_childs.push_back(child);
        }
        else if(child->isMuon())
        {
            mu_childs.push_back(child);
        }
    }

    std::vector<const xAOD::Electron*> matched_el(m_nEl, nullptr);
    std::vector<const xAOD::Photon*> matched_ph(m_nEl, nullptr);
    std::vector<const xAOD::Muon*> matched_mu(m_nMu, nullptr);

    std::vector<double> matched_el_dr(m_nEl, DRMIN_EL);
    std::vector<double> matched_ph_dr(m_nEl, DRMIN_PH);
    std::vector<double> matched_mu_dr(m_nMu, DRMIN_MU);

    // dR truth matching
    for(std::size_t i = 0; i < m_nEl; i++)
    {
        for(const xAOD::Electron* el: elc)
        {
            double dr = RefinedDR(*(el->caloCluster()), *(el_childs.at(i)));
            if(dr < matched_el_dr.at(i))
            {
                matched_el_dr.at(i) = dr;
                matched_el.at(i) = el;
            }
        }

        for(const xAOD::Photon* ph: phc)
        {
            double dr = RefinedDR(*(ph->caloCluster()), *(el_childs.at(i)));
            if(dr < matched_ph_dr.at(i))
            {
                matched_ph_dr.at(i) = dr;
                matched_ph.at(i) = ph;
            }
        }
    }
    for(std::size_t i = 0; i < m_nMu; i++)
    {
        for(const xAOD::Muon* mu: muc)
        {
            const xAOD::IParticle* mu_ip = nullptr;

            // use MS track which is not affected by badly matched ID tracks
            auto mstrk = mu->trackParticle(xAOD::Muon::MuonSpectrometerTrackParticle);
            if(mstrk == nullptr)
            {
                mu_ip = mu;
            }
            else
            {
                mu_ip = mstrk;
            }

            double dr = mu_childs.at(i)->p4().DeltaR(mu_ip->p4());
            if(dr < matched_mu_dr.at(i))
            {
                matched_mu_dr.at(i) = dr;
                matched_mu.at(i) = mu;
            }
        }
    }

    // resolve ambiguities
    // a reco particle can only be matched to one truth child
    // in most cases this should do nothing
    if(m_nEl > 1)
    {
        for(std::size_t i = 0; i < m_nEl; i++)
        {
            for(std::size_t j = i+1; j < m_nEl; j++)
            {
                if(matched_el.at(i) != nullptr && matched_el.at(i) == matched_el.at(j))
                {
                    if(matched_el_dr.at(i) < matched_el_dr.at(j))
                    {
                        matched_el.at(j) = nullptr;
                    }
                    else
                    {
                        matched_el.at(i) = nullptr;
                    }
                }
                if(matched_ph.at(i) != nullptr && matched_ph.at(i) == matched_ph.at(j))
                {
                    if(matched_ph_dr.at(i) < matched_ph_dr.at(j))
                    {
                        matched_ph.at(j) = nullptr;
                    }
                    else
                    {
                        matched_ph.at(i) = nullptr;
                    }
                }
            }
        }
    }
    if(m_nMu > 1)
    {
        for(std::size_t i = 0; i < m_nMu; i++)
        {
            for(std::size_t j = i+1; j < m_nMu; j++)
            {
                if(matched_mu.at(i) != nullptr && matched_mu.at(i) == matched_mu.at(j))
                {
                    if(matched_mu_dr.at(i) < matched_mu_dr.at(j))
                    {
                        matched_mu.at(j) = nullptr;
                    }
                    else
                    {
                        matched_mu.at(i) = nullptr;
                    }
                }
            }
        }
    }

    // store matched reco particles
    for(size_t i = 0; i < m_nEl; i++)
    {
        if(matched_el.at(i) != nullptr)
        {
            m_decElMatch(*el_childs.at(i)) = matched_el.at(i);

            m_decTruMatch(*matched_el.at(i)) = el_childs.at(i);
            m_decDR(*matched_el.at(i)) = matched_el_dr.at(i);

            m_el.push_back(matched_el.at(i));
        }
        if(matched_ph.at(i) != nullptr)
        {
            m_decPhMatch(*el_childs.at(i)) = matched_ph.at(i);

            m_decTruMatch(*matched_ph.at(i)) = el_childs.at(i);
            m_decDR(*matched_ph.at(i)) = matched_ph_dr.at(i);

            m_ph.push_back(matched_ph.at(i));
       }
    }
    for(size_t i = 0; i < m_nMu; i++)
    {
        if(matched_mu.at(i) != nullptr)
        {
            m_decMuMatch(*mu_childs.at(i)) = matched_mu.at(i);

            m_decTruMatch(*matched_mu.at(i)) = mu_childs.at(i);
            m_decDR(*matched_mu.at(i)) = matched_mu_dr.at(i);

            m_mu.push_back(matched_mu.at(i));
        }
    }
}

const xAOD::TruthVertex* DV::LSP::GetVtx() const
{
    return m_tp->decayVtx();
}

const std::vector<const xAOD::TruthParticle*>& DV::LSP::GetChilds() const
{
    return m_childs;
}

std::size_t DV::LSP::GetNElChilds() const
{
    return m_nEl;
}

std::size_t DV::LSP::GetNMuChilds() const
{
    return m_nMu;
}

const std::vector<const xAOD::Electron*>& DV::LSP::GetEl() const
{
    return m_el;
}

const std::vector<const xAOD::Muon*>& DV::LSP::GetMu() const
{
    return m_mu;
}

const std::vector<const xAOD::Photon*>& DV::LSP::GetPh() const
{
    return m_ph;
}
