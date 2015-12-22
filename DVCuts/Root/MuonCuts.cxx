#include "DVCuts/MuonCuts.h"

#ifdef ASGTOOL_ATHENA
#include "GaudiKernel/IJobOptionsSvc.h"
#endif

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

#ifdef ASGTOOL_ATHENA
    if(m_mst_init == false)
    {
        ServiceHandle<IJobOptionsSvc> josvc("JobOptionsSvc", name());

        // configure muon identification
        ATH_CHECK(josvc->addPropertyToCatalogue("ToolSvc.DVMuonSelectionTool",
                                            IntegerProperty("MuQuality", static_cast<int>(xAOD::Muon::Loose))));
        // turn off cuts on si hits
        ATH_CHECK(josvc->addPropertyToCatalogue("ToolSvc.DVMuonSelectionTool",
                                                BooleanProperty("PixCutOff", true)));
        ATH_CHECK(josvc->addPropertyToCatalogue("ToolSvc.DVMuonSelectionTool",
                                                BooleanProperty("SiHolesCutOff", true)));
    }
#endif

    // retrieve MuonSelectionTool
    if(m_mst.retrieve().isFailure())
    {
        ATH_MSG_ERROR("Could not retrieve MuonSelectionTool!");
       return StatusCode::FAILURE;
    }

    if(m_mst_init == false)
    {
        auto mst = dynamic_cast<asg::AsgTool*>(&*m_mst);

#ifdef ASGTOOL_STANDALONE
        // configure muon identification
        ATH_CHECK(mst->setProperty("MuQuality", static_cast<int>(xAOD::Muon::Loose)));
        // turn off cuts on si hits
        ATH_CHECK(mst->setProperty("PixCutOff", true));
        ATH_CHECK(mst->setProperty("SiHolesCutOff", true));

        ATH_CHECK(m_mst->initialize());
#endif

        // silence MuonSelectionTool
        mst->msg().setLevel(MSG::ERROR);

        m_mst_init = true;
    }

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
