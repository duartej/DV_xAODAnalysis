// DVTools_entries.cxx 
// Gaudi/Athena include(s):
#include "GaudiKernel/DeclareFactoryEntries.h"

// Local include(s):
#include "DVCuts/DVCuts.h"

DECLARE_NAMESPACE_TOOL_FACTORY( DV, DVCuts )

DECLARE_FACTORY_ENTRIES( DVCuts ) 
{
   DECLARE_NAMESPACE_TOOL( DV, DVCuts )
}
