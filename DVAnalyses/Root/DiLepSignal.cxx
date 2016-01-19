// To be used for RootCore ONLY
#ifdef ASGTOOL_STANDALONE

// ROOT
#include "TH1.h"
#include "TH2.h"

// xAOD
#include "xAODEventInfo/EventInfo.h"

// DV
#include "DVAnalyses/DiLepSignal.h"
#include "DVTools/PlotsManagerTool.h"
#include "DVUtils/LSP.h"

DV::DiLepSignal::DiLepSignal() :
    m_filters("DV::DiLepDESD/DiLepDESD")
{
    m_vx_types = {"ee", "em", "mm"};
    m_filt_names = {"SiEl", "SiPhX", "SiMu", "DiEl", "DiPh", "DiElPh", "DiLoElPh", "All"};

    m_bins = 16;
    m_bin_edges = std::vector<double>(m_bins+1, 0.0);
    m_bin_edges.at(1)  =   2.0;
    m_bin_edges.at(2)  =   6.0;
    m_bin_edges.at(3)  =  10.0;
    m_bin_edges.at(4)  =  20.0;
    m_bin_edges.at(5)  =  40.0;
    m_bin_edges.at(6)  =  60.0;
    m_bin_edges.at(7)  =  80.0;
    m_bin_edges.at(8)  = 100.0;
    m_bin_edges.at(9)  = 120.0;
    m_bin_edges.at(10) = 140.0;
    m_bin_edges.at(11) = 160.0;
    m_bin_edges.at(12) = 180.0;
    m_bin_edges.at(13) = 200.0;
    m_bin_edges.at(14) = 230.0;
    m_bin_edges.at(15) = 260.0;
    m_bin_edges.at(16) = 300.0;

    assignCutsAndTools();
}

void DV::DiLepSignal::bookHists(PlotsManagerTool* pm)
{
    pm->setCurrentModule("DiLep");

    // create and register histograms
    for(const std::string& vt: m_vx_types)
    {
        std::string hname = "vc_" + vt;
        m_hists1D[hname] = pm->bookTH1<TH1D>(hname, "", m_bins, m_bin_edges);

        for(const std::string& fn: m_filt_names)
        {
            if(vt == "ee" && fn == "SiMu") continue;
            if(vt == "em" && (fn.substr(0, 2) != "Si" && fn != "All")) continue;
            if(vt == "mm" && fn != "SiMu") continue;

            hname = "ve_" + vt + "_" + fn;
            m_hists1D[hname] = pm->bookTH1<TH1D>(hname, "", m_bins, m_bin_edges);
        }

        if(vt == "mm") continue;

        // overlap histogram
        hname = "ov_" + vt;
        std::size_t ov_bins = m_filt_names.size()-1;
        double ov_max = static_cast<double>(ov_bins);
        m_hists2D[hname] = pm->bookTH2<TH2D>(hname, "", ov_bins, 0., ov_max, ov_bins, 0., ov_max);
    }

    // LSP histograms
    m_hists1D["LSP_perp"] = pm->bookTH1<TH1D>("LSP_perp", "", 1000, 0., 1000.);
    m_hists1D["LSP_z"] = pm->bookTH1<TH1D>("LSP_z", "", 10000, -5000., 5000.);
    m_hists1D["LSP_dist"] = pm->bookTH1<TH1D>("LSP_dist", "", 5000, 0., 5000.);
}

bool DV::DiLepSignal::initialize()
{
    return true;
}

