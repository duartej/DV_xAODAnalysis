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

#include "AsgTools/MsgStream.h"

#include "DVEventLoopCore/ToolInstantiator.h"

// system libraries
#include<iostream>
#include<stdexcept>

/*
 * Insert here the include of any tool you want to use with this instantiator
 */

// Atlas tools
#include "ElectronPhotonSelectorTools/AsgElectronLikelihoodTool.h"
#include "MuonSelectorTools/MuonSelectionTool.h"
#include "TrigConfxAOD/xAODConfigTool.h"
#include "TrigDecisionTool/TrigDecisionTool.h"
#include "TrigMuonMatching/TrigMuonMatching.h"

// DVCuts
#include "DVCuts/DiLepCosmics.h"
#include "DVCuts/DiLepDESD.h"
#include "DVCuts/DiLepDVCuts.h"
#include "DVCuts/DVCuts.h"
#include "DVCuts/ElecCuts.h"
#include "DVCuts/EventCuts.h"
#include "DVCuts/MuonCuts.h"

// DVTools
#include "DVTools/OverlapRemoval.h"
#include "DVTools/PhotonMatch.h"
#include "DVTools/TrigMatch.h"

DV::ToolInstantiator * DV::ToolInstantiator::m_instance = nullptr;
std::vector<asg::IAsgTool *> DV::ToolInstantiator::m_atlas_tools;
std::vector<asg::IAsgTool *> DV::ToolInstantiator::m_dv_tools;
std::vector<bool> DV::ToolInstantiator::m_atlas_init;

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

bool DV::ToolInstantiator::findTool(const std::vector<asg::IAsgTool *> & tool_store,
                                    const std::string & type_and_name)
{
    for(auto & tool : tool_store)
    {
        if( type_and_name == tool->name() )
        {
            return true;
        }
    }

    return false;
}

std::pair<std::string, std::string> DV::ToolInstantiator::GetTypeName(const std::string & type_and_name)
{
    const std::size_t slashPos = type_and_name.find("/");

    if(slashPos != std::string::npos)
    {
        std::string toolType(type_and_name.substr(0, slashPos));
        std::string toolName(type_and_name.substr(slashPos+1));

        if(!toolType.empty() && !toolName.empty())
        {
            return std::make_pair(toolType, toolName);
        }
    }

    // invalid type_and_name
    std::string message("DV::ToolInstantiator ERROR: the tool name '"+type_and_name+"' is invalid!"\
                        "Tool names have to have the following format: 'Type/Name'");
    throw std::runtime_error(message);
}

StatusCode DV::ToolInstantiator::instantiateAtlasTool(const std::string & type_and_name)
{
    // type_and_name = "toolType/toolName"
    std::string toolType;
    std::string toolName;
    std::tie(toolType, toolName) = m_instance->GetTypeName(type_and_name);

    // Tool already present?
    if(m_instance->findTool(m_atlas_tools, toolName))
    {
        return StatusCode::SUCCESS;
    }

    asg::IAsgTool * p = 0;
    bool init = true; // false: tool is initialized by the DV tool itself

    if( toolType == "AsgElectronLikelihoodTool" or toolType == "IAsgElectronLikelihoodTool" )
    {
        p = new AsgElectronLikelihoodTool(toolName);
        init = false;
    }
    else if( toolType == "CP::MuonSelectionTool" or toolType == "CP::IMuonSelectionTool" )
    {
        p = new CP::MuonSelectionTool(toolName);
        init = false;
    }
    else if( toolType == "TrigConf::xAODConfigTool" or toolType == "TrigConf::IxAODConfigTool" )
    {
        p = new TrigConf::xAODConfigTool(toolName);
    }
    else if( toolType == "Trig::TrigDecisionTool" or toolType == "Trig::ITrigDecisionTool" )
    {
        p = new Trig::TrigDecisionTool(toolName);
    }
    else if( toolType == "Trig::TrigMuonMatching" or toolType == "Trig::ITrigMuonMatching" )
    {
        p = new Trig::TrigMuonMatching(toolName);
    }
    else
    {
        std::string message("DV::ToolInstantiator ERROR: the '"+toolType+"' atlas tool is not defined"\
                " in DVEventLoopCore/Root/ToolInstantiator.cxx. You should:\n"\
                "\t* include the header of this atlas tool in DVEventLoopCore/Root/ToolInstantiator.cxx\n"\
                "\t* include a new case inside the DV::ToolInstantiator::instantiateAtlasTool method"\
                "\nOr you can contact <jorge.duarte.campderros@cern.ch> ...");
        throw std::runtime_error(message);
    }

    std::cout << "ToolInstantiator::instantiateTool: '" << toolName
        << "' new instance" << std::endl;
    m_atlas_tools.push_back(p);
    m_atlas_init.push_back(init);

    return StatusCode::SUCCESS;
}

