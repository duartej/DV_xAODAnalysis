// To be used for RootCore ONLY
#ifdef ASGTOOL_STANDALONE

#include "DVAnalyses/DiLepCutFlow.h"

#include "DVCuts/EventCuts.h"

DV::DiLepCutFlow::DiLepCutFlow() :
    m_evtc("DV::EventCuts/EventCuts"),
    m_cos("DV::DiLepCosmics/DiLepCosmics"),
    m_dvc("DV::DVCuts/DVCuts"),
    m_dilepdvc("DV::DiLepDVCuts/DiLepDVCuts")
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
    // retrieve event info
    const xAOD::EventInfo* ei = nullptr;
    if(evt->retrieve(ei, "EventInfo").isFailure())
    {
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
        return false;
    }
    const xAOD::MuonContainer* muc = nullptr;
    if(evt->retrieve(muc, "Muons").isFailure())
    {
        return false;
    }

    // cosmic veto
    if(!m_cos->PassCosmicEventVeto(*elc, *muc)) return true;

    // retrieve PVs of event
    const xAOD::VertexContainer* pvc = nullptr;
    if(evt->retrieve(pvc, "PrimaryVertices").isFailure())
    {
        return false;
    }

    if(!m_evtc->PassPVCuts(*pvc)) return true;

    std::cout << "EVENT PASSED ALL EVENT CUTS :)" << std::endl;

    return true;
}

bool DV::DiLepCutFlow::finalize()
{
    return true;
}

void DV::DiLepCutFlow::assignCutsAndTools()
{
    m_cutnames.push_back("DV::DiLepCosmics/DiLepCosmics");
    m_cutnames.push_back("DV::DiLepDESD/DiLepDESD");
    m_cutnames.push_back("DV::DiLepDVCuts/DiLepDVCuts");
    m_cutnames.push_back("DV::DVCuts/DVCuts");
    m_cutnames.push_back("DV::ElecCuts/DiLepElecCuts");
    m_cutnames.push_back("DV::EventCuts/EventCuts");
    m_cutnames.push_back("DV::MuonCuts/DiLepMuonCuts");
    m_cutnames.push_back("DV::OverlapRemoval/OverlapRemoval");
    m_cutnames.push_back("DV::PhotonMatch/PhotonMatch");
    m_cutnames.push_back("DV::TrigMatch/TrigMatch");
}

#endif

