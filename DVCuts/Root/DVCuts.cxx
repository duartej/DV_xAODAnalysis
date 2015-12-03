#include "DVCuts/DVCuts.h"

#include "TFile.h"
#include "TH3C.h"

#include "xAODTracking/VertexContainer.h"

#include<cmath>

DV::DVCuts::DVCuts(const std::string & name) :
    asg::AsgTool(name),
    m_accMass("massPionHypo"),
    m_materialMap(nullptr)
{
    declareProperty("d0Max", m_d0Max = 300.0, "Fiducial cut for d0 [mm]");
    declareProperty("z0Max", m_z0Max = 300.0, "Fiducial cut for z0, absolute value [z0]");
    declareProperty("chisqPerDofMax", m_chisqDof = 5.0, "Cut for chi^2 per degrees of freedom of DV fit");
    declareProperty("distMin", m_distMin = 4.0, "Minimum distance of DV to all PVs in transverse plane");
    declareProperty("DVMassMin", m_DVMassMin = 10.0, "Minimum mass of DV");
    declareProperty("MaterialMapFile", m_mapFile = "", "Path to material map file");
}

StatusCode DV::DVCuts::initialize()
{
    // Greet the user:
    ATH_MSG_DEBUG("Initialising... " );

    if(m_mapFile.empty())
    {
        ATH_MSG_WARNING("No material map defined!");
    }
    else
    {
        TFile* mapFH = new TFile(m_mapFile.c_str(), "READ");
        if(mapFH->IsOpen())
        {
            mapFH->GetObject("map", m_materialMap);
            if(m_materialMap) m_materialMap->SetDirectory(0);
        }
        mapFH->Close();

        if(m_materialMap == nullptr)
        {
            ATH_MSG_ERROR("Material map could not be loaded!");
            return StatusCode::FAILURE;
        }
    }

    // Return gracefully:
    return StatusCode::SUCCESS;
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

bool DV::DVCuts::PassChisqCut(const xAOD::Vertex& dv) const
{
    float chisqPerDOF = dv.chiSquared() / dv.numberDoF();

    if(chisqPerDOF > m_chisqDof) return false;
    return true;
}

bool DV::DVCuts::PassDistCut(const xAOD::Vertex& dv, const xAOD::VertexContainer& pvc) const
{
    Amg::Vector3D dv_pos = dv.position();
    for(const xAOD::Vertex* pv: pvc)
    {
        Amg::Vector3D dist = pv->position() - dv_pos;
        if(dist.perp() < m_distMin)
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

bool DV::DVCuts::PassMassCut(const xAOD::Vertex& dv) const
{
    if(std::fabs(m_accMass(dv)) < m_DVMassMin)
    {
        return false;
    }
    return true;
}

bool DV::DVCuts::PassVertexCuts(const xAOD::Vertex& dv, const xAOD::VertexContainer& pvc) const
{
    // dv in fiducial volume?
    if(!PassFiducialCuts(dv))
    {
        return false;
    }

    // good fit?
    if(!PassChisqCut(dv))
    {
        return false;
    }

    // distance of DV to all PVs in transverse plane
    if(!PassDistCut(dv, pvc))
    {
        return false;
    }

    // material veto using a detector map
    if(!PassMaterialVeto(dv))
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

