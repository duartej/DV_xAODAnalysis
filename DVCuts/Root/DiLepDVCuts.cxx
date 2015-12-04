#include "DVCuts/DiLepDVCuts.h"

#include<cmath>

DV::DiLepDVCuts::DiLepDVCuts(const std::string & name) :
    asg::AsgTool(name),
    m_desd(""), m_ec(""), m_mc(""), m_over(""), m_trig(""),
    m_accEl("DV_ElCont"),
    m_accPh("DV_PhCont"),
    m_accMu("DV_MuCont"),
    m_accTrigSiPh("DV_SiPhTrigMatch"),
    m_accTrigDiPh("DV_DiPhTrigMatch"),
    m_accTrigSiMu("DV_SiMuTrigMatch")
{
    declareProperty("SiPhTrigger", m_trig_siph = "HLT_g140_loose", "Name of single photon trigger");
    declareProperty("DiPhTrigger", m_trig_diph = "HLT_2g50_loose", "Name of diphoton trigger");
    declareProperty("SiMuTrigger", m_trig_simu = "HLT_mu60_0eta105_msonly", "Name of single muon trigger");
    declareProperty("centEtaMax", m_centEta = 0.02, "Cut on |eta| for the central track veto");
}

StatusCode DV::DiLepDVCuts::initialize()
{
    // Greet the user:
    ATH_MSG_DEBUG("Initialising... ");

    if(m_desd.retrieve().isFailure())
    {
        ATH_MSG_ERROR("Failed to retrieve DiLepDESD!");
        return StatusCode::FAILURE;
    }
    if(m_ec.retrieve().isFailure())
    {
        ATH_MSG_ERROR("Failed to retrieve ElecCuts!");
        return StatusCode::FAILURE;
    }
    if(m_mc.retrieve().isFailure())
    {
        ATH_MSG_ERROR("Failed to retrieve MuonCuts!");
        return StatusCode::FAILURE;
    }
    if(m_over.retrieve().isFailure())
    {
        ATH_MSG_ERROR("Failed to retrieve OverlapRemoval!");
        return StatusCode::FAILURE;
    }
    if(m_trig.retrieve().isFailure())
    {
        ATH_MSG_ERROR("Failed to retrieve TrigMatch!");
        return StatusCode::FAILURE;
    }

    // Return gracefully:
    return StatusCode::SUCCESS;
}

void DV::DiLepDVCuts::ApplyLeptonMatching(xAOD::Vertex& dv,
                                          const xAOD::ElectronContainer& elc,
                                          const xAOD::PhotonContainer& phc,
                                          const xAOD::MuonContainer& muc) const
{
    xAOD::ElectronContainer* dv_elc = new xAOD::ElectronContainer();
    xAOD::PhotonContainer* dv_phc = new xAOD::PhotonContainer();
    xAOD::MuonContainer* dv_muc = new xAOD::MuonContainer();
    // FIXME: stores
    // FIXME: memory leaks (where should the containers be deleted?)

    for(const auto& trl: dv.trackParticleLinks())
    {
        for(const xAOD::Electron* el: elc)
        {
            if((*trl) == m_ec->GetTrack(*el))
            {
                xAOD::Electron* el_ptr = new xAOD::Electron();
                el_ptr->makePrivateStore(*el);
                dv_elc->push_back(el_ptr);

                // find associated photon
                const xAOD::Photon* ph = m_ec->FindMatchingPhoton(*el, phc);
                if(ph)
                {
                    xAOD::Photon* ph_ptr = new xAOD::Photon();
                    ph_ptr->makePrivateStore(*ph);
                    dv_phc->push_back(ph_ptr);
                }
            }
        }
        for(const xAOD::Muon* mu: muc)
        {
            if((*trl) == m_mc->GetTrack(*mu))
            {
                xAOD::Muon* mu_ptr = new xAOD::Muon();
                mu_ptr->makePrivateStore(*mu);
                dv_muc->push_back(mu_ptr);
            }
        }
    }

    m_accEl(dv) = dv_elc;
    m_accMu(dv) = dv_muc;
}

