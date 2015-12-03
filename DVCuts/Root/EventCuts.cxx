#include "DVCuts/EventCuts.h"

#include <cmath>

DV::EventCuts::EventCuts(const std::string& name)
#ifdef ASGTOOL_ATHENA
    : asg::AsgTool(name), m_grl(""), m_tdt(""), m_useGRL(true), m_checkTrig(true)
#elif defined(ASGTOOL_STANDALONE)
    : asg::AsgTool(name), m_grlR(nullptr), m_tdt(""), m_useGRL(true), m_checkTrig(true)
#endif // ASGTOOL_STANDALONE
{
#ifdef ASGTOOL_ATHENA
    declareProperty("GoodRunsListSelectionTool", m_grl);
#elif defined(ASGTOOL_STANDALONE)
    declareProperty("GoodRunsListFile", m_grlFile);
#endif // ASGTOOL_STANDALONE

    declareProperty("useGRL", m_useGRL);
    declareProperty("checkTriggers", m_checkTrig);

    declareProperty("TrigDecisionTool", m_tdt);
}

DV::EventCuts::~EventCuts()
{
#ifdef ASGTOOL_STANDALONE
    delete m_grlR;
#endif // ASGTOOL_STANDALONE
}

StatusCode DV::EventCuts::initialize()
{
    if(m_useGRL)
    {
#ifdef ASGTOOL_ATHENA
        if(m_grl.retrieve().isFailure())
        {
            ATH_MSG_ERROR("Failed to retrieve GoodRunsListSelectionTool!");
            return StatusCode::FAILURE;
        }
#elif defined(ASGTOOL_STANDALONE)
        if(m_grlFile.empty())
        {
            ATH_MSG_ERROR("No GRL file defined!");
            return StatusCode::FAILURE;
        }

        m_grlR = new Root::TGoodRunsListReader();
        m_grlR->SetXMLFile(m_grlFile.c_str());
        m_grlR->Interpret();

        m_grl = m_grlR->GetMergedGoodRunsList();
        m_grl.Summary(false);
#endif // ASGTOOL_STANDALONE
    }
    else
    {
      ATH_MSG_WARNING("GRL is not used!");
    }

    if(m_checkTrig)
    {
        if(m_tdt.retrieve().isFailure())
        {
            ATH_MSG_ERROR("Failed to retrieve TrigDecisionTool!");
            return StatusCode::FAILURE;
        }
    }
    else
    {
        ATH_MSG_WARNING("Triggers are not checked!");
    }

    return StatusCode::SUCCESS;
}

void DV::EventCuts::SetTriggers(const std::vector<std::string>& trigNames)
{
    m_trigNames = trigNames;
}

const xAOD::Vertex* DV::EventCuts::GetPV(const xAOD::VertexContainer& pvc) const
{
    for(const xAOD::Vertex* pv: pvc)
    {
        if(pv->vertexType() == xAOD::VxType::PriVtx)
        {
            return pv;
        }
    }

    return nullptr;
}

bool DV::EventCuts::PassGRL(const xAOD::EventInfo& ei) const
{
    // GRL is only applied to data
    if(m_useGRL && !ei.eventType(xAOD::EventInfo::IS_SIMULATION))
    {
#ifdef ASGTOOL_ATHENA
        if(!m_grl->passRunLB(ei)) return false;
#elif defined(ASGTOOL_STANDALONE)
        if(!m_grl.HasRunLumiBlock(ei.runNumber(), ei.lumiBlock())) return false;
#endif // ASGTOOL_STANDALONE
    }

    return true;
}

bool DV::EventCuts::PassEventCleaning(const xAOD::EventInfo& ei) const
{
    if(ei.errorState(xAOD::EventInfo::LAr) == xAOD::EventInfo::Error) return false;
    if(ei.errorState(xAOD::EventInfo::Tile) == xAOD::EventInfo::Error) return false;
    if(ei.errorState(xAOD::EventInfo::SCT) == xAOD::EventInfo::Error) return false;
    if(ei.isEventFlagBitSet(xAOD::EventInfo::Core, 18)) return false; // TTC veto

    return true;
}

bool DV::EventCuts::PassTrigger() const
{
    if(!m_checkTrig)
    {
        return true;
    }
    if(m_trigNames.empty())
    {
        ATH_MSG_ERROR("No trigger names were defined!");
        return false;
    }

    for(const std::string& tn: m_trigNames)
    {
        if(m_tdt->isPassed(tn)) return true;
    }
    return false;
}

bool DV::EventCuts::PassPVCuts(const xAOD::VertexContainer& pvc) const
{
    const xAOD::Vertex* pv = GetPV(pvc);

    if(pv == nullptr) return false;
    if(std::fabs(pv->z()) > 200.) return false;

    return true;
}