#ifndef DV_DILEPCUTFLOW_H
#define DV_DILEPCUTFLOW_H

// For RootCore
#ifdef ASGTOOL_STANDALONE

// STL
#include <cstdint>

// Tools
#include "AsgTools/ToolHandle.h"

// DV
#include "DVAnalyses/AlgBase.h"
#include "DVCuts/IDVCuts.h"
#include "DVCuts/IEventCuts.h"
#include "DVCuts/IDiLepCosmics.h"
#include "DVCuts/IDiLepDVCuts.h"

namespace DV
{
    class DiLepCutFlow : public AlgBase
    {
        public:
            DiLepCutFlow();
            virtual ~DiLepCutFlow() = default;

            void bookHists(PlotsManagerTool* pm) override;
            bool initialize() override;
            bool execute(xAOD::TEvent* evt) override;
            bool finalize() override;
        private:
            void assignCutsAndTools() override;

            ToolHandle<DV::IEventCuts> m_evtc; //!
            ToolHandle<DV::IDiLepCosmics> m_cos; //!
            ToolHandle<DV::IDVCuts> m_dvc; //!
            ToolHandle<DV::IDiLepDVCuts> m_dilepdvc; //!

            std::uint64_t m_cutflow_passed;

            //ClassDef(DiLepCutFlow, 1);
    };
}

#endif // ASGTOOL_STANDALONE

#endif
