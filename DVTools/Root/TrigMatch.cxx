#include "DVTools/TrigMatch.h"

#include "xAODEgamma/PhotonContainer.h"

DV::TrigMatch::TrigMatch(const std::string& name) :
    AsgTool(name),
    m_tdt("Trig::TrigDecisionTool/TrigDecisionTool"),
    m_tmm("Trig::TrigMuonMatching/TrigMuonMatching")
{
}

StatusCode DV::TrigMatch::initialize()
{
    // Greet the user:
    ATH_MSG_DEBUG("Initialising... " );

    // retrieve TrigDecisionTool
    if(m_tdt.retrieve().isFailure())
    {
        ATH_MSG_ERROR("Failed to retrieve TrigDecisionTool!");
        return StatusCode::FAILURE;
    }

#ifdef ASGTOOL_STANDALONE
    Trig::TrigMuonMatching* tmm = new Trig::TrigMuonMatching("TrigMuonMatching");
    m_tmm = tmm;
#endif

    // retrieve TrigMuonMatching
    if(m_tmm.retrieve().isFailure())
    {
        ATH_MSG_ERROR("Failed to retrieve TrigMuonMatching!");
        return StatusCode::FAILURE;
    }

    // Return gracefully:
    return StatusCode::SUCCESS;
}

bool DV::TrigMatch::Match(const xAOD::Egamma& eg, const std::string& trigger) const
{
    // TrigEgammaMatchingTool matches electrons to egamma electrons for all triggers
    // For the photon triggers we have to match electrons to egamma photons
    // adapted from: TrigEgammaMatchingTool::matchHLT

    Trig::FeatureContainer fc = m_tdt->features(trigger);
    const auto vec = fc.get<xAOD::PhotonContainer>("egamma_Photons");

    for(const auto& feat: vec)
    {
        const xAOD::PhotonContainer* cont = feat.cptr();
        if(cont == nullptr) continue;

        for(const xAOD::Photon* ph: *cont)
        {
            if(ph == nullptr) continue;

            double deltaR = 0;
            if(eg.type() == xAOD::Type::Electron)
            {
                deltaR = xAOD::P4Helpers::deltaR(*ph, *(eg.caloCluster()), false);
            }
            else
            {
                deltaR = xAOD::P4Helpers::deltaR(*ph, eg, false);
            }

            // value taken from TrigEgammaMatchingTool
            if(deltaR < 0.07) return true;
        }
    }

    return false;
}


bool DV::TrigMatch::Match(const xAOD::Muon& mu, const std::string& trigger) const
{
    return m_tmm->match(&mu, trigger);
}
