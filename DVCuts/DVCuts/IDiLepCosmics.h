#ifndef DV_IDILEPCOSMICS_H
#define DV_IDILEPCOSMICS_H

// ROOT
#include "TLorentzVector.h"

// Framework includes
#include "AsgTools/IAsgTool.h"

// xAOD
#include "xAODEgamma/ElectronContainer.h"
#include "xAODMuon/MuonContainer.h"

namespace DV
{
    class IDiLepCosmics : public virtual asg::IAsgTool
    {
        ASG_TOOL_INTERFACE(DV::IDiLepCosmics)

        public:
            virtual bool PassCosmicVeto(const TLorentzVector& lv1, const TLorentzVector& lv2) const = 0;
            virtual bool PassCosmicEventVeto(const xAOD::ElectronContainer& elc, const xAOD::MuonContainer& muc) const = 0;

            virtual bool PassMuPairCosmicVeto(const xAOD::Muon& mu1, const xAOD::Muon& mu2) const = 0;
            virtual bool PassElMuPairCosmicVeto(const xAOD::Electron& el, const xAOD::Muon& mu) const = 0;
            virtual bool PassElPairCosmicVeto(const xAOD::Electron& el1, const xAOD::Electron& el2) const = 0;
    };
}

#endif
