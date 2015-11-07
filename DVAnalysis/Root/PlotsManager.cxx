

#include "DVAnalysis/PlotsManager.h"

#include "TH1F.h"
#include "TH2F.h"
#include "TList.h"
#include "TFile.h"

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


TH1F * DV::PlotsManager::bookTH1F(const char * name, const char * title,
                    const int & xbin, const float & xmin, const float & xmax )
{
    const std::string hname(std::string(name)+"_"+m_currentModule);
    if( m_histList->FindObject(hname.c_str()) != 0 )
    {
        std::string message("PlotsManager ERROR: ""'"+std::string(name)+
                "' already used name ... Please change the name of this histo!!");
        throw std::runtime_error(message);//"Invalid histogram name");
    }

    m_histList->Add( new TH1F(hname.c_str(),title,xbin,xmin,xmax) );

    return static_cast<TH1F*>(m_histList->Last());
}

TH2F * DV::PlotsManager::bookTH2F(const char * name, const char * title,
                    const int & xbin, const float & xmin, const float & xmax,
                    const int & ybin, const float & ymin, const float & ymax )
{
    const std::string hname(std::string(name)+"_"+m_currentModule);
    // FIXME:: Not needed unordered_set, exist a TList::Find(const char *name) function
    if( m_histList->FindObject(hname.c_str()) != 0 )
    {
        std::string message("PlotsManager ERROR: ""'"+std::string(name)+
                "' already used name ... Please change the name of this histo!!");
        throw std::runtime_error(message);//"Invalid histogram name");
    }

    m_histList->Add( new TH2F(hname.c_str(),title,xbin,xmin,xmax,ybin,ymin,ymax) );

    return static_cast<TH2F*>(m_histList->Last());
}


