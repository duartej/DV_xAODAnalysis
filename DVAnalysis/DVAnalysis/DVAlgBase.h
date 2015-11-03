#ifndef DVAlgBase_h
#define DVAlgBase_h

#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
#include "TList.h"
#include "TH1F.h"
#include "TH2F.h"
#include <vector>

class DVAlgBase 
{
    public:
        virtual ~DVAlgBase() { };
        virtual void init() {bookHists();};
        //! book histograms at DVEventLoop::histInitialize
        virtual void bookHists() = 0;
        //! to be called byt the DVEventLoop::histFinalize
        virtual TList* getHists() = 0;
        virtual void execute(xAOD::TEvent*)=0;        
        virtual void finalize() =0;        
        virtual void setEvent(xAOD::TEvent *event) { m_event=event; };
    protected:
        xAOD::TEvent * m_event;          //!
        unsigned int   m_eventCounter;   //!
        //////  std::vector<TH1*> m_histList;   //!
};
#endif