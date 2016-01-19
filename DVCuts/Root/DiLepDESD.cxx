#include "DVCuts/DiLepDESD.h"

DV::DiLepDESD::DiLepDESD(const std::string& name) :
    AsgTool(name),
    m_tdt("Trig::TrigDecisionTool/TrigDecisionTool"),
    m_ec("DV::ElecCuts/DiLepElecCuts"),
    m_mc("DV::MuonCuts/DiLepMuonCuts"),
    m_phMatch("DV::PhotonMatch/PhotonMatch"),
    m_pass_siphtrig(false),
    m_pass_diphtrig(false),
    m_pass_simutrig(false)
{
    declareProperty("SiPhTrigger", m_siphtrig = "HLT_g140_loose", "Name of single photon trigger");
    declareProperty("DiPhTrigger", m_diphtrig = "HLT_2g50_loose", "Name of diphoton trigger");
    declareProperty("SimuTrigger", m_simutrig = "HLT_mu60_0eta105_msonly", "Name of single muon trigger");

    declareProperty("ElEtaMax", m_el_eta = 2.5, "Cut for electron |eta|");
    declareProperty("PhEtaMax", m_ph_eta = 2.5, "Cut for photon |eta|");
    declareProperty("MuEtaMax", m_mu_eta = 2.5, "Cut for muon |eta|");
    declareProperty("MuBaEtaMax", m_mu_beta = 1.07, "Cut for muon |eta| in the barrel region");

    declareProperty("ElD0Min", m_el_d0 = 2.0, "Cut for electron track |d0| [mm]");
    declareProperty("MuD0Min", m_mu_d0 = 1.5, "Cut for muon track |d0| [mm]");

    declareProperty("MuChi2DoF", m_mu_chi2 = 50.0, "Cut for chi2/DoF of muon track match");

    declareProperty("SiElPtMin", m_siel_pt = 140000.0, "Pt cut for single electron filter [MeV]");
    declareProperty("SiPhPtMin", m_siph_pt = 150000.0, "Pt cut for single photon filter [MeV]");
    declareProperty("SiPhXPtMin", m_siph_xpt = 10000.0, "Pt cut for X of single photon+X filter [MeV]");
    declareProperty("SiMuPtMin", m_simu_pt = 60000.0, "Pt cut for single muon filter [MeV]");
    declareProperty("DiElPtMin", m_diel_pt = 50000.0, "Pt cut for dielectron filter [MeV]");
    declareProperty("DiPhPtMin", m_diph_pt = 50000.0, "Pt cut for diphoton filter [MeV]");
    declareProperty("DiElPhPtMin", m_dielph_pt = 50000.0, "Pt cut for electron+photon filter [MeV]");
    declareProperty("DiLoElPhPtMin", m_diloelph_pt = 50000.0, "Pt cut for loose electron+photon filter [MeV]");
}