StatusCode DV::ToolInstantiator::instantiateDVTool(const std::string & type_and_name)
{
    // type_and_name = "toolType/toolName"
    std::string toolType;
    std::string toolName;
    std::tie(toolType, toolName) = m_instance->GetTypeName(type_and_name);

    // Tool already present?
    if(m_instance->findTool(m_dv_tools, toolName))
    {
        return StatusCode::SUCCESS;
    }

    asg::IAsgTool * p = 0;
    std::vector<std::string> atlas_tools;

    // DV tools
    // Algorithm cases: (Note the inclusion of the Interfaces for the ToolHandle case)
    if( toolType == "DV::DiLepCosmics" or toolType == "DV::IDiLepCosmics" )
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
    else if( toolType == "DV::DVCuts" or toolType == "DV::IDVCuts" )
    {
        p = new DV::DVCuts(toolName);
    }
    else if( toolType == "DV::ElecCuts" or toolType == "DV::IElecCuts" )
    {
        p = new DV::ElecCuts(toolName);

        atlas_tools.push_back("AsgElectronLikelihoodTool/DVElectronLikelihoodTool");
    }
    else if( toolType == "DV::EventCuts" or toolType == "DV::IEventCuts" )
    {
        p = new DV::EventCuts(toolName);

        atlas_tools.push_back("TrigConf::xAODConfigTool/TrigConf::xAODConfigTool");
        atlas_tools.push_back("Trig::TrigDecisionTool/TrigDecisionTool");
    }
    else if( toolType == "DV::MuonCuts" or toolType == "DV::IMuonCuts" )
    {
        p = new DV::MuonCuts(toolName);

        atlas_tools.push_back("CP::MuonSelectionTool/DVMuonSelectionTool");
    }
    else if( toolType == "DV::OverlapRemoval" or toolType == "DV::IOverlapRemoval" )
    {
        p = new DV::OverlapRemoval(toolName);
    }
    else if( toolType == "DV::PhotonMatch" or toolType == "DV::IPhotonMatch" )
    {
        p = new DV::PhotonMatch(toolName);
    }
    else if( toolType == "DV::TrigMatch" or toolType == "DV::ITrigMatch" )
    {
        p = new DV::TrigMatch(toolName);

        atlas_tools.push_back("TrigConf::xAODConfigTool/TrigConf::xAODConfigTool");
        atlas_tools.push_back("Trig::TrigDecisionTool/TrigDecisionTool");
        atlas_tools.push_back("Trig::TrigMuonMatching/TrigMuonMatching");
    }
    else
    {
        std::string message("DV::ToolInstantiator ERROR: the '"+toolType+"' tool is not defined"\
                " in the framework. If it is a valid tool, then this class (DV::ToolInstantiator)"\
                " should be extended to include it. You should:\n"\
                "\t* include the header of your new algorithm in DVEventLoopCore/Root/ToolInstantiator.cxx\n"\
                "\t* include a new case inside the DV::ToolInstantiator::instantiateDVTool method"\
                "\nOr you can contact <jorge.duarte.campderros@cern.ch> ...");
        throw std::runtime_error(message);
    }

    std::cout << "ToolInstantiator::instantiateTool: '" << type_and_name
        << "' new instance" << std::endl;
    m_dv_tools.push_back(p);

    // instantiate atlas tools
    for(const std::string& tool: atlas_tools)
    {
        if(instantiateAtlasTool(tool).isFailure())
        {
            return StatusCode::FAILURE;
        }
    }

    return StatusCode::SUCCESS;
}

const std::vector<std::string> DV::ToolInstantiator::getListOfAtlasTools()
{
    std::vector<std::string> toolList;
    for(auto & tool : m_atlas_tools)
    {
        toolList.push_back(tool->name());
    }

    return toolList;
}

const std::vector<std::string> DV::ToolInstantiator::getListOfDVTools()
{
    std::vector<std::string> toolList;
    for(auto & tool : m_dv_tools)
    {
        toolList.push_back(tool->name());
    }

    return toolList;
}

StatusCode DV::ToolInstantiator::initializeAtlasTools()
{
    for(std::size_t t = 0; t < m_atlas_tools.size(); t++)
    {
        // some atlas tools were initialized by a DV tool
        if(!m_atlas_init.at(t)) continue;

        if(m_atlas_tools.at(t)->initialize().isFailure())
        {
            return StatusCode::FAILURE;
        }
    }

    return StatusCode::SUCCESS;
}

StatusCode DV::ToolInstantiator::initializeDVTools()
{
    for(auto & tool : m_dv_tools)
    {
        if(tool->initialize().isFailure())
        {
            return StatusCode::FAILURE;
        }
    }

    return StatusCode::SUCCESS;
}
