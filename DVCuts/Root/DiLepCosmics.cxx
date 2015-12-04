#include "DVCuts/DiLepCosmics.h"

#include <cmath>

DV::DiLepCosmics::DiLepCosmics(const std::string& name) :
    asg::AsgTool(name), m_ec(""), m_mc("")
{
    declareProperty("RcosMax", m_Rcos = 0.04, "Cut for Rcos");
    declareProperty("deltaRMax", m_deltaR = 0.04, "Cut for deltaR");
}

StatusCode DV::DiLepCosmics::initialize()
{
    // Greet the user:
    ATH_MSG_DEBUG("Initialising... " );

    if(m_ec.retrieve().isFailure())
    {
        ATH_MSG_ERROR("Failed to retrieve ElectronCuts!");
        return StatusCode::FAILURE;
    }
    if(m_mc.retrieve().isFailure())
    {
        ATH_MSG_ERROR("Failed to retrieve MuonCuts!");
        return StatusCode::FAILURE;
    }

    // Return gracefully:
    return StatusCode::SUCCESS;
}

bool DV::DiLepCosmics::PassCosmicVeto(const TLorentzVector& lv1, const TLorentzVector& lv2) const
{
    // FIXME: pi
    double deltaPhiMinusPi = std::fabs(std::fabs(lv1.DeltaPhi(lv2)) - 3.1415926535897932384626433832795028841971693993);
    double sumEta = lv1.Eta() + lv2.Eta();
    if(std::sqrt(sumEta * sumEta + deltaPhiMinusPi * deltaPhiMinusPi) < m_Rcos) return false;

    // same side cosmics
    if(lv1.DeltaR(lv2) < m_deltaR) return false;

    return true;
}

bool DV::DiLepCosmics::PassCosmicEventVeto(const xAOD::ElectronContainer& elc, const xAOD::MuonContainer& muc) const
{
    for(auto mu1_itr = muc.cbegin(); mu1_itr != muc.cend(); mu1_itr++)
    {
        for(auto mu2_itr = mu1_itr+1; mu2_itr != muc.cend(); mu2_itr++)
        {
            if(!PassMuPairCosmicVeto(**mu1_itr, **mu2_itr)) return false;
        }
        for(auto el_itr = elc.cbegin(); el_itr != elc.cend(); el_itr++)
        {
            if(!PassElMuPairCosmicVeto(**el_itr, **mu1_itr)) return false;
        }
     }

    for(auto el1_itr = elc.cbegin(); el1_itr != elc.cend(); el1_itr++)
    {
        for(auto el2_itr = el1_itr+1; el2_itr != elc.cend(); el2_itr++)
        {
            if(!PassElPairCosmicVeto(**el1_itr, **el2_itr)) return false;
        }
    }

    return true;
}

bool DV::DiLepCosmics::PassMuPairCosmicVeto(const xAOD::Muon& mu1, const xAOD::Muon& mu2) const
{
    if(!m_mc->PassPtCut(mu1) || !m_mc->PassPtCut(mu2)) return true;

    return PassCosmicVeto(mu1.p4(), mu2.p4());
}

bool DV::DiLepCosmics::PassElMuPairCosmicVeto(const xAOD::Electron& el, const xAOD::Muon& mu) const
{
    if(!m_mc->PassPtCut(mu)) return true;
    if(!m_ec->PassPtCut(el)) return true;
    if(!m_ec->PassEtaCut(el)) return true;
    if(!m_ec->PassID(el)) return true;

    return PassCosmicVeto(el.p4(), mu.p4());
}

bool DV::DiLepCosmics::PassElPairCosmicVeto(const xAOD::Electron& el1, const xAOD::Electron& el2) const
{
    if(!m_ec->PassPtCut(el1) || !m_ec->PassPtCut(el2)) return true;
    if(!m_ec->PassEtaCut(el1) || !m_ec->PassEtaCut(el2)) return true;
    if(!m_ec->PassID(el1) || !m_ec->PassID(el2)) return true;

    return PassCosmicVeto(el1.p4(), el2.p4());
}
