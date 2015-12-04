#include "DVTools/OverlapRemoval.h"

#include "xAODEgamma/ElectronxAODHelpers.h"

DV::OverlapRemoval::OverlapRemoval(const std::string& name) :
    AsgTool(name),
    m_accOver("DV_isOverlap"),
    m_accLepTrk("DV_isLepTrk")
{
}

StatusCode DV::OverlapRemoval::initialize()
{
    // Greet the user:
    ATH_MSG_DEBUG("Initialising... " );

    // Return gracefully:
    return StatusCode::SUCCESS;
}

void DV::OverlapRemoval::RemoveOverlap(xAOD::ElectronContainer& elc,
                                       xAOD::MuonContainer& muc) const
{
    for(auto el1 = elc.begin(); el1 != elc.end(); el1++)
    {
        const xAOD::TrackParticle* el1_tr = xAOD::EgammaHelpers::getOriginalTrackParticle(*el1);

        m_accOver(**el1) = 0;

        // ee
        for(auto el2 = el1+1; el2 != elc.end(); el2++)
        {
            const xAOD::TrackParticle* el2_tr = xAOD::EgammaHelpers::getOriginalTrackParticle(*el2);

            if(el1_tr == el2_tr)
            {
                // remove lower pt electron
                if((*el1)->pt() < (*el2)->pt())
                {
                    m_accOver(**el1) = 1;
                }
                else
                {
                    m_accOver(**el2) = 1;
                }
            }
        }

        if(m_accOver(**el1) == 1) continue;

        // em
        for(auto mu = muc.cbegin(); mu != muc.cend(); mu++)
        {
            const xAOD::TrackParticle* mu_tr = (*mu)->trackParticle(xAOD::Muon::InnerDetectorTrackParticle);

            if(el1_tr == mu_tr)
            {
                // only the electron is removed
                m_accOver(**el1) = 1;
            }
        }
    }

    // mm
    for(auto mu1 = muc.begin(); mu1 != muc.end(); mu1++)
    {
        const xAOD::TrackParticle* mu1_tr = (*mu1)->trackParticle(xAOD::Muon::InnerDetectorTrackParticle);

        m_accOver(**mu1) = 0;

        for(auto mu2 = mu1+1; mu2 != muc.end(); mu2++)
        {
            const xAOD::TrackParticle* mu2_tr = (*mu2)->trackParticle(xAOD::Muon::InnerDetectorTrackParticle);

            if(mu1_tr == mu2_tr)
            {
                // remove lower pt muon
                if((*mu1)->pt() < (*mu2)->pt())
                {
                    m_accOver(**mu1) = 1;
                }
                else
                {
                    m_accOver(**mu2) = 1;
                }
            }
        }
    }
}

void DV::OverlapRemoval::RemoveLeptonTracks(const xAOD::ElectronContainer& elc,
                                            const xAOD::MuonContainer& muc,
                                            xAOD::TrackParticleContainer& trc) const
{
    for(auto tr = trc.begin(); tr != trc.end(); tr++)
    {
        m_accLepTrk(**tr) = 0;

        for(const xAOD::Electron* el: elc)
        {
            const xAOD::TrackParticle* el_tr = xAOD::EgammaHelpers::getOriginalTrackParticle(el);

            if(*tr == el_tr)
            {
                m_accLepTrk(**tr) = 1;
                break;
            }
        }

        if(m_accLepTrk(**tr) == true) continue;

        for(const xAOD::Muon* mu: muc)
        {
            const xAOD::TrackParticle* mu_tr = mu->trackParticle(xAOD::Muon::InnerDetectorTrackParticle);

            if(*tr == mu_tr)
            {
                m_accLepTrk(**tr) = 1;
                break;
            }
        }
    }
}

bool DV::OverlapRemoval::IsOverlap(const xAOD::IParticle& p) const
{
    return m_accOver(p) == 1;
}

bool DV::OverlapRemoval::IsLeptonTrack(const xAOD::TrackParticle& tr) const
{
    return m_accLepTrk(tr) == 1;
}
