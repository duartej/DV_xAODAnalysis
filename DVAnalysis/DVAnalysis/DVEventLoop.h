#ifndef XAOD_STANDALONE
#define XAOD_STANDALONE
#endif


#ifndef DVAna_DVEventLoop_H
#define DVAna_DVEventLoop_H


#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"

#include <EventLoop/Algorithm.h>
#include "DVAnalysis/DVAlgBase.h"
#include <TH1F.h>
#include <TH2F.h>
#include <TFile.h>

#include<vector>
#include<string>

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
  // Tree *myTree; //!
  // TH1 *myHist; //!



  // this is a standard constructor
  DVEventLoop ();
  // and destructor
  ~DVEventLoop();

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
 

  std::vector<std::string> m_cutNames;
  std::vector<std::string> m_algNames;

  std::vector<DVAlgBase*>* m_analysisAlgs;

  TFile* m_outputFile; //!

  // this is needed to distribute the algorithm to the workers
  ClassDef(DVEventLoop, 1);
};

#endif