StatusCode DV::DiLepDESD::initialize()
{
    // Greet the user:
    ATH_MSG_DEBUG("Initialising... ");

    if(m_tdt.retrieve().isFailure())
    {
        ATH_MSG_ERROR("Failed to retrieve TrigDecisionTool!");
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
    if(m_phMatch.retrieve().isFailure())
    {
        ATH_MSG_ERROR("Failed to retrieve PhotonMatch!");
        return StatusCode::FAILURE;
    }

    // Return gracefully:
    return StatusCode::SUCCESS;
}

void DV::DiLepDESD::RetrieveTriggerFlags()
{
    m_pass_siphtrig = m_tdt->isPassed(m_siphtrig);
    m_pass_diphtrig = m_tdt->isPassed(m_diphtrig);
    m_pass_simutrig = m_tdt->isPassed(m_simutrig);
}

void DV::DiLepDESD::SetTriggerFlags(bool siph, bool diph, bool simu)
{
    m_pass_siphtrig = siph;
    m_pass_diphtrig = diph;
    m_pass_simutrig = simu;
}

bool DV::DiLepDESD::PassSiEl(const xAOD::Electron& el) const
{
    if(!m_pass_siphtrig) return false;

    if(!m_ec->IsPreRetracking(el)) return false;

    return PassCuts(el, m_siel_pt);
}

bool DV::DiLepDESD::PassSiPhX(const xAOD::Photon& ph, const xAOD::Electron& el) const
{
    if(!m_pass_siphtrig) return false;

    if(!m_ec->IsPreRetracking(el)) return false;

    if(SameCluster(ph, el)) return false;

    if(!PassCuts(ph, m_siph_pt)) return false;
    if(!PassCuts(el, m_siph_xpt)) return false;

    return true;
}

bool DV::DiLepDESD::PassSiPhX(const xAOD::Photon& ph1, const xAOD::Photon& ph2) const
{
    if(!m_pass_siphtrig) return false;

    if(SameCluster(ph1, ph2)) return false;

    if(PassCuts(ph1, m_siph_pt) && PassCuts(ph2, m_siph_xpt)) return true;
    if(PassCuts(ph2, m_siph_pt) && PassCuts(ph1, m_siph_xpt)) return true;

    return false;
}

bool DV::DiLepDESD::PassSiPhX(const xAOD::Photon& ph, const xAOD::Muon& mu) const
{
    if(!m_pass_siphtrig) return false;

    if(!m_mc->IsPreRetracking(mu)) return false;

    if(!PassCuts(ph, m_siph_pt)) return false;
    if(!PassCuts(mu, m_siph_xpt, m_mu_eta)) return false;

    return true;
}

bool DV::DiLepDESD::PassSiMu(const xAOD::Muon& mu) const
{
    if(!m_pass_simutrig) return false;

    if(!m_mc->IsPreRetracking(mu)) return false;

    return PassCuts(mu, m_simu_pt, m_mu_beta);
}

bool DV::DiLepDESD::PassDiEl(const xAOD::Electron& el1, const xAOD::Electron& el2) const
{
    if(!m_pass_diphtrig) return false;

    if(!m_ec->IsPreRetracking(el1)) return false;
    if(!m_ec->IsPreRetracking(el2)) return false;

    if(SameCluster(el1, el2)) return false;

    if(!PassCuts(el1, m_diel_pt)) return false;
    if(!PassCuts(el2, m_diel_pt)) return false;

    return true;
}

bool DV::DiLepDESD::PassDiPh(const xAOD::Photon& ph1, const xAOD::Photon& ph2) const
{
    if(!m_pass_diphtrig) return false;

    if(SameCluster(ph1, ph2)) return false;

    if(!PassCuts(ph1, m_diph_pt)) return false;
    if(!PassCuts(ph2, m_diph_pt)) return false;

    return true;
}

bool DV::DiLepDESD::PassDiElPh(const xAOD::Electron& el, const xAOD::Photon& ph) const
{
    if(!m_pass_diphtrig) return false;

    if(!m_ec->IsPreRetracking(el)) return false;

    if(SameCluster(el, ph)) return false;

    if(!PassCuts(el, m_dielph_pt)) return false;
    if(!PassCuts(ph, m_dielph_pt)) return false;

    return true;
}

bool DV::DiLepDESD::PassDiLoElPh(const xAOD::Electron& el, const xAOD::Photon& ph) const
{
    if(!m_pass_diphtrig) return false;

    if(!m_ec->IsPreRetracking(el)) return false;

    if(SameCluster(el, ph)) return false;

    if(!PassCuts(el, m_diloelph_pt, true)) return false;
    if(!PassCuts(ph, m_diloelph_pt)) return false;

    return true;
}

bool DV::DiLepDESD::PassEta(const xAOD::IParticle& part, double eta_cut) const
{
    return std::fabs(part.eta()) < eta_cut;
}

bool DV::DiLepDESD::Passd0(const xAOD::Electron& el, double d0_cut) const
{
    const xAOD::TrackParticle* el_tr = el.trackParticle();
    if(el_tr == nullptr)
    {
        ATH_MSG_WARNING("Failed to retrieve electron track!");
        return false;
     }

    return std::fabs(el_tr->d0()) > d0_cut;
}

bool DV::DiLepDESD::Passd0(const xAOD::Muon& mu, double d0_cut) const
{
    const xAOD::TrackParticle* mu_tr = mu.trackParticle(xAOD::Muon::InnerDetectorTrackParticle);
    if(mu_tr == nullptr)
    {
        ATH_MSG_WARNING("Failed to retrieve muon ID track!");
        return false;
    }

    return std::fabs(mu_tr->d0()) > d0_cut;
}

bool DV::DiLepDESD::IsLoose(const xAOD::Egamma& eg) const
{
    bool isLoose = false;
    eg.passSelection(isLoose, "Loose");
    return isLoose;
}

bool DV::DiLepDESD::SameCluster(const xAOD::Egamma& eg1, const xAOD::Egamma& eg2) const
{
    const xAOD::CaloCluster* calo1 = eg1.caloCluster();
    const xAOD::CaloCluster* calo2 = eg2.caloCluster();
    if(calo1 == nullptr || calo2 == nullptr)
    {
        ATH_MSG_WARNING("Failed to retrieve CaloCluster!");
        return false;
    }

    return calo1->p4().DeltaR(calo2->p4()) < 0.01;
}

bool DV::DiLepDESD::IsGood(const xAOD::Muon& mu) const
{
    if(mu.muonType() != xAOD::Muon::Combined) return false;

    float chi2 = 0.;
    if(!mu.parameter(chi2, xAOD::Muon::msInnerMatchChi2)) return false;

    int dof = 1;
    if(!mu.parameter(dof, xAOD::Muon::msInnerMatchDOF)) return false;
    if(dof == 0) dof = 1;

    return (chi2 / static_cast<float>(dof)) < m_mu_chi2;
}

bool DV::DiLepDESD::PassCuts(const xAOD::Electron& el, double pt_cut, bool loose) const
{
    if(el.pt() < pt_cut) return false;
    if(!PassEta(el, m_el_eta)) return false;

    if(loose)
    {
        if(!IsLoose(el)) return false;
    }
    else
    {
        if(!Passd0(el, m_el_d0)) return false;
    }

    return true;
}

bool DV::DiLepDESD::PassCuts(const xAOD::Photon& ph, double pt_cut) const
{
    if(ph.pt() < pt_cut) return false;
    if(!PassEta(ph, m_ph_eta)) return false;
    if(!IsLoose(ph)) return false;

    return true;
}

bool DV::DiLepDESD::PassCuts(const xAOD::Muon& mu, double pt_cut, double eta_cut) const
{
    const xAOD::IParticle* mu_ip = nullptr;

    if(IsGood(mu))
    {
        if(!Passd0(mu, m_mu_d0)) return false;

        mu_ip = &mu;
    }
    else
    {
        const xAOD::TrackParticle* mu_mstr = mu.trackParticle(xAOD::Muon::MuonSpectrometerTrackParticle);
        if(mu_mstr == nullptr) return false;

        mu_ip = mu_mstr;
    }

    if(mu_ip->pt() < pt_cut) return false;
    if(!PassEta(*mu_ip, eta_cut)) return false;

    return true;
}

bool DV::DiLepDESD::PassAny(const xAOD::ElectronContainer& elc, const xAOD::MuonContainer& muc) const
{
    for(auto el1 = elc.cbegin(); el1 != elc.cend(); el1++)
    {
        const xAOD::Photon* ph1 = m_phMatch->GetPhoton(**el1);

        if(PassSiEl(**el1)) return true;

        for(auto el2 = el1+1; el2 != elc.cend(); el2++)
        {
            const xAOD::Photon* ph2 = m_phMatch->GetPhoton(**el2);

            if(PassDiEl(**el1, **el2)) return true;
            if(ph1)
            {
                if(PassSiPhX(*ph1, **el2)) return true;
                if(PassDiElPh(**el2, *ph1)) return true;
                if(PassDiLoElPh(**el2, *ph1)) return true;
            }
            if(ph1 && ph2)
            {
                if(PassSiPhX(*ph1, *ph2)) return true;
                if(PassDiPh(*ph1, *ph2)) return true;
            }
        }

        if(!ph1) continue;

        for(const xAOD::Muon* mu: muc)
        {
            if(PassSiPhX(*ph1, *mu)) return true;
        }
    }

    for(const xAOD::Muon* mu: muc)
    {
        if(PassSiMu(*mu)) return true;
    }

    return false;
}

bool DV::DiLepDESD::PassSiElCuts(const xAOD::Electron& el) const
{
  return PassCuts(el, m_siel_pt);
}

bool DV::DiLepDESD::PassSiPhCuts(const xAOD::Photon& ph) const
{
  return PassCuts(ph, m_siph_pt);
}

bool DV::DiLepDESD::PassSiPhXCuts(const xAOD::Electron& el) const
{
  return PassCuts(el, m_siph_xpt);
}

bool DV::DiLepDESD::PassSiPhXCuts(const xAOD::Photon& ph) const
{
  return PassCuts(ph, m_siph_xpt);
}

bool DV::DiLepDESD::PassSiPhXCuts(const xAOD::Muon& mu) const
{
  return PassCuts(mu, m_siph_xpt, m_mu_eta);
}

bool DV::DiLepDESD::PassSiMuCuts(const xAOD::Muon& mu) const
{
  return PassCuts(mu, m_simu_pt, m_mu_beta);
}

bool DV::DiLepDESD::PassDiElCuts(const xAOD::Electron& el) const
{
  return PassCuts(el, m_diel_pt);
}

bool DV::DiLepDESD::PassDiPhCuts(const xAOD::Photon& ph) const
{
  return PassCuts(ph, m_diph_pt);
}

bool DV::DiLepDESD::PassDiElPhCuts(const xAOD::Electron& el) const
{
  return PassCuts(el, m_dielph_pt);
}

bool DV::DiLepDESD::PassDiElPhCuts(const xAOD::Photon& ph) const
{
  return PassCuts(ph, m_dielph_pt);
}

bool DV::DiLepDESD::PassDiLoElPhCuts(const xAOD::Electron& el) const
{
  return PassCuts(el, m_diloelph_pt, true);
}

bool DV::DiLepDESD::PassDiLoElPhCuts(const xAOD::Photon& ph) const
{
  return PassCuts(ph, m_diloelph_pt);
}

