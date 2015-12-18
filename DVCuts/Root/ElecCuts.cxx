#include "DVCuts/ElecCuts.h"

#ifdef ASGTOOL_ATHENA
#include "GaudiKernel/IJobOptionsSvc.h"
#endif

#include <cmath>

DV::ElecCuts::ElecCuts(const std::string& name) :
    asg::AsgTool(name),
    m_elt("AsgElectronLikelihoodTool/DVElectronLikelihoodTool")
{
    declareProperty("ptMin", m_ptMin = 10000., "Cut for electron track pt [MeV]");
    declareProperty("etaMax", m_etaMax = 2.5, "Cut for electron track |eta|");
    declareProperty("d0Min", m_d0Min = 2.0, "Cut for electron track |d0|");
}

StatusCode DV::ElecCuts::initialize()
{
    // Greet the user:
    ATH_MSG_DEBUG("Initialising... " );

    // working point of electron ID
    std::string config_file = "ElectronPhotonSelectorTools/offline/mc15_20150712/ElectronLikelihoodLooseNoD0OfflineConfig2015.conf";

#ifdef ASGTOOL_ATHENA
    ServiceHandle<IJobOptionsSvc> josvc("JobOptionsSvc", name());

    ATH_CHECK(josvc->addPropertyToCatalogue("ToolSvc.DVElectronLikelihoodTool",
                                            StringProperty("ConfigFile", config_file)));
#endif

    // retrieve AsgElectronLikelihoodTool
    if(m_elt.retrieve().isFailure())
    {
      ATH_MSG_ERROR("Could not retrieve AsgElectronLikelihoodTool!");
      return StatusCode::FAILURE;
    }

    auto elt = dynamic_cast<AsgElectronLikelihoodTool*>(&*m_elt);

#ifdef ASGTOOL_STANDALONE
    ATH_CHECK(elt->setProperty("ConfigFile", config_file));

    ATH_CHECK(elt->initialize());
#endif

    // turn off cuts on si hits for electron ID (has to be done after initialization)
    ATH_CHECK(elt->setProperty("CutBL", std::vector<int>()));
    ATH_CHECK(elt->setProperty("CutPi", std::vector<int>()));
    ATH_CHECK(elt->setProperty("CutSi", std::vector<int>()));

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

bool DV::ElecCuts::PassKinematics(const xAOD::Electron& el) const
{
    if(!PassPtCut(el)) return false;
    if(!PassEtaCut(el)) return false;

    return true;
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
    /*const xAOD::TrackParticle* el_tr = GetTrack(el);
    if(el_tr)
    {
        auto tr_pattern = el_tr->patternRecoInfo();
        if(tr_pattern.test(xAOD::SiSpacePointsSeedMaker_LargeD0)) return false;
    }*/

    return true;
}