bool DV::DiLepSignal::execute(xAOD::TEvent* evt)
{
    // retrieve event info
    const xAOD::EventInfo* ei = nullptr;
    if(evt->retrieve(ei, "EventInfo").isFailure())
    {
        Error("DiLepSignal::execute()", "Failed to retrieve EventInfo!");
        return false;
    }

    // check for MC as we want to use truth information
    if(!ei->eventType(xAOD::EventInfo::IS_SIMULATION))
    {
        Error("DiLepSignal::execute()", "This analysis cannot be used on data!");
        return false;
    }

    // retrieve triggers
    m_filters->RetrieveTriggerFlags();

    // retrieve particles
    const xAOD::ElectronContainer* elc = nullptr;
    if(evt->retrieve(elc, "Electrons").isFailure())
    {
        Error("DiLepSignal::execute()", "Failed to retrieve Electrons!");
        return false;
    }
    const xAOD::PhotonContainer* phc = nullptr;
    if(evt->retrieve(phc, "Photons").isFailure())
    {
        Error("DiLepSignal::execute()", "Failed to retrieve Photons!");
        return false;
    }
    const xAOD::MuonContainer* muc = nullptr;
    if(evt->retrieve(muc, "Muons").isFailure())
    {
        Error("DiLepSignal::execute()", "Failed to retrieve Muons!");
        return false;
    }
    const xAOD::TruthParticleContainer* truc = nullptr;
    if(evt->retrieve(truc, "TruthParticles").isFailure())
    {
        Error("DiLepSignal::execute()", "Failed to retrieve TruthParticles!");
        return false;
    }

    // retrieve LSPs
    auto LSP_store = DV::LSP::GetLSPs(*truc);
    if(LSP_store.size() != 2)
    {
        Warning("DiLepSignal::execute()", "Event does not contain two LSPs!");
        return true;
    }

    for(auto LSP: LSP_store)
    {
        // retrieve decay position of LSP
        const xAOD::TruthVertex* LSP_vtx = LSP->GetVtx();
        float decay_radius = LSP_vtx->perp();
        float decay_z = LSP_vtx->z();

        m_hists1D.at("LSP_perp")->Fill(decay_radius);
        m_hists1D.at("LSP_z")->Fill(decay_z);
        m_hists1D.at("LSP_dist")->Fill(std::sqrt(decay_radius*decay_radius + decay_z*decay_z));

        // fiducial volume cuts
        if(decay_radius > 300.0) continue;
        if(std::fabs(decay_z) > 300.0) continue;

        // determine vertex type
        std::string vx_type = "";
        if(LSP->GetNElChilds() == 2) vx_type = "ee";
        else if(LSP->GetNMuChilds() == 2) vx_type = "mm";
        else vx_type = "em";

        // vertex type counter
        m_hists1D.at("vc_" + vx_type)->Fill(decay_radius);

        // deltaR truth matching
        LSP->MatchReco(*elc, *phc, *muc);
        auto LSP_elc = LSP->GetEl();
        auto LSP_phc = LSP->GetPh();
        auto LSP_muc = LSP->GetMu();

        // map to store filter flags
        std::map<std::string, bool> filt_passed;
        for(const std::string& fn: m_filt_names)
        {
             if(fn == "All") continue;

             filt_passed[fn] = false;
        }

        if(vx_type == "ee" || vx_type == "em")
        {
            for(auto el1 = LSP_elc.cbegin(); el1 != LSP_elc.cend(); el1++)
            {
                if(!filt_passed.at("SiEl") && m_filters->PassSiEl(**el1))
                {
                    m_hists1D.at("ve_" + vx_type + "_SiEl")->Fill(decay_radius);
                    filt_passed.at("SiEl") = true;
                }
            }
        }

        if(vx_type == "ee")
        {
            for(auto el1 = LSP_elc.cbegin(); el1 != LSP_elc.cend(); el1++)
            {
                for(auto ph = LSP_phc.cbegin(); ph != LSP_phc.cend(); ph++)
                {
                    if(!filt_passed.at("SiPhX") && m_filters->PassSiPhX(**ph, **el1))
                    {
                        m_hists1D.at("ve_" + vx_type + "_SiPhX")->Fill(decay_radius);
                        filt_passed.at("SiPhX") = true;
                    }
                    if(!filt_passed.at("DiElPh") && m_filters->PassDiElPh(**el1, **ph))
                    {
                        m_hists1D.at("ve_" + vx_type + "_DiElPh")->Fill(decay_radius);
                        filt_passed.at("DiElPh") = true;
                    }
                    if(!filt_passed.at("DiLoElPh") && m_filters->PassDiLoElPh(**el1, **ph))
                    {
                        m_hists1D.at("ve_" + vx_type + "_DiLoElPh")->Fill(decay_radius);
                        filt_passed.at("DiLoElPh") = true;
                    }
                }

                for(auto el2 = el1+1; el2 != LSP_elc.cend(); el2++)
                {
                    if(!filt_passed.at("DiEl") && m_filters->PassDiEl(**el1, **el2))
                    {
                        m_hists1D.at("ve_" + vx_type + "_DiEl")->Fill(decay_radius);
                        filt_passed.at("DiEl") = true;
                        break;
                    }
                }
            }

            for(auto ph1 = LSP_phc.cbegin(); ph1 != LSP_phc.cend(); ph1++)
            {
                for(auto ph2 = ph1+1; ph2 != LSP_phc.cend(); ph2++)
                {
                    if(!filt_passed.at("SiPhX") && m_filters->PassSiPhX(**ph1, **ph2))
                    {
                        m_hists1D.at("ve_" + vx_type + "_SiPhX")->Fill(decay_radius);
                        filt_passed.at("SiPhX") = true;
                    }

                    if(!filt_passed.at("DiPh") && m_filters->PassDiPh(**ph1, **ph2))
                    {
                        m_hists1D.at("ve_" + vx_type + "_DiPh")->Fill(decay_radius);
                        filt_passed.at("DiPh") = true;
                    }
                }
            }
        }

        if(vx_type == "mm" || vx_type == "em")
        {
            for(auto mu = LSP_muc.cbegin(); mu != LSP_muc.cend(); mu++)
            {
                if(!filt_passed.at("SiMu") && m_filters->PassSiMu(**mu))
                {
                    m_hists1D.at("ve_" + vx_type + "_SiMu")->Fill(decay_radius);
                    filt_passed.at("SiMu") = true;
                }

                if(vx_type == "mm") continue;

                for(auto ph = LSP_phc.cbegin(); ph != LSP_phc.cend(); ph++)
                {
                    if(!filt_passed.at("SiPhX") && m_filters->PassSiPhX(**ph, **mu))
                    {
                        m_hists1D.at("ve_" + vx_type + "_SiPhX")->Fill(decay_radius);
                        filt_passed.at("SiPhX") = true;
                        break;
                    }
                }
            }
        }

        if(vx_type == "ee" || vx_type == "em")
        {
            // calculate total filter efficiency
            for(const std::string& fn: m_filt_names)
            {
                if(fn == "All") continue;

                if(filt_passed.at(fn) == true)
                {
                    m_hists1D.at("ve_" + vx_type + "_All")->Fill(decay_radius);
                    break;
                }
            }

            // calculate overlap between filters
            // loops assume that last filter is "All"
            for(std::size_t i = 0; i < m_filt_names.size()-1; i++)
            {
                std::string fn1 = m_filt_names.at(i);
                if(!filt_passed.at(fn1)) continue;

                for(std::size_t j = 0; j < m_filt_names.size()-1; j++)
                {
                    if(i == j) continue;

                    std::string fn2 = m_filt_names.at(j);
                    if(!filt_passed.at(fn2)) continue;

                    m_hists2D.at("ov_" + vx_type)->Fill(static_cast<double>(i), static_cast<double>(j));
                }
            }
        }
    }

    return true;
}

