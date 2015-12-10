// -*- C++ -*-
//
// Package:    DV_xAODAnalysis/DVAnalysis
// Class:      DVToolInstantiator
//
/**\class DVToolInstantiator DVToolInstantiator.h DV_xAODAnalysis/DVAnalysis/Root/DVToolInstantiator.cxx
 Description: a container of tools and a centralized place (builder) to
        create the tool instances
 Implementation:
        This class is a container of tools which are going to be used
        by the DV::AlgBase algorithms scheduled. All the algorithms (clients)
        shared the same instance of a particular tool.
        The main method, DVToolInstantiator::instantiateTool(string), initializes the
        concrete tool given the name of the class.
*/
//
// Original Author: Jordi Duarte Campderros
//         Created:  Sun Nov 03 15:32:34 CET 2015
//
// jordi.duarte.campderros@cern.ch
//
//
#ifndef DV_TOOLINSTANTIATOR_H
#define DV_TOOLINSTANTIATOR_H

#include "AsgTools/IAsgTool.h"

#include<string>
#include<vector>

namespace DV
{
    class ToolInstantiator
    {
        public:
            ~ToolInstantiator();
            // the unique instance
            static ToolInstantiator * getInstance();
            //! try to instantiate the tool given "ToolType/ToolName".
            static StatusCode instantiateAtlasTool(const std::string & type_and_name);
            static StatusCode instantiateDVTool(const std::string & type_and_name);
            //! Get the "ToolType/ToolName" list of available tools
            static const std::vector<std::string> getListOfAtlasTools();
            static const std::vector<std::string> getListOfDVTools();
            //! Initialize tools
            static StatusCode initializeAtlasTools();
            static StatusCode initializeDVTools();

        private:
            ToolInstantiator() { ; };
            // Not implemented copy methods
            ToolInstantiator(ToolInstantiator const & copy) = delete;
            ToolInstantiator & operator=(ToolInstantiator const & copy) = delete;

            inline bool findTool(const std::vector<asg::IAsgTool *> & tool_store,
                                 const std::string & type_and_name);
            inline std::pair<std::string, std::string> GetTypeName(const std::string & type_and_name);

            static ToolInstantiator * m_instance;

            static std::vector<asg::IAsgTool *> m_atlas_tools;
            static std::vector<asg::IAsgTool *> m_dv_tools;

            static std::vector<bool> m_atlas_init;
    };
}
#endif
