
#include "DVTools/PlotsManagerTool.h"

#include "TH1.h"
#include "TH2.h"
#include "TList.h"
#include "TFile.h"
#include "TObject.h"

#include<stdexcept>

// this is needed to distribute the algorithm to the workers
//ClassImp(DV::PlotsManagerTool)

DV::PlotsManagerTool::PlotsManagerTool(const std::string & name):
    asg::AsgTool( name ),
    m_histList(0),
    m_outputfile(0)
{
    m_histList = new TList();
}

StatusCode DV::PlotsManagerTool::initialize()
{
    // Greet the user:
    ATH_MSG_DEBUG("Initialising... " );

    // Return gracefully:
    return StatusCode::SUCCESS;
}

void DV::PlotsManagerTool::bookFile(const std::string & outfilename, const std::string & mode)
{
    if(!m_outputfile)
    {
        m_outputfile = TFile::Open(outfilename.c_str(), mode.c_str());
    }
    else
    {
        std::string message("PlotsManagerTool ERROR: There is already an open output file!");
        throw std::runtime_error(message);
    }
}

void DV::PlotsManagerTool::checkHistName(const std::string & hist_name) const
{
    if( m_histList->FindObject(hist_name.c_str()) != 0 )
    {
        std::string message("PlotsManagerTool ERROR: A histogram with name '" + hist_name +
                "' already exists! Please change the name of this histo!");
        throw std::runtime_error(message);
    }
}

template <class THist1Dim>
THist1Dim * DV::PlotsManagerTool::bookTH1(const std::string & name, const std::string & title,
                    const int & xbins, const double & xmin, const double & xmax )
{
    const std::string hname(name + "_" + m_currentModule);
    checkHistName(hname);

    auto hist = new THist1Dim(hname.c_str(), title.c_str(), xbins, xmin, xmax);
    m_histList->Add(hist);

    return hist;
}

template <class THist1Dim>
THist1Dim * DV::PlotsManagerTool::bookTH1(const std::string & name, const std::string & title,
                    const int & xbins, const std::vector<double> & xbin_edges )
{
    const std::string hname(name + "_" + m_currentModule);
    checkHistName(hname);

    auto hist = new THist1Dim(hname.c_str(), title.c_str(), xbins, &xbin_edges[0]);
    m_histList->Add(hist);

    return hist;
}

template <class THist2Dim>
THist2Dim * DV::PlotsManagerTool::bookTH2(const std::string & name, const std::string & title,
                    const int & xbins, const double & xmin, const double & xmax,
                    const int & ybins, const double & ymin, const double & ymax )
{
    const std::string hname(name + "_" + m_currentModule);
    checkHistName(hname);

    auto hist = new THist2Dim(hname.c_str(), title.c_str(), xbins, xmin, xmax, ybins, ymin, ymax);
    m_histList->Add(hist);

    return hist;
}

template <class THist2Dim>
THist2Dim * DV::PlotsManagerTool::bookTH2(const std::string & name, const std::string & title,
                    const int & xbins, const std::vector<double> & xbin_edges,
                    const int & ybins, const std::vector<double> & ybin_edges )
{
    const std::string hname(name + "_" + m_currentModule);
    checkHistName(hname);

    auto hist = new THist2Dim(hname.c_str(), title.c_str(), xbins, &xbin_edges[0], ybins, &ybin_edges[0]);
    m_histList->Add(hist);

    return hist;
}

// Declarations of used and valid specializations
template TH1C * DV::PlotsManagerTool::bookTH1<TH1C>(const std::string & name, const std::string & title,
        const int & xbin, const double & xmin, const double & xmax);
template TH1I * DV::PlotsManagerTool::bookTH1<TH1I>(const std::string & name, const std::string & title,
        const int & xbin, const double & xmin, const double & xmax);
template TH1F * DV::PlotsManagerTool::bookTH1<TH1F>(const std::string & name, const std::string & title,
        const int & xbin, const double & xmin, const double & xmax);
template TH1D * DV::PlotsManagerTool::bookTH1<TH1D>(const std::string & name, const std::string & title,
        const int & xbin, const double & xmin, const double & xmax);

template TH1C * DV::PlotsManagerTool::bookTH1<TH1C>(const std::string & name, const std::string & title,
        const int & xbins, const std::vector<double> & xbin_edges);
template TH1I * DV::PlotsManagerTool::bookTH1<TH1I>(const std::string & name, const std::string & title,
        const int & xbins, const std::vector<double> & xbin_edges);
template TH1F * DV::PlotsManagerTool::bookTH1<TH1F>(const std::string & name, const std::string & title,
        const int & xbins, const std::vector<double> & xbin_edges);
template TH1D * DV::PlotsManagerTool::bookTH1<TH1D>(const std::string & name, const std::string & title,
        const int & xbins, const std::vector<double> & xbin_edges);

template TH2C * DV::PlotsManagerTool::bookTH2<TH2C>(const std::string & name, const std::string & title,
        const int & xbin, const double & xmin, const double & xmax,
        const int & ybin, const double & ymin, const double & ymax);
template TH2I * DV::PlotsManagerTool::bookTH2<TH2I>(const std::string & name, const std::string & title,
        const int & xbin, const double & xmin, const double & xmax,
        const int & ybin, const double & ymin, const double & ymax);
template TH2F * DV::PlotsManagerTool::bookTH2<TH2F>(const std::string & name, const std::string & title,
        const int & xbin, const double & xmin, const double & xmax,
        const int & ybin, const double & ymin, const double & ymax);
template TH2D * DV::PlotsManagerTool::bookTH2<TH2D>(const std::string & name, const std::string & title,
        const int & xbin, const double & xmin, const double & xmax,
        const int & ybin, const double & ymin, const double & ymax);

template TH2C * DV::PlotsManagerTool::bookTH2<TH2C>(const std::string & name, const std::string & title,
        const int & xbins, const std::vector<double> & xbin_edges,
        const int & ybins, const std::vector<double> & ybin_edges);
template TH2I * DV::PlotsManagerTool::bookTH2<TH2I>(const std::string & name, const std::string & title,
        const int & xbins, const std::vector<double> & xbin_edges,
        const int & ybins, const std::vector<double> & ybin_edges);
template TH2F * DV::PlotsManagerTool::bookTH2<TH2F>(const std::string & name, const std::string & title,
        const int & xbins, const std::vector<double> & xbin_edges,
        const int & ybins, const std::vector<double> & ybin_edges);
template TH2D * DV::PlotsManagerTool::bookTH2<TH2D>(const std::string & name, const std::string & title,
        const int & xbins, const std::vector<double> & xbin_edges,
        const int & ybins, const std::vector<double> & ybin_edges);

bool DV::PlotsManagerTool::saveResults()
{
    m_outputfile->cd();

    TObject* h(0);

    TIter next(m_histList);
    while( (h=next()) )
    {
        h->Write();
    }

    m_outputfile->Close();

    // freeing memory
    delete m_outputfile;
    m_outputfile = 0;

    return true;
}

