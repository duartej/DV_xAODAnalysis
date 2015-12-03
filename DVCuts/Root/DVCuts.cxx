#include "DVCuts/DVCuts.h"

#include "TH3C.h"

//#include "xAODTracking/TrackParticle.h"
#include "xAODTracking/TrackParticleContainer.h"

#include<cmath>


DV::DVCuts::DVCuts(const std::string & name) :
    asg::AsgTool(name),
    //m_pc("DV::ParticleCuts/ParticleCuts"),
    // FIXME:: Sure that name is going to be fixed forever? ...
    m_acc_vp4("vertexP4"),
    m_materialMap(nullptr)
{
    declareProperty("d0Max", m_d0Max = 300.0, "Fiducial cut for d0 [mm]");
    declareProperty("z0Max", m_z0Max = 300.0, "Fiducial cut for z0, absolute value [z0]");
}

StatusCode DV::DVCuts::initialize()
{
    // Greet the user:
    ATH_MSG_DEBUG("Initialising... " );
    // retrieve ParticleCuts
    /*
    if(m_pc.retrieve().isFailure())
    {
        ATH_MSG_ERROR("Could not retrieve Tool: ParticleCuts!");
        return StatusCode::FAILURE;  
    }*/

    // Return gracefully:
    return StatusCode::SUCCESS;
}


bool DV::DVCuts::GetMaterialMap(const std::string& path)
{
    // I don't see why should be here this function ....
    /*m_materialFile = new TFile(PathResolverFindCalibFile(path).c_str(), "READ");
    if(!m_materialFile->IsOpen()) return false;
  
    m_materialFile->GetObject("map", m_materialMap);
    if(m_materialMap == nullptr) return false;
    */
    
    return true;
}

bool DV::DVCuts::PassFiducialCuts(const xAOD::Vertex& dv) const
{
    Amg::Vector3D dv_pos = dv.position();
    
    if(dv_pos.perp() > m_d0Max || std::fabs(dv_pos.z()) > m_z0Max)
    {
        return false;
    }
    return true;
}

bool DV::DVCuts::PassTrackd0(const xAOD::Vertex& dv, const Amg::Vector3D& pv) const
{
    /* fIXME: Not available yet ParticleCut
    for(const auto& trl: dv.trackParticleLinks())
    {
        if(std::fabs(m_pc->d0wrtPV(**trl, pv)) < 2.)
        { 
            return false;
        }
    }*/
    return true;
}

bool DV::DVCuts::PassDVDist(const xAOD::Vertex& dv, const std::vector<Amg::Vector3D>& pvs) const
{
    // maximum distance in the tranverse plane [mm]
    const int m_maxDistanceDV_TP = 4.0;

    Amg::Vector3D dv_pos = dv.position();
    for(const Amg::Vector3D& pv: pvs)
    {
        Amg::Vector3D dist = pv - dv_pos;
        // FIXME: either Property or an new argument to this function 
        // (plus an overloaded func. with this default)
        if(dist.perp() < m_maxDistanceDV_TP)
        {
            return false;
        }
    }
    return true;
}

bool DV::DVCuts::PassMaterialVeto(const xAOD::Vertex& dv) const
{
    Amg::Vector3D dv_pos = dv.position();
    
    double rDV   = dv_pos.perp();
    double phiDV = dv_pos.phi();
    if(phiDV < 0) 
    {
        //FIXME:: Not available yet the utils pkg.
        //phiDV = 2. * DV::Pi + phiDV;
        phiDV = 2.*3.1415926535897932384626433832795028841971693993 + phiDV;
    }
  
    double zDV = dv_pos.z();
    if(m_materialMap->GetBinContent(m_materialMap->FindBin(rDV, phiDV, zDV)) > 0)
    {
        return false;
    }
    return true;
}

bool DV::DVCuts::PassCentralEtaVeto(const xAOD::Vertex& dv) const
{
    // max. allowed eta
    const float m_absMaxEta = 0.02;
    for(const auto& trl: dv.trackParticleLinks())
    {
        //FIXME:: either a property or a argument to this function (plus the overload..
        if(std::fabs((*trl)->eta()) < m_absMaxEta)
        {
            return false;
        }
    }
    return true;
}

bool DV::DVCuts::PassChargeRequirement(const xAOD::Vertex& dv) const
{
    bool has_pos = false;
    bool has_neg = false;
    
    for(const auto & trl: dv.trackParticleLinks())
    {
        has_pos |= ((*trl)->charge() > 0);
        has_neg |= ((*trl)->charge() < 0);
    }
    return has_pos && has_neg;
}

bool DV::DVCuts::PassMassCut(const xAOD::Vertex& dv) const
{
    const double m_maxDVMass = 10000.; // 10.* DV::GeVtoMeV - FIXME: not available utils yet
    //FIXME:: either a property or a argument to this function (plus the overload..
    if(std::fabs(m_acc_vp4(dv).M()) < m_maxDVMass) 
    {
        return false;
    }
    return true;
}

bool DV::DVCuts::PassVertexCuts(const xAOD::Vertex& dv, const std::vector<Amg::Vector3D>& pvs) const
{
    /*
     CAUTION:
     the selected tracks have the following properties:
       - lepton tracks: pass signal selection
       - pass cosmic veto
       - pass at least one DESD filter (=> lepton tracks: trigger matched)
   
     the fitted displaced vertex has the following properties:
       - chi2 < 5
       - closeToPV veto
       - inner hit veto
    */
  
    // dv in fiducial volume?
    if(!PassFiducialCuts(dv))
    {
        return false;
    }

    // d0 of tracks wrt to PV
    if(!PassTrackd0(dv, pvs.at(0))) 
    {
        return false;
    }

    // distance of DV to PV in transverse plane
    if(!PassDVDist(dv, pvs))
    {
        return false;
    }
  
    // material veto using a detector map
    if(!PassMaterialVeto(dv)) 
    {
        return false;
    }
  
    // central track veto
    if(!PassCentralEtaVeto(dv))
    {
        return false;
    }

    // opposite charge cut
    if(!PassChargeRequirement(dv))
    {
        return false;
    }

    // invariant mass cut
    if(!PassMassCut(dv))
    {
        return false;
    }

    return true;
}

