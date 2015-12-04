#ifndef DV_DILEPDVCUTS_H
#define DV_DILEPDVCUTS_H

// STL
#include <string>

// Tools
#include "AsgTools/AsgTool.h"

// xAOD
#include "xAODEgamma/ElectronContainer.h"
#include "xAODEgamma/PhotonContainer.h"
#include "xAODMuon/MuonContainer.h"

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

            // method matches leptons and photons to vertex and decorates the vertex
            void ApplyLeptonMatching(xAOD::Vertex& dv,
                                     const xAOD::ElectronContainer& elc,
                                     const xAOD::PhotonContainer& phc,
                                     const xAOD::MuonContainer& muc) const override;
            void ApplyOverlapRemoval(const xAOD::Vertex& dv) const override;
            void ApplyTriggerMatching(xAOD::Vertex& dv) const override;

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
            SG::AuxElement::Accessor<xAOD::PhotonContainer*> m_accPh;
            SG::AuxElement::Accessor<xAOD::MuonContainer*> m_accMu;

            // accessors for trigger matching results
            SG::AuxElement::Accessor<char> m_accTrigSiPh;
            SG::AuxElement::Accessor<char> m_accTrigDiPh;
            SG::AuxElement::Accessor<char> m_accTrigSiMu;

            void GetLeptonContainers(const xAOD::Vertex& dv,
                                     xAOD::ElectronContainer*& elc,
                                     xAOD::MuonContainer*& muc) const;

            void GetPhotonContainer(const xAOD::Vertex& dv,
                                    xAOD::PhotonContainer*& phc) const;
    };
}

#endif   // DV_DiLepDVCUTS_H