bool DV::DiLepSignal::finalize()
{
    // normalize histograms
    for(const std::string& vt: m_vx_types)
    {
        TH1D* denom = m_hists1D.at("vc_" + vt);
        denom->Sumw2();

        // overlap histograms
        if(vt != "mm")
        {
            for(std::size_t i = 0; i < m_filt_names.size()-1; i++)
            {
                std::string fn = m_filt_names.at(i);
                if(vt == "ee" && fn == "SiMu") continue;
                if(vt == "em" && fn.substr(0, 2) != "Si") continue;

                for(std::size_t j = 0; j < m_filt_names.size()-1; j++)
                {
                    if(i == j) continue;

                    double ov = m_hists2D.at("ov_" + vt)->GetBinContent(i+1, j+1);
                    double fact = m_hists1D.at("ve_" + vt + "_" + fn)->GetEntries();
                    if(fact == 0) continue;

                    m_hists2D.at("ov_" + vt)->SetBinContent(i+1, j+1, ov / fact);
                }
            }
        }

        // vertex efficiency histograms
        for(const std::string& fn: m_filt_names)
        {
            if(vt == "ee" && fn == "SiMu") continue;
            if(vt == "em" && (fn.substr(0, 2) != "Si" && fn != "All")) continue;
            if(vt == "mm" && fn != "SiMu") continue;

            TH1D* nom = m_hists1D.at("ve_" + vt + "_" + fn);
            nom->Sumw2();

            nom->Divide(nom, denom, 1., 1., "B");
        }
    }

    return true;
}

void DV::DiLepSignal::assignCutsAndTools()
{
    // add analysis specific DV tools
    m_toolnames.push_back("DV::DiLepDESD/DiLepDESD");
    m_toolnames.push_back("DV::ElecCuts/DiLepElecCuts");
    m_toolnames.push_back("DV::MuonCuts/DiLepMuonCuts");

    // add generic DV tools which can be shared between analyses
    m_toolnames.push_back("DV::PhotonMatch/PhotonMatch");
}

#endif

