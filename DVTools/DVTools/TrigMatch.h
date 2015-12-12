#ifndef DV_TRIGMATCH_H
#define DV_TRIGMATCH_H

// STL
#include <string>

// Tools
#include "AsgTools/AsgTool.h"
#include "AsgTools/ToolHandle.h"
#include "TrigDecisionTool/TrigDecisionTool.h"
#include "TrigMuonMatching/TrigMuonMatching.h"

// xAOD
#include "xAODEgamma/Egamma.h"
#include "xAODMuon/Muon.h"

// DVCuts
#include "DVTools/ITrigMatch.h"

namespace DV
{
    class TrigMatch : virtual public ITrigMatch, public asg::AsgTool
    {
        ASG_TOOL_CLASS(TrigMatch, DV::ITrigMatch)
        public:
            TrigMatch(const std::string& name);
            virtual ~TrigMatch() = default;

            StatusCode initialize() override;

            // This method matches egamma particles to photon triggers
            bool Match(const xAOD::Egamma& eg, const std::string& trigger) const override;

            bool Match(const xAOD::Muon& mu, const std::string& trigger) const override;

        private:
            ToolHandle<Trig::TrigDecisionTool> m_tdt;
            ToolHandle<Trig::TrigMuonMatching> m_tmm;
    };
}

#endif
