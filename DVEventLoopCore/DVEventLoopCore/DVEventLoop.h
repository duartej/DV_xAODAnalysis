#ifndef XAOD_STANDALONE
#define XAOD_STANDALONE
#endif


#ifndef DVEVENTLOOP_H
#define DVEVENTLOOP_H

#include "DVAnalyses/AlgBase.h"

#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"

#include <EventLoop/Algorithm.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TFile.h>

#include<vector>
#include<string>

namespace DV
{
    class PlotsManagerTool;
}

class DVEventLoop : public EL::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
public:
  // float cutValue;

  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)
public:

  // this is a standard constructor
  DVEventLoop ();
  // and destructor
  ~DVEventLoop();

  //! set the maximum number of events to be processed
  void setEvtsMax(const int & evtmax){ m_evtsMax = evtmax; };
  //! name of the output file
  void setOutputFilename(const std::string & name) { m_outputFilename = name; };
  //! entry point of the available cut algorithms 
  EL::StatusCode addCutAlgs(const std::vector<std::string> & cut_algs);
  //! entry point for the analysis algorithms to be executed
  EL::StatusCode addAnalysisAlgs(const std::vector<std::string> & ana_names);

  // these are the functions inherited from Algorithm
  virtual EL::StatusCode setupJob (EL::Job& job);
  virtual EL::StatusCode fileExecute ();
  virtual EL::StatusCode histInitialize ();
  virtual EL::StatusCode addAnalysisAlgs ();
  virtual EL::StatusCode changeInput (bool firstFile);
  virtual EL::StatusCode initialize ();
  virtual EL::StatusCode execute ();
  virtual EL::StatusCode postExecute ();
  virtual EL::StatusCode finalize ();
  virtual EL::StatusCode histFinalize ();


  xAOD::TEvent *m_event;          //!
  unsigned int  m_eventCounter;   //!
  int m_evtsMax;                  //!
 

  std::vector<std::string> m_cutNames;
  std::vector<std::string> m_algNames;

  std::vector<DV::AlgBase*>* m_analysisAlgs;

  DV::PlotsManagerTool * m_plotmanager; //!

  std::string m_outputFilename;

  // this is needed to distribute the algorithm to the workers
  // 0: you dont want to use I/O for this class, right? FIXME
  ClassDef(DVEventLoop, 1);
};

#endif
