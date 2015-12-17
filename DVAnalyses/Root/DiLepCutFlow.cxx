// To be used for RootCore ONLY
#ifdef ASGTOOL_STANDALONE

// ROOT
#include "TH1F.h"

// xAOD
#include "xAODCore/ShallowCopy.h"

// DV
#include "DVAnalyses/DiLepCutFlow.h"
#include "DVCuts/DVCuts.h"
#include "DVCuts/EventCuts.h"
#include "DVTools/PlotsManagerTool.h"

DV::DiLepCutFlow::DiLepCutFlow() :
    m_evtc("DV::EventCuts/DiLepEventCuts"),
    m_cos("DV::DiLepCosmics/DiLepCosmics"),
    m_dvc("DV::DVCuts/DiLepBaseCuts"),
    m_dilepdvc("DV::DiLepDVCuts/DiLepDVCuts"),
    m_or("DV::OverlapRemoval/OverlapRemoval"),
    m_phmatch("DV::PhotonMatch/PhotonMatch"),
    m_evt_hist(nullptr),
    m_vx_hist(nullptr)
{
    assignCutsAndTools();
}

void DV::DiLepCutFlow::bookHists(PlotsManagerTool* pm)
{
    pm->setCurrentModule("DiLep");

    m_evt_bins.push_back("Initial events");
    m_evt_bins.push_back("Event cleaning");
    m_evt_bins.push_back("GRL");
    m_evt_bins.push_back("Trigger");
    m_evt_bins.push_back("Cosmics veto");
    m_evt_bins.push_back("Primary vertex");

    std::size_t n_evt_bins = m_evt_bins.size();

    m_evt_hist = pm->bookTH1<TH1F>("evt_cuts", "", n_evt_bins, -0.5, n_evt_bins-0.5);
    for(std::size_t i = 0; i < n_evt_bins; i++)
    {
        m_evt_hist->GetXaxis()->SetBinLabel(i+1, m_evt_bins.at(i).c_str());
    }

    m_vx_bins.push_back("Reco DV");
    m_vx_bins.push_back("Fiducial acceptance");
    m_vx_bins.push_back("DV displacement");
    m_vx_bins.push_back("Fit quality");
    m_vx_bins.push_back("Material veto");
    m_vx_bins.push_back("Central track veto");
    m_vx_bins.push_back("Lepton matching");
    m_vx_bins.push_back("Lepton kinematics");
    m_vx_bins.push_back("Lepton ID");
    m_vx_bins.push_back("Overlap removal");
    m_vx_bins.push_back("Trigger matching");
    m_vx_bins.push_back("DESD offline cuts");
    m_vx_bins.push_back("DV mass");
    m_vx_bins.push_back("Opposite charge");

    std::size_t n_vx_bins = m_vx_bins.size();

    m_vx_hist = pm->bookTH1<TH1F>("vx_cuts", "", n_vx_bins, -0.5, n_vx_bins-0.5);
    for(std::size_t i = 0; i < n_vx_bins; i++)
    {
        m_vx_hist->GetXaxis()->SetBinLabel(i+1, m_vx_bins.at(i).c_str());
    }
}

bool DV::DiLepCutFlow::initialize()
{
    // configure EventCuts
    auto evtc = dynamic_cast<DV::EventCuts*>(&*m_evtc);

    std::string grl_file = "data15_13TeV.periodAllYear_DetStatus-v73-pro19-08_DQDefects-00-01-02_PHYS_StandardGRL_All_Good_25ns.xml";
    if(evtc->setProperty("GoodRunsListFile", grl_file).isFailure()) return false;

    evtc->SetTriggers({"HLT_g140_loose", "HLT_2g50_loose", "HLT_mu60_0eta105_msonly"});

    // configure DVCuts
    auto dvc = dynamic_cast<DV::DVCuts*>(&*m_dvc);

    if(dvc->setProperty("MaterialMapFile", "materialMap3D.root").isFailure()) return false;

    return true;
}

