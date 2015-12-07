#include "DVTools/PhotonMatch.h"

DV::PhotonMatch::PhotonMatch(const std::string & name) :
    asg::AsgTool(name),
    m_decPhot("DV_PhMatch")
{
}

StatusCode DV::PhotonMatch::initialize()
{
    // Greet the user:
    ATH_MSG_DEBUG("Initialising... ");

    // Return gracefully:
    return StatusCode::SUCCESS;
}

void DV::PhotonMatch::MatchPhotons(const xAOD::PhotonContainer& phc, const xAOD::ElectronContainer& elc) const
{
    for(const xAOD::Electron* el: elc)
    {
        const xAOD::CaloCluster* el_calo = el->caloCluster();

        m_decPhot(*el) = nullptr;

        for(const xAOD::Photon* ph: phc)
        {
            const xAOD::CaloCluster* ph_calo = ph->caloCluster();

            // search for a photon with the same calo cluster as the electron
            if(ph_calo->p4().DeltaR(el_calo->p4()) < 0.01)
            {
                m_decPhot(*el) = ph;
                break;
            }
        }
    }
}

const xAOD::Photon* DV::PhotonMatch::GetPhoton(const xAOD::Electron& el) const
{
    if(!m_decPhot.isAvailable(el))
    {
        ATH_MSG_WARNING("Failed to retrieve matched photon from electron!");
        return nullptr;
    }

    return m_decPhot(el);
}
