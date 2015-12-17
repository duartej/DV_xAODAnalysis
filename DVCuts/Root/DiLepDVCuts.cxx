#include "DVCuts/DiLepDVCuts.h"

#include<cmath>

#include "xAODEgamma/ElectronxAODHelpers.h"

DV::DiLepDVCuts::DiLepDVCuts(const std::string & name) :
    asg::AsgTool(name),
    m_dvc("DV::DVCuts/DiLepBaseCuts"),
    m_desd("DV::DiLepDESD/DiLepDESD"),
    m_ec("DV::ElecCuts/DiLepElecCuts"),
    m_mc("DV::MuonCuts/DiLepMuonCuts"),
    m_over("DV::OverlapRemoval/OverlapRemoval"),
    m_trig("DV::TrigMatch/TrigMatch"),
    m_accEl("DV_Electrons"),
    m_accMu("DV_Muons"),
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

    if(m_dvc.retrieve().isFailure())
    {
        ATH_MSG_ERROR("Failed to retrieve DVCuts!");
        return StatusCode::FAILURE;
    }
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
                                          xAOD::ElectronContainer& elc,
                                          xAOD::MuonContainer& muc) const
{
    // create containers
    auto dv_elc = std::make_shared<xAOD::ElectronContainer>(SG::VIEW_ELEMENTS);
    m_accEl(dv) = dv_elc;

    auto dv_muc = std::make_shared<xAOD::MuonContainer>(SG::VIEW_ELEMENTS);
    m_accMu(dv) = dv_muc;

    // lepton matching (temporary solution, better: pointer comparison...)
    for(const auto& trl: dv.trackParticleLinks())
    {
        auto tr_p4 = (*trl)->p4();

        for(xAOD::Electron* el: elc)
        {
            auto el_p4 = xAOD::EgammaHelpers::getOriginalTrackParticle(el)->p4();

            if(tr_p4 == el_p4)
            {
                dv_elc->push_back(el);
            }
        }
        for(xAOD::Muon* mu: muc)
        {
            const xAOD::TrackParticle* mu_idtr = m_mc->GetTrack(*mu);
            if(mu_idtr == nullptr) continue;

            if(tr_p4 == mu_idtr->p4())
            {
                dv_muc->push_back(mu);
            }
        }
    }
}

void DV::DiLepDVCuts::ApplyOverlapRemoval(const xAOD::Vertex& dv) const
{
    // retrieve particles from vertex
    auto elc = m_accEl(dv);
    auto muc = m_accMu(dv);

    for(auto el = elc->begin(); el != elc->end();)
    {
        if(m_over->IsOverlap(**el)) elc->erase(el);
        else el++;
    }
    for(auto mu = muc->begin(); mu != muc->end();)
    {
        if(m_over->IsOverlap(**mu)) muc->erase(mu);
        else mu++;
    }
}

void DV::DiLepDVCuts::ApplyKinematics(const xAOD::Vertex& dv) const
{
    // retrieve particles from vertex
    auto elc = m_accEl(dv);
    auto muc = m_accMu(dv);

    for(auto el = elc->begin(); el != elc->end();)
    {
        if(!m_ec->PassKinematics(**el)) elc->erase(el);
        else el++;
    }
    for(auto mu = muc->begin(); mu != muc->end();)
    {
        if(!m_mc->PassKinematics(**mu)) muc->erase(mu);
        else mu++;
    }
}

void DV::DiLepDVCuts::ApplyTightness(const xAOD::Vertex& dv) const
{
    // retrieve particles from vertex
    auto elc = m_accEl(dv);
    auto muc = m_accMu(dv);

    for(auto el = elc->begin(); el != elc->end();)
    {
        if(!m_ec->PassID(**el)) elc->erase(el);
        else el++;
    }
    for(auto mu = muc->begin(); mu != muc->end();)
    {
        if(!m_mc->PassID(**mu)) muc->erase(mu);
        else mu++;
    }
}

