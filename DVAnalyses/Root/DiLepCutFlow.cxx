#include "DVAnalyses/DiLepCutFlow.h"

DV::DiLepCutFlow::DiLepCutFlow()
{
}

void DV::DiLepCutFlow::bookHists(PlotsManagerTool* pm)
{
}

bool DV::DiLepCutFlow::initialize()
{
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
}