void DV::DiLepDVCuts::ApplyOverlapRemoval(const xAOD::Vertex& dv) const
{
    // retrieve leptons from vertex
    xAOD::ElectronContainer* elc = nullptr;
    xAOD::MuonContainer* muc = nullptr;
    GetLeptonContainers(dv, elc, muc);

    if(elc && muc)
    {
        m_over->RemoveOverlap(*elc, *muc);
    }
    else
    {
        ATH_MSG_ERROR("Overlap removal could not be applied!");
    }
}

void DV::DiLepDVCuts::ApplyTriggerMatching(xAOD::Vertex& dv) const
{
    // retrieve leptons from vertex
    xAOD::ElectronContainer* elc = nullptr;
    xAOD::MuonContainer* muc = nullptr;
    GetLeptonContainers(dv, elc, muc);

    m_accTrigSiPh(dv) = 0;
    m_accTrigDiPh(dv) = 0;
    m_accTrigSiMu(dv) = 0;

    if(elc && muc)
    {
        unsigned int count_di = 0;
        for(xAOD::Electron* el: *elc)
        {
            if(m_over->IsOverlap(*el)) continue;

            if(m_trig->Match(*el, m_trig_siph)) m_accTrigSiPh(dv) = 1;
            if(m_trig->Match(*el, m_trig_diph)) count_di++;
        }
        if(count_di >= 2) m_accTrigDiPh(dv) = 1;

        for(xAOD::Muon* mu: *muc)
        {
            if(m_over->IsOverlap(*mu)) continue;

            if(m_trig->Match(*mu, m_trig_simu))
            {
                m_accTrigSiMu(dv) = 1;
                break;
            }
        }
    }
    else
    {
        ATH_MSG_ERROR("Triggers could not be matched!");
    }
}

bool DV::DiLepDVCuts::PassCentralEtaVeto(const xAOD::Vertex& dv) const
{
    for(const auto& trl: dv.trackParticleLinks())
    {
        if(std::fabs((*trl)->eta()) < 0.02) return false;
    }
    return true;
}

bool DV::DiLepDVCuts::PassChargeRequirement(const xAOD::Vertex& dv) const
{
    bool has_pos = false;
    bool has_neg = false;

    for(const auto& trl: dv.trackParticleLinks())
    {
        has_pos |= ((*trl)->charge() > 0);
        has_neg |= ((*trl)->charge() < 0);
    }

    return has_pos && has_neg;
}

bool DV::DiLepDVCuts::PassTriggerMatching(const xAOD::Vertex& dv) const
{
    if(m_accTrigSiPh(dv) == 1) return true;
    if(m_accTrigDiPh(dv) == 1) return true;
    if(m_accTrigSiMu(dv) == 1) return true;

    return false;
}

bool DV::DiLepDVCuts::PassDESDMatching(const xAOD::Vertex& dv) const
{
    // retrieve containers from vertex
    xAOD::ElectronContainer* elc = nullptr;
    xAOD::MuonContainer* muc = nullptr;
    GetLeptonContainers(dv, elc, muc);

    xAOD::PhotonContainer* phc = nullptr;
    GetPhotonContainer(dv, phc);

    if(elc && phc && muc)
    {
        m_desd->SetTriggerFlags(m_accTrigSiPh(dv),
                                m_accTrigDiPh(dv),
                                m_accTrigSiMu(dv));

        return m_desd->PassAny(*elc, *phc, *muc);
    }
    else
    {
        ATH_MSG_ERROR("DESD filters could not be matched!");
    }

    return false;
}

void DV::DiLepDVCuts::GetLeptonContainers(const xAOD::Vertex& dv,
                                          xAOD::ElectronContainer*& elc,
                                          xAOD::MuonContainer*& muc) const
{
    elc = m_accEl(dv);
    if(!elc)
    {
        ATH_MSG_ERROR("Failed to retrieve electrons from DV!");
    }

    muc = m_accMu(dv);
    if(!muc)
    {
        ATH_MSG_ERROR("Failed to retrieve muons from DV!");
    }
}

void DV::DiLepDVCuts::GetPhotonContainer(const xAOD::Vertex& dv,
                                          xAOD::PhotonContainer*& phc) const
{
    phc = m_accPh(dv);
    if(!phc)
    {
        ATH_MSG_ERROR("Failed to retrieve photons from DV!");
    }
}
