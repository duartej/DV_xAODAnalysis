// To be used for RootCore ONLY
#ifdef ASGTOOL_STANDALONE

#include "xAODCore/ShallowCopy.h"

#include "DVAnalyses/DiLepCutFlow.h"

#include "DVCuts/EventCuts.h"

DV::DiLepCutFlow::DiLepCutFlow() :
    m_evtc("DV::EventCuts/EventCuts"),
    m_cos("DV::DiLepCosmics/DiLepCosmics"),
    m_dvc("DV::DVCuts/DVCuts"),
    m_dilepdvc("DV::DiLepDVCuts/DiLepDVCuts"),
    m_or("DV::OverlapRemoval/OverlapRemoval"),
    m_phmatch("DV::PhotonMatch/PhotonMatch"),
    m_nevt_passed(0),
    m_nvx(0),
    m_nvx_passed(0)
{
    assignCutsAndTools();
}

void DV::DiLepCutFlow::bookHists(PlotsManagerTool* pm)
{
}

bool DV::DiLepCutFlow::initialize()
{
    // configure EventCuts
    DV::EventCuts* evtc = dynamic_cast<DV::EventCuts*>(&*m_evtc);

    std::string grl_file = "data15_13TeV.periodAllYear_DetStatus-v73-pro19-08_DQDefects-00-01-02_PHYS_StandardGRL_All_Good_25ns.xml";
    if(evtc->setProperty("GoodRunsListFile", grl_file).isFailure()) return false;

    evtc->SetTriggers({"HLT_g140_loose", "HLT_2g50_loose", "HLT_mu60_0eta105_msonly"});

    return true;
}

bool DV::DiLepCutFlow::execute(xAOD::TEvent* evt)
{
    m_eventCounter++;

    // retrieve event info
    const xAOD::EventInfo* ei = nullptr;
    if(evt->retrieve(ei, "EventInfo").isFailure())
    {
        Error("DiLepCutFlow::execute()", "Failed to retrieve EventInfo!");
        return false;
    }

    // GRL
    if(!m_evtc->PassGRL(*ei)) return true;

    // event cleaning
    if(!m_evtc->PassEventCleaning(*ei)) return true;

    // trigger check
    if(!m_evtc->PassTrigger()) return true;

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

    // cosmic veto
    if(!m_cos->PassCosmicEventVeto(*elc, *muc)) return true;

    // retrieve PVs of event
    const xAOD::VertexContainer* pvc = nullptr;
    if(evt->retrieve(pvc, "PrimaryVertices").isFailure())
    {
        Error("DiLepCutFlow::execute()", "Failed to retrieve PrimaryVertices!");
        return false;
    }

    if(!m_evtc->PassPVCuts(*pvc)) return true;

    m_nevt_passed++;

    // make copies of leptons
    auto elc_copy = xAOD::shallowCopyContainer(*elc);
    auto muc_copy = xAOD::shallowCopyContainer(*muc);

    // decorate overlap
    m_or->RemoveOverlap(*elc_copy.first, *muc_copy.first);

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
        m_nvx++;

        // FIXME: better blinding :)
        if(!ei->eventType(xAOD::EventInfo::IS_SIMULATION)) continue;

        if(!m_dvc->PassFiducialCuts(*dv)) continue;

        if(!m_dvc->PassDistCut(*dv, *pvc)) continue;

        if(!m_dvc->PassChisqCut(*dv)) continue;

        if(!m_dvc->PassMaterialVeto(*dv)) continue;

        if(!m_dilepdvc->PassCentralEtaVeto(*dv)) continue;

        // perform lepton matching
        m_dilepdvc->ApplyLeptonMatching(*dv, *elc_copy.first, *muc_copy.first);
        if(!m_dilepdvc->PassNLeptons(*dv)) continue;

        // kinematic cuts
        m_dilepdvc->ApplyKinematics(*dv);
        if(!m_dilepdvc->PassNLeptons(*dv)) continue;

        // ID cuts
        m_dilepdvc->ApplyTightness(*dv);
        if(!m_dilepdvc->PassNLeptons(*dv)) continue;

        // overlap removal
        m_dilepdvc->ApplyOverlapRemoval(*dv);
        if(!m_dilepdvc->PassNLeptons(*dv)) continue;

        // trigger matching
        m_dilepdvc->DoTriggerMatching(*dv);
        if(!m_dilepdvc->PassTriggerMatching(*dv)) continue;

        if(!m_dilepdvc->PassDESDMatching(*dv)) continue;

        if(!m_dvc->PassMassCut(*dv)) continue;

        if(!m_dilepdvc->PassChargeRequirement(*dv)) continue;

        m_nvx_passed++;
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
    std::cout << "Events passed cutflow: " << m_nevt_passed << " / " << m_eventCounter << std::endl;
    std::cout << "DVs passed cutflow: " << m_nvx_passed << " / " << m_nvx << std::endl;

    return true;
}

void DV::DiLepCutFlow::assignCutsAndTools()
{
    m_toolnames.push_back("DV::DiLepCosmics/DiLepCosmics");
    m_toolnames.push_back("DV::DiLepDESD/DiLepDESD");
    m_toolnames.push_back("DV::DiLepDVCuts/DiLepDVCuts");
    m_toolnames.push_back("DV::DVCuts/DVCuts");
    m_toolnames.push_back("DV::ElecCuts/DiLepElecCuts");
    m_toolnames.push_back("DV::EventCuts/EventCuts");
    m_toolnames.push_back("DV::MuonCuts/DiLepMuonCuts");
    m_toolnames.push_back("DV::OverlapRemoval/OverlapRemoval");
    m_toolnames.push_back("DV::PhotonMatch/PhotonMatch");
    m_toolnames.push_back("DV::TrigMatch/TrigMatch");
}

#endif