bool DV::DiLepCutFlow::execute(xAOD::TEvent* evt)
{
    m_evt_hist->Fill("Initial events", 1.);

    // retrieve event info
    const xAOD::EventInfo* ei = nullptr;
    if(evt->retrieve(ei, "EventInfo").isFailure())
    {
        Error("DiLepCutFlow::execute()", "Failed to retrieve EventInfo!");
        return false;
    }

    // event cleaning
    if(!m_evtc->PassEventCleaning(*ei)) return true;
    m_evt_hist->Fill("Event cleaning", 1.);

    // GRL
    if(!m_evtc->PassGRL(*ei)) return true;
    m_evt_hist->Fill("GRL", 1.);

    // trigger check
    if(!m_evtc->PassTrigger()) return true;
    m_evt_hist->Fill("Trigger", 1.);

    // retrieve leptons
    const xAOD::ElectronContainer* elc = nullptr;
    if(evt->retrieve(elc, "Electrons").isFailure())
    {
        Error("DiLepCutFlow::execute()", "Failed to retrieve Electrons!");
        return false;
    }
    const xAOD::MuonContainer* muc = nullptr;
    if(evt->retrieve(muc, "Muons").isFailure())
    {
        Error("DiLepCutFlow::execute()", "Failed to retrieve Muons!");
        return false;
    }

    // cosmics veto
    if(!m_cos->PassCosmicEventVeto(*elc, *muc)) return true;
    m_evt_hist->Fill("Cosmics veto", 1.);

    // retrieve PVs of event
    const xAOD::VertexContainer* pvc = nullptr;
    if(evt->retrieve(pvc, "PrimaryVertices").isFailure())
    {
        Error("DiLepCutFlow::execute()", "Failed to retrieve PrimaryVertices!");
        return false;
    }

    if(!m_evtc->PassPVCuts(*pvc)) return true;
    m_evt_hist->Fill("Primary vertex", 1.);

    // make copies of leptons
    auto elc_copy = xAOD::shallowCopyContainer(*elc);
    auto muc_copy = xAOD::shallowCopyContainer(*muc);

    // decorate overlap
    m_or->FindOverlap(*elc_copy.first, *muc_copy.first);

    // retrieve photons
    const xAOD::PhotonContainer* phc = nullptr;
    if(evt->retrieve(phc, "Photons").isFailure())
    {
        Error("DiLepCutFlow::execute()", "Failed to retrieve Photons!");
        return false;
    }

    // perform photon to electron matching
    m_phmatch->MatchPhotons(*phc, *elc_copy.first);

    // retrieve displaced vertices
    const xAOD::VertexContainer* dvc = nullptr;
    if(evt->retrieve(dvc, "RPVSecVertices").isFailure())
    {
        Error("DiLepCutFlow::execute()", "Failed to retrieve RPVSecVertices!");
        return false;
    }

    // make a copy of DVs
    auto dvc_copy = xAOD::shallowCopyContainer(*dvc);

    for(xAOD::Vertex* dv: *dvc_copy.first)
    {
        // perform lepton matching
        m_dilepdvc->ApplyLeptonMatching(*dv, *elc_copy.first, *muc_copy.first);

        // perform blinding
        if(m_dilepdvc->IsBlinded(*ei, *dv, *pvc)) continue;

        m_vx_hist->Fill("Reco DV", 1.);

        if(!m_dvc->PassFiducialCuts(*dv)) continue;
        m_vx_hist->Fill("Fiducial acceptance", 1.);

        if(!m_dvc->PassDistCut(*dv, *pvc)) continue;
        m_vx_hist->Fill("DV displacement", 1.);

        if(!m_dvc->PassChisqCut(*dv)) continue;
        m_vx_hist->Fill("Fit quality", 1.);

        if(!m_dvc->PassMaterialVeto(*dv)) continue;
        m_vx_hist->Fill("Material veto", 1.);

        if(!m_dilepdvc->PassCentralEtaVeto(*dv)) continue;
        m_vx_hist->Fill("Central track veto", 1.);

        if(!m_dilepdvc->PassNLeptons(*dv)) continue;
        m_vx_hist->Fill("Lepton matching", 1.);

        // kinematic cuts
        m_dilepdvc->ApplyKinematics(*dv);
        if(!m_dilepdvc->PassNLeptons(*dv)) continue;
        m_vx_hist->Fill("Lepton kinematics", 1.);

        // ID cuts
        m_dilepdvc->ApplyTightness(*dv);
        if(!m_dilepdvc->PassNLeptons(*dv)) continue;
        m_vx_hist->Fill("Lepton ID", 1.);

        // overlap removal
        m_dilepdvc->ApplyOverlapRemoval(*dv);
        if(!m_dilepdvc->PassNLeptons(*dv)) continue;
        m_vx_hist->Fill("Overlap removal", 1.);

        // trigger matching
        m_dilepdvc->DoTriggerMatching(*dv);
        if(!m_dilepdvc->PassTriggerMatching(*dv)) continue;
        m_vx_hist->Fill("Trigger matching", 1.);

        if(!m_dilepdvc->PassDESDMatching(*dv)) continue;
        m_vx_hist->Fill("DESD offline cuts", 1.);

        if(!m_dvc->PassMassCut(*dv)) continue;
        m_vx_hist->Fill("DV mass", 1.);

        if(!m_dilepdvc->PassChargeRequirement(*dv)) continue;
        m_vx_hist->Fill("Opposite charge", 1.);
    }

    // delete copies
    delete elc_copy.first;
    delete elc_copy.second;
    delete muc_copy.first;
    delete muc_copy.second;
    delete dvc_copy.first;
    delete dvc_copy.second;

    return true;
}

bool DV::DiLepCutFlow::finalize()
{
    return true;
}

void DV::DiLepCutFlow::assignCutsAndTools()
{
    // add analysis specific DV tools
    m_toolnames.push_back("DV::DiLepCosmics/DiLepCosmics");
    m_toolnames.push_back("DV::DiLepDESD/DiLepDESD");
    m_toolnames.push_back("DV::DiLepDVCuts/DiLepDVCuts");
    m_toolnames.push_back("DV::DVCuts/DiLepBaseCuts");
    m_toolnames.push_back("DV::ElecCuts/DiLepElecCuts");
    m_toolnames.push_back("DV::EventCuts/DiLepEventCuts");
    m_toolnames.push_back("DV::MuonCuts/DiLepMuonCuts");

    // add generic DV tools which can be shared between analyses
    m_toolnames.push_back("DV::OverlapRemoval/OverlapRemoval");
    m_toolnames.push_back("DV::PhotonMatch/PhotonMatch");
    m_toolnames.push_back("DV::TrigMatch/TrigMatch");
}

#endif

