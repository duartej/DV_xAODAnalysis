#include "DVTools/OverlapRemoval.h"

#include "xAODEgamma/ElectronxAODHelpers.h"

DV::OverlapRemoval::OverlapRemoval(const std::string& name) :
    AsgTool(name),
    m_decOver("DV_isOverlap"),
    m_decLepTrk("DV_isLepTrk")
{
}

StatusCode DV::OverlapRemoval::initialize()
{
    // Greet the user:
    ATH_MSG_DEBUG("Initialising... " );

    // Return gracefully:
    return StatusCode::SUCCESS;
}

void DV::OverlapRemoval::FindOverlap(const xAOD::ElectronContainer& elc,
                                     const xAOD::MuonContainer& muc) const
{
    for(auto el1 = elc.cbegin(); el1 != elc.cend(); el1++)
    {
        const xAOD::TrackParticle* el1_tr = xAOD::EgammaHelpers::getOriginalTrackParticle(*el1);

        m_decOver(**el1) = 0;

        // ee
        for(auto el2 = el1+1; el2 != elc.cend(); el2++)
        {
            const xAOD::TrackParticle* el2_tr = xAOD::EgammaHelpers::getOriginalTrackParticle(*el2);

            if(el1_tr == el2_tr)
            {
                // remove lower pt electron
                if((*el1)->pt() < (*el2)->pt())
                {
                    m_decOver(**el1) = 1;
                }
                else
                {
                    m_decOver(**el2) = 1;
                }
            }
        }

        if(m_decOver(**el1) == 1) continue;

        // em
        for(auto mu = muc.cbegin(); mu != muc.cend(); mu++)
        {
            const xAOD::TrackParticle* mu_tr = (*mu)->trackParticle(xAOD::Muon::InnerDetectorTrackParticle);

            if(el1_tr == mu_tr)
            {
                // only the electron is removed
                m_decOver(**el1) = 1;
            }
        }
    }

    // mm
    for(auto mu1 = muc.cbegin(); mu1 != muc.cend(); mu1++)
    {
        const xAOD::TrackParticle* mu1_tr = (*mu1)->trackParticle(xAOD::Muon::InnerDetectorTrackParticle);

        m_decOver(**mu1) = 0;

        for(auto mu2 = mu1+1; mu2 != muc.cend(); mu2++)
        {
            const xAOD::TrackParticle* mu2_tr = (*mu2)->trackParticle(xAOD::Muon::InnerDetectorTrackParticle);

            if(mu1_tr == mu2_tr)
            {
                // remove lower pt muon
                if((*mu1)->pt() < (*mu2)->pt())
                {
                    m_decOver(**mu1) = 1;
                }
                else
                {
                    m_decOver(**mu2) = 1;
                }
            }
        }
    }
}

void DV::OverlapRemoval::FindLeptonTracks(const xAOD::ElectronContainer& elc,
                                          const xAOD::MuonContainer& muc,
                                          const xAOD::TrackParticleContainer& trc) const
{
    for(auto tr = trc.cbegin(); tr != trc.cend(); tr++)
    {
        m_decLepTrk(**tr) = 0;

        for(const xAOD::Electron* el: elc)
        {
            const xAOD::TrackParticle* el_tr = xAOD::EgammaHelpers::getOriginalTrackParticle(el);

            if(*tr == el_tr)
            {
                m_decLepTrk(**tr) = 1;
                break;
            }
        }

        if(m_decLepTrk(**tr) == true) continue;

        for(const xAOD::Muon* mu: muc)
        {
            const xAOD::TrackParticle* mu_tr = mu->trackParticle(xAOD::Muon::InnerDetectorTrackParticle);

            if(*tr == mu_tr)
            {
                m_decLepTrk(**tr) = 1;
                break;
            }
        }
    }
}

bool DV::OverlapRemoval::IsOverlap(const xAOD::IParticle& p) const
{
    return m_decOver(p) == 1;
}

bool DV::OverlapRemoval::IsLeptonTrack(const xAOD::TrackParticle& tr) const
{
    return m_decLepTrk(tr) == 1;
}
