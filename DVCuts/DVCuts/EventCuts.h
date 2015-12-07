#ifndef DV_EVENTCUTS_H
#define DV_EVENTCUTS_H

// STL
#include <string>
#include <vector>

// Tools
#include "AsgTools/AsgTool.h"
#include "AsgTools/ToolHandle.h"
#include "TrigDecisionTool/TrigDecisionTool.h"

#ifdef ASGTOOL_ATHENA
#include "GoodRunsLists/IGoodRunsListSelectionTool.h"
#elif defined(ASGTOOL_STANDALONE)
#include "GoodRunsLists/TGoodRunsList.h"
#include "GoodRunsLists/TGoodRunsListReader.h"
#else
#  error "What environment are we in?!?"
#endif // Include headers depending environment

// xAOD
#include "xAODEventInfo/EventInfo.h"
#include "xAODTracking/VertexContainer.h"

// DVCuts
#include "DVCuts/IEventCuts.h"

namespace DV
{
    class EventCuts : virtual public IEventCuts, public asg::AsgTool
    {
        ASG_TOOL_CLASS(EventCuts, DV::IEventCuts)

        public:
            EventCuts(const std::string& name);
            virtual ~EventCuts();

            // retrieves Tools
            StatusCode initialize() override;

            // defines trigger names
            void SetTriggers(const std::vector<std::string>& trigNames) override;

            // retrieves PV from container
            const xAOD::Vertex* GetPV(const xAOD::VertexContainer& pvc) const override;

            // Cuts
            bool PassGRL(const xAOD::EventInfo& ei) const override;
            bool PassEventCleaning(const xAOD::EventInfo& ei) const override;
            bool PassTrigger() const override;
            bool PassPVCuts(const xAOD::VertexContainer& pvc) const override;

        private:
            ToolHandle<Trig::TrigDecisionTool> m_tdt;

#ifdef ASGTOOL_ATHENA
            ToolHandle<IGoodRunsListSelectionTool> m_grl;
#elif defined(ASGTOOL_STANDALONE)
            Root::TGoodRunsList m_grl;
            Root::TGoodRunsListReader* m_grlR;

            std::string m_grlFile;
#endif // ASGTOOL_STANDALONE

            bool m_useGRL;
            bool m_checkTrig;

            // stores trigger names
            std::vector<std::string> m_trigNames;
    };
}

#endif
