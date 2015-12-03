#include "DVCuts/DiLepDVCuts.h"

#include "TFile.h"
#include "TH3C.h"

#include "xAODTracking/TrackParticleContainer.h"
#include "xAODTracking/VertexContainer.h"

#include<cmath>

DV::DiLepDVCuts::DiLepDVCuts(const std::string & name) :
    asg::AsgTool(name)
{
    declareProperty("centEtaMax", m_centEta = 0.02, "Cut on |eta| for the central track veto");
}

StatusCode DV::DiLepDVCuts::initialize()
{
    // Greet the user:
    ATH_MSG_DEBUG("Initialising... " );

    // Return gracefully:
    return StatusCode::SUCCESS;
}

bool DV::DiLepDVCuts::PassCentralEtaVeto(const xAOD::Vertex& dv) const
{
    for(const auto& trl: dv.trackParticleLinks())
    {
        if(std::fabs((*trl)->eta()) < 0.02) return false;
    }
    return true;
}

bool DV::DiLepDVCuts::PassChargeRequirement(const xAOD::Vertex& dv) const
{
    bool has_pos = false;
    bool has_neg = false;

    for(const auto& trl: dv.trackParticleLinks())
    {
        has_pos |= ((*trl)->charge() > 0);
        has_neg |= ((*trl)->charge() < 0);
    }

    return has_pos && has_neg;
}
