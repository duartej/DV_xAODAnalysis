#ifndef DV_DILEPSIGNAL_H
#define DV_DILEPSIGNAL_H

// For RootCore
#ifdef ASGTOOL_STANDALONE

// STL
#include <map>
#include <string>
#include <vector>

// Tools
#include "AsgTools/ToolHandle.h"

// DV
#include "DVAnalyses/AlgBase.h"
#include "DVCuts/IDiLepDESD.h"

// forward declaration
class TH1D;
class TH2D;

namespace DV
{
    class DiLepSignal : public AlgBase
    {
        public:
            DiLepSignal();
            virtual ~DiLepSignal() = default;

            void bookHists(PlotsManagerTool* pm) override;
            bool initialize() override;
            bool execute(xAOD::TEvent* evt) override;
            bool finalize() override;
        private:
            void assignCutsAndTools() override;

            ToolHandle<DV::IDiLepDESD> m_filters; //!

            std::vector<std::string> m_vx_types;
            std::vector<std::string> m_filt_names;

            int m_bins;
            std::vector<double> m_bin_edges;

            std::map<std::string, TH1D*> m_hists1D;
            std::map<std::string, TH2D*> m_hists2D;

            //ClassDef(DiLepSignal, 1);
    };
}

#endif // ASGTOOL_STANDALONE

#endif
