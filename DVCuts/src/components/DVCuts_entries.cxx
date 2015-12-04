// DVCuts_entries.cxx
// Gaudi/Athena include(s):
#include "GaudiKernel/DeclareFactoryEntries.h"

// Local include(s):
#include "DVCuts/DVCuts.h"
#include "DVCuts/ElecCuts.h"
#include "DVCuts/EventCuts.h"
#include "DVCuts/MuonCuts.h"
#include "DVCuts/DiLepCosmics.h"
#include "DVCuts/DiLepDESD.h"
#include "DVCuts/DiLepDVCuts.h"

DECLARE_NAMESPACE_TOOL_FACTORY( DV, DVCuts )
DECLARE_NAMESPACE_TOOL_FACTORY( DV, ElecCuts )
DECLARE_NAMESPACE_TOOL_FACTORY( DV, EventCuts )
DECLARE_NAMESPACE_TOOL_FACTORY( DV, MuonCuts )
DECLARE_NAMESPACE_TOOL_FACTORY( DV, DiLepCosmics )
DECLARE_NAMESPACE_TOOL_FACTORY( DV, DiLepDESD )
DECLARE_NAMESPACE_TOOL_FACTORY( DV, DiLepDVCuts )

DECLARE_FACTORY_ENTRIES( DVCuts )
{
    DECLARE_NAMESPACE_TOOL( DV, DVCuts )
    DECLARE_NAMESPACE_TOOL( DV, ElecCuts )
    DECLARE_NAMESPACE_TOOL( DV, EventCuts )
    DECLARE_NAMESPACE_TOOL( DV, MuonCuts )
    DECLARE_NAMESPACE_TOOL( DV, DiLepCosmics )
    DECLARE_NAMESPACE_TOOL( DV, DiLepDESD )
    DECLARE_NAMESPACE_TOOL( DV, DiLepDVCuts )
}
