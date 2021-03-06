#ifndef DV_IDILEPDESD_H
#define DV_IDILEPDESD_H

// ROOT
#include "TLorentzVector.h"

// Tools
#include "AsgTools/IAsgTool.h"

// xAOD
#include "xAODEgamma/ElectronContainer.h"
#include "xAODEgamma/Photon.h"
#include "xAODMuon/MuonContainer.h"

namespace DV
{
    class IDiLepDESD : public virtual asg::IAsgTool
    {
        ASG_TOOL_INTERFACE(DV::IDiLepDESD)

        public:
            virtual void RetrieveTriggerFlags() = 0;
            virtual void SetTriggerFlags(bool siph, bool diph, bool simu) = 0;

            virtual bool PassSiEl(const xAOD::Electron& el) const = 0;
            virtual bool PassSiPhX(const xAOD::Photon& ph, const xAOD::Electron& el) const = 0;
            virtual bool PassSiPhX(const xAOD::Photon& ph1, const xAOD::Photon& ph2) const = 0;
            virtual bool PassSiPhX(const xAOD::Photon& ph, const xAOD::Muon& mu) const = 0;
            virtual bool PassSiMu(const xAOD::Muon& mu) const = 0;

            virtual bool PassDiEl(const xAOD::Electron& el1, const xAOD::Electron& el2) const = 0;
            virtual bool PassDiPh(const xAOD::Photon& ph1, const xAOD::Photon& ph2) const = 0;
            virtual bool PassDiElPh(const xAOD::Electron& el, const xAOD::Photon& ph) const = 0;
            virtual bool PassDiLoElPh(const xAOD::Electron& el, const xAOD::Photon& ph) const = 0;

            virtual bool PassAny(const xAOD::ElectronContainer& elc, const xAOD::MuonContainer& muc) const = 0;

            virtual bool PassSiElCuts(const xAOD::Electron& el) const = 0;
            virtual bool PassSiPhCuts(const xAOD::Photon& ph) const = 0;
            virtual bool PassSiPhXCuts(const xAOD::Electron& el) const = 0;
            virtual bool PassSiPhXCuts(const xAOD::Photon& ph) const = 0;
            virtual bool PassSiPhXCuts(const xAOD::Muon& mu) const = 0;
            virtual bool PassSiMuCuts(const xAOD::Muon& mu) const = 0;

            virtual bool PassDiElCuts(const xAOD::Electron& el) const = 0;
            virtual bool PassDiPhCuts(const xAOD::Photon& ph) const = 0;
            virtual bool PassDiElPhCuts(const xAOD::Electron& el) const = 0;
            virtual bool PassDiElPhCuts(const xAOD::Photon& ph) const = 0;
            virtual bool PassDiLoElPhCuts(const xAOD::Electron& el) const = 0;
            virtual bool PassDiLoElPhCuts(const xAOD::Photon& ph) const = 0;

            virtual bool SameCluster(const TLorentzVector& t1, const TLorentzVector& t2) const = 0;
    };
}

#endif
