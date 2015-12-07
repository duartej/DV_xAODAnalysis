#include "DVCuts/MuonCuts.h"

#include <cmath>

DV::MuonCuts::MuonCuts(const std::string& name) :
    asg::AsgTool(name),
    m_mst("CP::IMuonSelectionTool/DVMuonSelectionTool")
{
    declareProperty("ptMin", m_ptMin = 10000., "Cut for muon track pt [MeV]");
    declareProperty("etaMax", m_etaMax = 2.5, "Cut for muon track |eta|");
    declareProperty("d0Min", m_d0Min = 2.0, "Cut for muon track |d0|");
    declareProperty("muID", m_muID = xAOD::Muon::VeryLoose, "Working point for muon identification");
}

StatusCode DV::MuonCuts::initialize()
{
    // Greet the user:
    ATH_MSG_DEBUG("Initialising... " );

#ifdef ASGTOOL_STANDALONE
    CP::MuonSelectionTool* mst = new CP::MuonSelectionTool("DVMuonSelectionTool");
#elif defined(ASGTOOL_ATHENA)
    CP::MuonSelectionTool *mst = dynamic_cast<CP::MuonSelectionTool*>(&*m_mst);
#endif

    // configure muon identification
    ATH_CHECK(mst->setProperty("MuQuality", m_muID));
    // turn off cuts on si hits
    ATH_CHECK(mst->setProperty("PixCutOff", true));
    ATH_CHECK(mst->setProperty("SiHolesCutOff", true));

#ifdef ASGTOOL_STANDALONE
    m_mst = mst;
#endif

    // Return gracefully:
    return StatusCode::SUCCESS;
}

const xAOD::TrackParticle* DV::MuonCuts::GetTrack(const xAOD::Muon& mu) const
{
    const xAOD::TrackParticle* mu_tr = mu.trackParticle(xAOD::Muon::InnerDetectorTrackParticle);
    if(mu_tr == nullptr)
    {
        ATH_MSG_WARNING("Failed to retrieve Muon track!");
    }

    return mu_tr;
}

bool DV::MuonCuts::PassPtCut(const xAOD::Muon& mu) const
{
    const xAOD::TrackParticle* mu_tr = GetTrack(mu);
    if(mu_tr)
    {
        if(mu_tr->pt() > m_ptMin) return true;
    }

    return false;
}

bool DV::MuonCuts::PassEtaCut(const xAOD::Muon& mu) const
{
    const xAOD::TrackParticle* mu_tr = GetTrack(mu);
    if(mu_tr)
    {
        if(std::fabs(mu_tr->eta()) < m_etaMax) return true;
    }

    return false;
}

bool DV::MuonCuts::PassD0Cut(const xAOD::Muon& mu) const
{
    const xAOD::TrackParticle* mu_tr = GetTrack(mu);
    if(mu_tr)
    {
        if(std::fabs(mu_tr->d0()) > m_d0Min) return true;
    }

    return false;
}

bool DV::MuonCuts::PassID(const xAOD::Muon& mu) const
{
    return m_mst->accept(mu);
}

bool DV::MuonCuts::IsPreRetracking(const xAOD::Muon& mu) const
{
    // FIXME: find some clever way to do it
    return false;
}
