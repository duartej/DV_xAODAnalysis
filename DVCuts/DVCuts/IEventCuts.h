#ifndef DV_IEVENTCUTS_H
#define DV_IEVENTCUTS_H

// STL
#include <string>
#include <vector>

// Framework includes
#include "AsgTools/IAsgTool.h"

// xAOD
#include "xAODEventInfo/EventInfo.h"
#include "xAODTracking/VertexContainer.h"

namespace DV
{
    class IEventCuts : public virtual asg::IAsgTool
    {
        ASG_TOOL_INTERFACE(DV::IEventCuts)

        public:
            virtual void SetTriggers(const std::vector<std::string>& trigNames) = 0;

            virtual const xAOD::Vertex* GetPV(const xAOD::VertexContainer& pvc) const = 0;

            virtual bool PassGRL(const xAOD::EventInfo& ei) const = 0;
            virtual bool PassEventCleaning(const xAOD::EventInfo& ei) const = 0;
            virtual bool PassTrigger() const = 0;
            virtual bool PassPVCuts(const xAOD::VertexContainer& pvc) const = 0;
    };
}

#endif
