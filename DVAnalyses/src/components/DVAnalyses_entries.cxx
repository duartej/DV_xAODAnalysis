// DVAnalyses_entries.cxx 
// Gaudi/Athena include(s):
#include "GaudiKernel/DeclareFactoryEntries.h"

// Local include(s):
//#include "DVAnalyses/AlgBase.h"
//#include "DVAnalyses/BasicPlots.h"
#include "DVAnalyses/TrkBasicPlots.h"

//DECLARE_NAMESPACE_ALGORITHM_FACTORY( DV, AlgBase )
//DECLARE_NAMESPACE_ALGORITHM_FACTORY( DV, BasicPlots )
DECLARE_NAMESPACE_ALGORITHM_FACTORY( DV, TrkBasicPlots )

DECLARE_FACTORY_ENTRIES( DVAnalyses ) 
{
   //DECLARE_NAMESPACE_ALGORITHM( DV, AlgBase )
   DECLARE_NAMESPACE_ALGORITHM( DV, TrkBasicPlots )
}
