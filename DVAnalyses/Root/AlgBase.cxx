
#include "DVAnalyses/AlgBase.h"
#include<stdexcept>

void DV::AlgBase::checkCutAvailability(const std::string & cut,const std::string & myname)
{
    // FIXME:: There is a way to know the name of the algorithm..?
    if( this->m_cutsMap.find(cut) == this->m_cutsMap.end() )
    {
        //FIXME:: TO BE REMOVED THIS COMMENT: this is ROOT: TError.h
        //Error(myname.c_str(),std::string("Not attached the '"+cut+"' CutsBase algorithm!! Exiting").c_str());
        // QUE HAY QUE HACER PARA SOLUCIONARLO? ESCRIBELO!!
        const std::string meserr(myname+" ERROR: Not attached the '"+cut+"' CutsBase algorithm!! Exiting");
        throw std::runtime_error(meserr);
    }
}
