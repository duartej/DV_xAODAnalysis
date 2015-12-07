#ifndef DV_DILEPDVCUTS_H
#define DV_DILEPDVCUTS_H

// STL
#include <string>

// Tools
#include "AsgTools/AsgTool.h"
#include "AsgTools/ToolHandle.h"

// xAOD
#include "xAODEgamma/ElectronContainer.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODTracking/Vertex.h"

// DV
#include "DVCuts/IDiLepDESD.h"
#include "DVCuts/IDiLepDVCuts.h"
#include "DVCuts/IElecCuts.h"
#include "DVCuts/IMuonCuts.h"
#include "DVTools/IOverlapRemoval.h"
#include "DVTools/ITrigMatch.h"

namespace DV
{
    class DiLepDVCuts : public asg::AsgTool, virtual public DV::IDiLepDVCuts
    {
        // Creates a proper constructor for Athena
        ASG_TOOL_CLASS(DiLepDVCuts, DV::IDiLepDVCuts)

        public:
            // constructor for standalone usage
            DiLepDVCuts(const std::string& name);

            StatusCode initialize() override;

            /*
             * CAUTION!:
             * PrepareVertex() matches leptons to a DV (via tracks) and performs trigger matching.
             * This method has to be called on every DV before the other methods are used.
             * The lepton containers should contain all leptons from the current event.
             */
            void PrepareVertex(xAOD::Vertex& dv,
                               const xAOD::ElectronContainer& elc,
                               const xAOD::MuonContainer& muc) const override;

            const xAOD::ElectronContainer* GetEl(const xAOD::Vertex& dv) const override;
            const xAOD::MuonContainer* GetMu(const xAOD::Vertex& dv) const override;

            bool PassCentralEtaVeto(const xAOD::Vertex& dv) const override;
            bool PassChargeRequirement(const xAOD::Vertex& dv) const override;

            bool PassTriggerMatching(const xAOD::Vertex& dv) const override;
            bool PassDESDMatching(const xAOD::Vertex& dv) const override;

        private:
            ToolHandle<DV::IDiLepDESD> m_desd;
            ToolHandle<DV::IElecCuts> m_ec;
            ToolHandle<DV::IMuonCuts> m_mc;
            ToolHandle<DV::IOverlapRemoval> m_over;
            ToolHandle<DV::ITrigMatch> m_trig;

            // trigger names
            std::string m_trig_siph;
            std::string m_trig_diph;
            std::string m_trig_simu;

            //! cut for central eta veto
            double m_centEta;

            // accessors for leptons associated to a DV
            SG::AuxElement::Accessor<xAOD::ElectronContainer*> m_accEl;
            SG::AuxElement::Accessor<xAOD::MuonContainer*> m_accMu;

            // accessors for trigger matching results
            SG::AuxElement::Accessor<char> m_accTrigSiPh;
            SG::AuxElement::Accessor<char> m_accTrigDiPh;
            SG::AuxElement::Accessor<char> m_accTrigSiMu;
    };
}

#endif   // DV_DILEPDVCUTS_H
