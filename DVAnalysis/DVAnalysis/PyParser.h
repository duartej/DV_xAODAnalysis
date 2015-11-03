// -*- C++ -*-
//
// Package:    DV_xAODAnalysis/DVAnalysis
// Class:      PyParser
// 
/**\class PyParser PyParser.h DV_xAODAnalysis/DVAnalysis/Root/PyParser.cxx
 Description: Builder to create DVAnaBase 
              (MTAtom concrete classes).
 Implementation: The Builder is called by the DVEventLoop class (addAnalysisAlgs method)
              acts as a client. The class calls the instances of DVAnaBase
              selected by the user. Return a generic DVAnaBase.     
*/
//
// Original Author: Jordi Duarte Campderros  
//         Created:  Sun Nov 01 13:59:11 CET 2015
// 
// jordi.duarte.campderros@cern.ch
//
//


#ifndef XAOD_STANDALONE
#define XAOD_STANDALONE
#endif


#ifndef PyParser_H
#define PyParser_H
//#ifdef __cplusplus
//extern "C" {
//#endif

//#include "Python.h"
//#include "boost/python.hpp"

#include<map>
#include<string>
#include<vector>

typedef struct _object PyObject;

class PyParser
{
    public:
        PyParser(const std::string & filename);
        ~PyParser();       //! Getters from key
        //! Check the key exist
        bool Check(const std::string & key) const;
        //! Getters from key
        template <class T>
            T Get(const std::string & key);
        //! Print the parsed content
        void Dump();

    private:
        PyParser() { };
        //! Specialized getters from keys
        //int getint(const std::string & key,const int & index = -1) const;
        //float getint(const std::string & key,const int & index = -1) const;
        //std::string getint(const std::string & key, const int & index = -1) const;
        //! Check the type
        //const std::string getType(const PyObject * const _o) const;
        std::string _filename;
        //! Name of the configurable and values
        std::map<std::string,PyObject*> _configMap;
};

//#ifdef __cplusplus
//}
//#endif

#endif
