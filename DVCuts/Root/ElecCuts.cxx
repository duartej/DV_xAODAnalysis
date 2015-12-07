#include "DVCuts/ElecCuts.h"

#include <cmath>

DV::ElecCuts::ElecCuts(const std::string& name) :
    asg::AsgTool(name),
    m_elt("AsgElectronLikelihoodTool/DVElectronLikelihoodTool")
{
    declareProperty("ptMin", m_ptMin = 10000., "Cut for electron track pt [MeV]");
    declareProperty("etaMax", m_etaMax = 2.5, "Cut for electron track |eta|");
    declareProperty("d0Min", m_d0Min = 2.0, "Cut for electron track |d0|");
    declareProperty("elecID", m_elecID = "Loose", "Working point for electron likelihood identification");
}

StatusCode DV::ElecCuts::initialize()
{
    // Greet the user:
    ATH_MSG_DEBUG("Initialising... " );

#ifdef ASGTOOL_STANDALONE
    AsgElectronLikelihoodTool* elt = new AsgElectronLikelihoodTool("DVElectronLikelihoodTool");
#elif defined(ASGTOOL_ATHENA)
    AsgElectronLikelihoodTool* elt = dynamic_cast<AsgElectronLikelihoodTool*>(&*m_elt);
#endif

    // set working point of electron ID
    std::string config_file = "ElectronPhotonSelectorTools/offline/mc15_20150712/ElectronLikelihood" + m_elecID + "NoD0OfflineConfig2015.conf";
    ATH_CHECK(elt->setProperty("ConfigFile", config_file));

    // turn off cuts on si hits for electron ID
    ATH_CHECK(elt->setProperty("CutBL", std::vector<int>()));
    ATH_CHECK(elt->setProperty("CutPi", std::vector<int>()));
    ATH_CHECK(elt->setProperty("CutSi", std::vector<int>()));

#ifdef ASGTOOL_STANDALONE
    m_elt = elt;
#endif

    // Return gracefully:
    return StatusCode::SUCCESS;
}

const xAOD::TrackParticle* DV::ElecCuts::GetTrack(const xAOD::Electron& el) const
{
    const xAOD::TrackParticle* el_tr = el.trackParticle();
    if(el_tr == nullptr)
    {
        ATH_MSG_WARNING("Failed to retrieve electron track!");
    }

    return el_tr;
}

bool DV::ElecCuts::PassPtCut(const xAOD::Electron& el) const
{
    const xAOD::TrackParticle* el_tr = GetTrack(el);
    if(el_tr)
    {
        if(el_tr->pt() > m_ptMin) return true;
    }

    return false;
}

bool DV::ElecCuts::PassEtaCut(const xAOD::Electron& el) const
{
    const xAOD::TrackParticle* el_tr = GetTrack(el);
    if(el_tr)
    {
        if(std::fabs(el_tr->eta()) < m_etaMax) return true;
    }

    return false;
}

bool DV::ElecCuts::PassD0Cut(const xAOD::Electron& el) const
{
    const xAOD::TrackParticle* el_tr = GetTrack(el);
    if(el_tr)
    {
        if(std::fabs(el_tr->d0()) > m_d0Min) return true;
    }

    return false;
}

bool DV::ElecCuts::PassID(const xAOD::Electron& el) const
{
    return m_elt->accept(el);
}

bool DV::ElecCuts::IsPreRetracking(const xAOD::Electron& el) const
{
    // FIXME: find some clever way to do it
    return false;
}
