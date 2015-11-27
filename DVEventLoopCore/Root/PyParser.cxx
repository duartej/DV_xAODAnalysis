// -*- C++ -*-
//
// Package:    DV_xAODDVAnalysis/DVAnalysis
// Class:      PyParser
// 
//
// Original Author: Jordi Duarte Campderros  
//         Created: Sun Nov 1 13:59:11 CET 2015
// 
// jordi.duarte.campderros@cern.ch
//
// first of all, the python header (includes som pre-processor defs.)
#include"Python.h"
// system libraries
#include<iostream>
#include<iomanip>
#include<string>
#include<map>
#include<stdexcept>

#include "boost/variant.hpp"

#include "DVEventLoopCore/PyParser.h"


// The known list of analysis, include your DV::AlgBase here...
//const std::vector<std::string> PyParser::_validNames{"TrkBasicPlots","BasicPlots","LowMassDVEff"};

PyParser::PyParser(const std::string & inputfile):
    _filename(inputfile)
{
    // Initialize interpreter (to be closed at the destructor
    // as we used some of the PyObject along the life of this parser
    Py_Initialize();

    // Strip the .py suffix if any
    int afterdot = inputfile.find_last_of("."); 
    std::string file_as_module_name = _filename.substr(0, afterdot);
    
    // retrieve the configuration file (python module) 
    PyObject * cfg_module = PyImport_ImportModule(file_as_module_name.c_str());
    if(cfg_module == NULL)
    {
        std::string message("\033[1;31mrunDVAna ERROR\033[1;m not found config file '"+inputfile+"'");
        throw std::runtime_error(message);
    }
    PyObject * cfg_dict   = PyModule_GetDict(cfg_module);

    // Get the names of the configurables keys
    // and populates the data-members
    PyObject * keys = PyDict_Keys(cfg_dict);
    for(Py_ssize_t i = 0; i < PyList_Size(keys); ++i)
    {
        PyObject * py_keystr = PyList_GetItem(keys,i);
        std::string keystr(PyString_AsString(py_keystr));
        _configMap[keystr] = PyDict_GetItem(cfg_dict,py_keystr);
    }
    Py_DECREF(keys);
}

//! Destructor, finalize interpreter
PyParser::~PyParser(){ Py_Finalize(); }

bool PyParser::Check(const std::string & key) const
{
    if( _configMap.find(key) == _configMap.end() )
    {
        return false;
    }
    
    return true;
}

template <class T>
T PyParser::Get(const std::string & key) 
{
    // Checking for existence
    if( ! Check(key) )
    {
        std::string message("\033[1;31mPyParser ERROR\033[1;m The key"\
               " '"+key+"' was not defined in the configuration file"\
               " '"+_filename+"'");
        throw std::runtime_error(message);
    }

    PyObject * _o = _configMap[key];

    boost::variant<int,float,std::string,std::vector<std::string>,
        std::map<std::string,std::vector<std::string> > > configdata;
    // Checking the type
    if( PyInt_Check(_o) )
    {
        configdata = static_cast<int>(PyInt_AsLong(_o));
    }
    else if( PyFloat_Check(_o) )
    {
        configdata = static_cast<float>(PyFloat_AsDouble(_o));
    }
    else if( PyString_Check(_o) )
    {
        configdata = static_cast<std::string>(PyString_AsString(_o));
    }
    else if( PyList_Check(_o) )
    {
        // By the moment only accepted vector of strings
        // Probably it is easy to call here a helper function
        // to deal with the initialization of the vector using
        // a boost::variant or just using anytype = boost::variant<...>
        std::vector<std::string> values;
        for(Py_ssize_t i = 0; i < PyList_Size(_o); ++i)
        {
            PyObject * _objstr = PyList_GetItem(_o,i);
            std::string strcontent(PyString_AsString(_objstr));
            values.push_back(strcontent);
        }
        configdata = values;
    }
    else if( PyDict_Check(_o) )
    {
        // By the moment only accepted key and values of strings
        // Probably it is easy to call here a helper function
        // to deal with the initialization of the vector using
        // a boost::variant or just using anytype = boost::variant<...>
        std::map<std::string,std::vector<std::string> > keyvalues;
        PyObject *_dictkey   = 0;
        PyObject *_dictvalue = 0;
        Py_ssize_t pos  = 0;
        while(PyDict_Next(_o, &pos, &_dictkey, &_dictvalue)) 
        {
            if( ! PyList_Check( _dictvalue ) )
            {   
                std::string message("\033[1;31mrunDVAna ERROR\033[1;m"\
                       " Not Implemented type for values in the dictionary");
                throw std::runtime_error(message);
            }
            std::vector<std::string> _dictvalueslist;
            for(Py_ssize_t i = 0; i < PyList_Size(_dictvalue); ++i)
            {
                PyObject * _objstr = PyList_GetItem(_dictvalue,i);
                std::string strcontent(PyString_AsString(_objstr));
                _dictvalueslist.push_back(strcontent);
            }
            std::string key_str_content(PyString_AsString(_dictkey));
            keyvalues[key_str_content] = _dictvalueslist;
        }
        configdata = keyvalues;
    }
    else
    {
        std::string message("\033[1;31mrunDVAna ERROR\033[1;m Not Implemented type for "\
                "'"+key+"' value");
        throw std::runtime_error(message);
    }
    return boost::get<T>(configdata);
}

// Declaration of the used types
template int PyParser::Get<int>(const std::string &);
template float PyParser::Get<float>(const std::string &);
template std::string PyParser::Get<std::string>(const std::string &);
template std::vector<std::string> PyParser::Get<std::vector<std::string> >(const std::string &);
template std::map<std::string,std::vector<std::string> > PyParser::Get<std::map<std::string,
         std::vector<std::string> > >(const std::string &);

void PyParser::Dump()
{
    std::cout << "\033[1;34mPyParser::INFO\033[1;m filename:'"<< _filename 
        << "'" << std::endl;
    std::cout << "  ::" << std::setw(15) << "KEY" << std::setw(15) << " VALUE " <<  std::endl;
    for(auto & p: _configMap)
    {
        if( p.first.find("__",0,2) != std::string::npos )
        {
            continue;
        }
        // FIXME:: What happens with the list representation? Crashes...
        PyObject * value_repr = PyObject_Repr(p.second);
        if( value_repr == NULL )
        {
            std::cerr << "\033[1;31PyParder::ERROR\033[1;m Problems extracting "
                << "the value of " << p.first << std::endl;
            continue;
        }

        std::cout << "  ::" << std::setw(15) << p.first << "  "
            << PyString_AsString(value_repr) << std::endl;
        Py_DECREF(value_repr);
    }       
}
