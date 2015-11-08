

#include "DVAnalysis/PlotsManager.h"

#include "TH1I.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TH2I.h"
#include "TH2F.h"
#include "TH2D.h"
#include "TList.h"
#include "TFile.h"
#include "TObject.h"

#include<stdexcept>

// this is needed to distribute the algorithm to the workers
ClassImp(DV::PlotsManager)

DV::PlotsManager::PlotsManager():
    m_histList(0)
{
    m_histList = new TList();
}

/*DV::PlotsManager::~PlotsManager()
{
    // la TList delete ? ojo, probablement la own of the THF es 
    // del fichero
}*/

void DV::PlotsManager::bookFile(const std::string & outfilename, const std::string & mode)
{
    m_outputfile = TFile::Open(outfilename.c_str(),mode.c_str());
    // FIXME: Check if it is oK!!
}

template <class THist1Dim,typename TYPE>
THist1Dim * DV::PlotsManager::bookTH1(const char * name, const char * title,
                    const int & xbin, const TYPE & xmin, const TYPE & xmax )
{
    const std::string hname(std::string(name)+"_"+m_currentModule);
    if( m_histList->FindObject(hname.c_str()) != 0 )
    {
        std::string message("PlotsManager ERROR: ""'"+std::string(name)+
                "' already used name ... Please change the name of this histo!!");
        throw std::runtime_error(message);//"Invalid histogram name");
    }

    m_histList->Add( new THist1Dim(hname.c_str(),title,xbin,xmin,xmax) );

    return static_cast<THist1Dim*>(m_histList->Last());
}

template <class THist2Dim,typename TYPE>
THist2Dim * DV::PlotsManager::bookTH2(const char * name, const char * title,
                    const int & xbin, const TYPE & xmin, const TYPE & xmax,
                    const int & ybin, const TYPE & ymin, const TYPE & ymax )
{
    const std::string hname(std::string(name)+"_"+m_currentModule);
    // FIXME:: Not needed unordered_set, exist a TList::Find(const char *name) function
    if( m_histList->FindObject(hname.c_str()) != 0 )
    {
        std::string message("PlotsManager ERROR: ""'"+std::string(name)+
                "' already used name ... Please change the name of this histo!!");
        throw std::runtime_error(message);//"Invalid histogram name");
    }

    m_histList->Add( new THist2Dim(hname.c_str(),title,xbin,xmin,xmax,ybin,ymin,ymax) );

    return static_cast<THist2Dim*>(m_histList->Last());
}

// Declarations of used and valid specializations
template TH1I * DV::PlotsManager::bookTH1<TH1I,int>(const char * name, const char * title,
        const int & xbin, const int & xmin, const int & xmax);
template TH1F * DV::PlotsManager::bookTH1<TH1F,float>(const char * name, const char * title,
        const int & xbin, const float & xmin, const float & xmax);
template TH1D * DV::PlotsManager::bookTH1<TH1D,double>(const char * name, const char * title,
        const int & xbin, const double & xmin, const double & xmax);

template TH2I * DV::PlotsManager::bookTH2<TH2I,int>(const char * name, const char * title,
        const int & xbin, const int & xmin, const int & xmax,
        const int & ybin, const int & ymin, const int & ymax);
template TH2F * DV::PlotsManager::bookTH2<TH2F,float>(const char * name, const char * title,
        const int & xbin, const float & xmin, const float & xmax,
        const int & ybin, const float & ymin, const float & ymax);
template TH2D * DV::PlotsManager::bookTH2<TH2D,double>(const char * name, const char * title,
        const int & xbin, const double & xmin, const double & xmax,
        const int & ybin, const double & ymin, const double & ymax);

bool DV::PlotsManager::saveResults()
{
    m_outputfile->cd();
    
    //TList * hists = this->getHists();
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

