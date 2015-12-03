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
#include "DVCuts/IDVCuts.h"  
#include "DVCuts/DVCuts.h"

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
    
    // Algorithm cases
    if( toolType == "DV::DVCuts" )
    {
        p = new DV::DVCuts(toolName);
    }
    else if( toolType == "DV::IDVCuts" )
    {
        p = new DV::DVCuts(toolName);
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
