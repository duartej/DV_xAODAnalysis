// -*- C++ -*-
//
// Package:    DV_xAODDVAnalysis/DVEventLoopCore
// Class:      DV::ToolInstantiator
// 
//
// Original Author: Jordi Duarte Campderros  
//         Created: Sun Nov 3 13:59:11 CET 2015
// 
// jordi.duarte.campderros@cern.ch
//

//#include "AsgTools/ToolStore.h"

#include "DVEventLoopCore/ToolInstantiator.h"

// Insert here the include of any tool you want to use with this 
// instantiator (what we should used? IAsgTools or AsgTools?)
#include "DVCuts/DVCuts.h"
#include "DVCuts/DiLepCosmics.h"
#include "DVCuts/DiLepDESD.h"
#include "DVCuts/DiLepDVCuts.h"
#include "DVCuts/EventCuts.h"
#include "DVCuts/ElecCuts.h"
#include "DVCuts/MuonCuts.h"


// system libraries
#include<iostream>
#include<stdexcept>

DV::ToolInstantiator * DV::ToolInstantiator::m_instance = nullptr;
std::vector<asg::IAsgTool *> DV::ToolInstantiator::m_tools;

DV::ToolInstantiator::~ToolInstantiator()
{
}

DV::ToolInstantiator *  DV::ToolInstantiator::getInstance()
{
    /*
    static ToolInstantiator instance;
    return &instance;*/
    if( m_instance == nullptr )
    {
        m_instance = new DV::ToolInstantiator();
    }
    
    return m_instance;
}

StatusCode DV::ToolInstantiator::instantiateTool(const std::string & cutname)
{
    // It is already present?  // with std::find and lambda func. ??
    for(auto & tool : m_tools)
    {
        std::cout << "instantiateTool:" << tool->name() << std::endl;
        if( cutname == tool->name() )
        {
            return StatusCode::SUCCESS;
        }
    }

    // Not found .. let's instantiate the tool 
    // Assuming cutname = "ToolType/ToolName"
    const size_t slashPos = cutname.find("/");
    // FIXME:: It's an error to have a different format, return Status/throw instance
    const std::string toolType(cutname.substr(0,slashPos));
    const std::string toolName(cutname.substr(slashPos+1));

    asg::IAsgTool * p = 0;
    
    // Algorithm cases: (Note the inclusion of the Interfaces for the ToolHandle case)
    if( toolType == "DV::DVCuts" or toolType == "DV::IDVCuts" )
    {
        p = new DV::DVCuts(toolName);
    }
    else if( toolType == "DV::DiLepCosmics" or toolType == "DV::IDiLepCosmics" )
    {
        p = new DV::DiLepCosmics(toolName);
    }
    else if( toolType == "DV::DiLepDESD" or toolType == "DV::IDiLepDESD" )
    {
        p = new DV::DiLepDESD(toolName);
    }
    else if( toolType == "DV::DiLepDVCuts" or toolType == "DV::IDiLepDVCuts" )
    {
        p = new DV::DiLepDVCuts(toolName);
    }
    else if( toolType == "DV::EventCuts" or toolType == "DV::IEventCuts" )
    {
        p = new DV::EventCuts(toolName);
    }
    else if( toolType == "DV::ElecCuts" or toolType == "DV::IElecCuts" )
    {
        p = new DV::ElecCuts(toolName);
    }
    else if( toolType == "DV::MuonCuts" or toolType == "DV::IMuonCuts" )
    {
        p = new DV::MuonCuts(toolName);
    }
    else
    {
        std::string message("DV::ToolInstantiator ERROR: the '"+toolType+"' Tool is not defined"\
                " in the framework. If it is a valid tool, then this class (DV::ToolInstantiator)"\
                " should be extended to include it. You should:\n"\
                "\t* include the header of your new algorithm in DVEventLoopCore/Root/ToolInstantiator.cxx\n"\
                "\t* include a new case inside the DV::ToolInstantiator::instantiateTool method"\
                "\nOr you can contact <jorge.duarte.campderros@cern.ch> ...");
         throw std::runtime_error(message);
    }

    m_tools.push_back(p);

    return StatusCode::SUCCESS;
}

const std::vector<std::string> DV::ToolInstantiator::getListOfTools() 
{
    std::vector<std::string> toolList;
    for(auto & tool : m_tools)
    {
        toolList.push_back(tool->name());
    }

    return toolList;
}
