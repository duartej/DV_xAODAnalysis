// DVTools_entries.cxx
// Gaudi/Athena include(s):
#include "GaudiKernel/DeclareFactoryEntries.h"

// Local include(s):
#include "DVTools/OverlapRemoval.h"
#include "DVTools/PhotonMatch.h"
#include "DVTools/PlotsManagerTool.h"
#include "DVTools/TrigMatch.h"

DECLARE_NAMESPACE_TOOL_FACTORY( DV, OverlapRemoval )
DECLARE_NAMESPACE_TOOL_FACTORY( DV, PhotonMatch )
DECLARE_NAMESPACE_TOOL_FACTORY( DV, PlotsManagerTool )
DECLARE_NAMESPACE_TOOL_FACTORY( DV, TrigMatch )

DECLARE_FACTORY_ENTRIES( DVTools )
{
    DECLARE_NAMESPACE_TOOL( DV, OverlapRemoval )
    DECLARE_NAMESPACE_TOOL( DV, PhotonMatch )
    DECLARE_NAMESPACE_TOOL( DV, PlotsManagerTool )
    DECLARE_NAMESPACE_TOOL( DV, TrigMatch )
}
