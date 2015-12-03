#ifndef DV_ITRIGMATCH_H
#define DV_ITRIGMATCH_H

// STL
#include <string>

// Framework includes
#include "AsgTools/IAsgTool.h"

// xAOD
#include "xAODEgamma/Egamma.h"
#include "xAODMuon/Muon.h"

namespace DV
{
    class ITrigMatch : public virtual asg::IAsgTool
    {
        ASG_TOOL_INTERFACE(DV::ITrigMatch)
        public:
            virtual bool Match(const xAOD::Egamma& eg, const std::string& trigger) const = 0;
            virtual bool Match(const xAOD::Muon& mu, const std::string& trigger) const = 0;
     };
}

#endif