bool DV::DiLepDVCuts::IsBlinded(const xAOD::EventInfo& ei,
                                const xAOD::Vertex& dv,
                                const xAOD::VertexContainer& pvc) const
{
    if(ei.eventType(xAOD::EventInfo::IS_SIMULATION)) return false;

    // retrieve particles from vertex
    auto elc = m_accEl(dv);
    auto muc = m_accMu(dv);

    if(elc && muc)
    {
        if((elc->size() + muc->size()) < 2) return false;
        if(!m_dvc->PassMassCut(dv)) return false;
        if(!m_dvc->PassDistCut(dv, pvc)) return false;
    }

    return true;
}

void DV::DiLepDVCuts::DoTriggerMatching(xAOD::Vertex& dv) const
{
    // retrieve particles from vertex
    auto elc = m_accEl(dv);
    auto muc = m_accMu(dv);

    // trigger matching
    m_accTrigSiPh(dv) = 0;
    m_accTrigDiPh(dv) = 0;
    m_accTrigSiMu(dv) = 0;

    bool siph_matched = false;
    unsigned int count_di = 0;
    for(xAOD::Electron* el: *elc)
    {
        if(!siph_matched && m_trig->Match(*el, m_trig_siph))
        {
            siph_matched = true;
            m_accTrigSiPh(dv) = 1;
        }
        if(count_di < 2 && m_trig->Match(*el, m_trig_diph)) count_di++;
    }
    if(count_di == 2) m_accTrigDiPh(dv) = 1;

    for(xAOD::Muon* mu: *muc)
    {
        if(m_trig->Match(*mu, m_trig_simu))
        {
            m_accTrigSiMu(dv) = 1;
            break;
        }
    }
}

const std::shared_ptr<xAOD::ElectronContainer> DV::DiLepDVCuts::GetEl(const xAOD::Vertex& dv) const
{
    if(!m_accEl.isAvailable(dv))
    {
        ATH_MSG_WARNING("Failed to retrieve electrons from DV!");
        return nullptr;
    }

    return m_accEl(dv);
}

const std::shared_ptr<xAOD::MuonContainer> DV::DiLepDVCuts::GetMu(const xAOD::Vertex& dv) const
{
    if(!m_accMu.isAvailable(dv))
    {
        ATH_MSG_WARNING("Failed to retrieve muons from DV!");
        return nullptr;
    }

    return m_accMu(dv);
}

DV::DiLepTypes DV::DiLepDVCuts::GetType(const xAOD::Vertex& dv) const
{
    // retrieve particles from vertex
    auto elc = m_accEl(dv);
    auto muc = m_accMu(dv);

    if(elc && muc)
    {
        if(elc->size() >= 1 && muc->size() >= 1) return DV::DiLepTypes::em;
        else if(elc->size() >= 2) return DV::DiLepTypes::ee;
        else if(muc->size() >= 2) return DV::DiLepTypes::mm;
    }

    return DV::DiLepTypes::None;
}

std::string DV::DiLepDVCuts::GetTypeStr(const xAOD::Vertex& dv) const
{
    auto dv_type = GetType(dv);

    switch(dv_type)
    {
        case DV::DiLepTypes::ee : return "ee";
        case DV::DiLepTypes::em : return "em";
        case DV::DiLepTypes::mm : return "mm";
        default : return "";
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

bool DV::DiLepDVCuts::PassNLeptons(const xAOD::Vertex& dv) const
{
    // retrieve particles from vertex
    auto elc = m_accEl(dv);
    auto muc = m_accMu(dv);

    if(elc && muc)
    {
        return (elc->size() + muc->size()) >= 2;
    }

    return false;
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
    // retrieve particles from vertex
    auto elc = m_accEl(dv);
    auto muc = m_accMu(dv);

    if(elc && muc)
    {
        m_desd->SetTriggerFlags(m_accTrigSiPh(dv),
                                m_accTrigDiPh(dv),
                                m_accTrigSiMu(dv));

        return m_desd->PassAny(*elc, *muc);
    }

    return false;
}
