// -*- C++ -*-
//
// Package:    DV_xAODAnalysis/DVAnalysis
// Class:      DVToolInstantiator
//
/**\class DVToolInstantiator DVToolInstantiator.h DV_xAODAnalysis/DVAnalysis/Root/DVToolInstantiator.cxx
 Description: a container of tools and a centralized place (builder) to
        create the tool instances
 Implementation: The class inherits from DV::AlgBase in order to be treat by
        the DVEventLoop (the main actor) identically as a regular algorithm,
        therefore taking care of initialization and destruction, and providing
        event-wise data from the input file.
        This class is a container of cut algorithms which are going to be used
        by the DV::AlgBase algorithms scheduled. All the algorithms (clients)
        shared the same instance of a particular cut.
        The main method, DVToolInstantiator::add(string), initializes the
        concrete DV::ToolStore given the name of the class.
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

//#include "AsgTools/AsgTool.h"
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
            static StatusCode instantiateTool(const std::string & type_and_name);
            //! Get the "ToolType/ToolName" list of available tools
            static const std::vector<std::string> getListOfTools();
            //! Initialize an instantiated tool
            static StatusCode initializeTool(const std::string & type_and_name);

        private:
            ToolInstantiator() { ; };
            // Not implemented copy methods
            ToolInstantiator(ToolInstantiator const & copy) = delete;
            ToolInstantiator & operator=(ToolInstantiator const & copy) = delete;

            static ToolInstantiator * m_instance;
            //! list of tools (to be decided...)
            //std::vector<asg::AsgTool *> m_tools;
            static std::vector<asg::IAsgTool *> m_tools;
    };
}
#endif
