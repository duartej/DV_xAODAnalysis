#include "DVCuts/DVCuts.h"

#include "TFile.h"
#include "TH3C.h"
#ifdef ASGTOOL_STANDALONE
#include "TSystem.h"
#endif

#include "xAODTracking/VertexContainer.h"

#include<cmath>

DV::DVCuts::DVCuts(const std::string & name) :
    asg::AsgTool(name),
    m_accMass("massPionHypo"),
    m_materialMap(nullptr)
{
    declareProperty("rDVMax", m_rDVMax = 300.0, "Fiducial cut for r_DV [mm]");
    declareProperty("zDVMax", m_zDVMax = 300.0, "Fiducial cut for |z_DV| [mm]");
    declareProperty("chisqPerDofMax", m_chisqDof = 5.0, "Cut for chi^2 per degrees of freedom of DV fit");
    declareProperty("distMin", m_distMin = 4.0, "Minimum distance of DV to all PVs in transverse plane [mm]");
    declareProperty("DVMassMin", m_DVMassMin = 10000.0, "Minimum mass of DV [MeV]");
    declareProperty("MaterialMapFile", m_mapFile = "", "Path to material map file");
}

DV::DVCuts::~DVCuts()
{
    delete m_materialMap;
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
#ifdef ASGTOOL_STANDALONE
        m_mapFile = "$ROOTCOREBIN/../DV_xAODAnalysis/DVAnalyses/data/" + m_mapFile;
        m_mapFile = gSystem->ExpandPathName(m_mapFile.c_str());
#endif

        TFile* mapFH = new TFile(m_mapFile.c_str(), "READ");
        if(mapFH->IsOpen())
        {
            mapFH->GetObject("map", m_materialMap);
            if(m_materialMap) m_materialMap->SetDirectory(0);
        }
        mapFH->Close();
        delete mapFH;

        if(m_materialMap == nullptr)
        {
            ATH_MSG_ERROR("Material map could not be loaded!");
            return StatusCode::FAILURE;
        }

        ATH_MSG_INFO("Material map was successfully loaded!");
    }

    // Return gracefully:
    return StatusCode::SUCCESS;
}

bool DV::DVCuts::PassFiducialCuts(const xAOD::Vertex& dv) const
{
    Amg::Vector3D dv_pos = dv.position();

    if(dv_pos.perp() > m_rDVMax || std::fabs(dv_pos.z()) > m_zDVMax)
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
    if(!m_materialMap) return false;

    Amg::Vector3D dv_pos = dv.position();

    double rDV   = dv_pos.perp();
    double phiDV = dv_pos.phi();
    if(phiDV < 0)
    {
        phiDV = 2.*std::acos(-1.) + phiDV;
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

