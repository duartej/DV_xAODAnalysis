#include "DVCuts/MuonCuts.h"

#include <cmath>

bool DV::MuonCuts::m_mst_init = false;

DV::MuonCuts::MuonCuts(const std::string& name) :
    asg::AsgTool(name),
    m_mst("CP::IMuonSelectionTool/DVMuonSelectionTool")
{
    declareProperty("ptMin", m_ptMin = 10000., "Cut for muon track pt [MeV]");
    declareProperty("etaMax", m_etaMax = 2.5, "Cut for muon track |eta|");
    declareProperty("d0Min", m_d0Min = 2.0, "Cut for muon track |d0| [mm]");
}

StatusCode DV::MuonCuts::initialize()
{
    // Greet the user:
    ATH_MSG_DEBUG("Initialising... " );

    // check for MuonSelectionTool
    if(!asg::ToolStore::contains<CP::MuonSelectionTool>(m_mst.name()))
    {
      ATH_MSG_ERROR("Could not find: " + m_mst.name());
      return StatusCode::FAILURE;
    }

    // get pointer to MuonSelectionTool
    auto mst = asg::ToolStore::get<CP::MuonSelectionTool>(m_mst.name());

    // do configuration only once
    if(!m_mst_init)
    {
        // set working point of muon ID
        ATH_CHECK(mst->setProperty("MuQuality", static_cast<int>(xAOD::Muon::Loose)));
        // turn off cuts on si hits
        ATH_CHECK(mst->setProperty("PixCutOff", true));
        ATH_CHECK(mst->setProperty("SiHolesCutOff", true));

        // initialize tool
        ATH_CHECK(mst->initialize());

        // silence MuonSelectionTool
        mst->msg().setLevel(MSG::ERROR);

        m_mst_init = true;
    }

    m_mst = mst;

    // Return gracefully:
    return StatusCode::SUCCESS;
}

const xAOD::TrackParticle* DV::MuonCuts::GetTrack(const xAOD::Muon& mu) const
{
    const xAOD::TrackParticle* mu_tr = mu.trackParticle(xAOD::Muon::InnerDetectorTrackParticle);
    if(mu_tr == nullptr && mu.muonType() == xAOD::Muon::Combined)
    {
        ATH_MSG_WARNING("Failed to retrieve ID track of combined muon!");
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

bool DV::MuonCuts::PassKinematics(const xAOD::Muon& mu) const
{
    if(!PassPtCut(mu)) return false;
    if(!PassEtaCut(mu)) return false;

    return true;
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
    if(mu.muonType() != xAOD::Muon::Combined) return false;

    return m_mst->accept(mu);
}

bool DV::MuonCuts::IsPreRetracking(const xAOD::Muon& mu) const
{
    /*const xAOD::TrackParticle* mu_tr = GetTrack(mu);
    if(mu_tr)
    {
        auto tr_pattern = mu_tr->patternRecoInfo();
        if(tr_pattern.test(xAOD::SiSpacePointsSeedMaker_LargeD0)) return false;
    }*/

    return true;
}
