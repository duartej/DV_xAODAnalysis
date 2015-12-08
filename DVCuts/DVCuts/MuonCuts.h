#ifndef DV_MUONCUTS_H
#define DV_MUONCUTS_H

// STL
#include <string>

// Tools
#include "AsgTools/AsgTool.h"
#include "AsgTools/ToolHandle.h"
#include "MuonSelectorTools/MuonSelectionTool.h"

// xAOD
#include "xAODMuon/Muon.h"
#include "xAODTracking/TrackParticle.h"

// DVCuts
#include "DVCuts/IMuonCuts.h"

namespace DV
{
    class MuonCuts : virtual public IMuonCuts, public asg::AsgTool
    {
        ASG_TOOL_CLASS(MuonCuts, DV::IMuonCuts)

        public:
            MuonCuts(const std::string& name);
            virtual ~MuonCuts() = default;

            // retrieves Tools
            StatusCode initialize() override;

            const xAOD::TrackParticle* GetTrack(const xAOD::Muon& mu) const override;

            bool PassPtCut(const xAOD::Muon& mu) const override;
            bool PassEtaCut(const xAOD::Muon& mu) const override;
            bool PassD0Cut(const xAOD::Muon& mu) const override;
            bool PassID(const xAOD::Muon& mu) const override;

            bool IsPreRetracking(const xAOD::Muon& mu) const override;

        private:
            // muon identification tool
            ToolHandle<CP::IMuonSelectionTool> m_mst;

            double m_ptMin;
            double m_etaMax;
            double m_d0Min;
    };
}

#endif
