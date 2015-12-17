## DV_xADOAnalysis: Analysis framework for Run 2 DV analysis 

Dual-use framework intended for the multi-track and dilepton 
displaced-vertices analyses in ATLAS. The framework is able 
to read xAOD files, in particular samples created after a large-d0 
enhanced re-tracking and a posterior vertex reconstruction 
(using [RPVDispVrt](https://svnweb.cern.ch/trac/atlasoff/browser/Reconstruction/VKalVrt/RPVDispVrt) 
athena-package). The framework can be used both in an athena-based 
mode (__AthAnalysisBase__) or in standalone (__RootCore__/__EventLoop__).

The main use-case of this framework is to run one or several `analysis` 
algorithms, each of them have their own histograms and do their own event 
loop. The framework provides also a common set of tools, cuts utils to be used 
in the analyses and which harmonize them. 

If you want to implement your own analysis, take a look at the existing `analysis` 
(in __Analyses__ sub-package) and try to modify a similar one to your necessities.

* You can find detailed instructions and the documentation of the package in https://github.com/duartej/DV_xAODAnalysis/wiki
* Report any bug/request/... on  https://github.com/duartej/DV_xAODAnalysis/issues
* Questions/support/developement: 
  * Jordi Duarte-Campderros, jorge.duarte.campderos< >cern.ch
  * Dominik Krauss, krauss< >mpp.mpg.de
* Package based on Run1 package developed by nick.barlow< >cern.ch


### Quick start: standalone mode
 1. First time setup. Start from a fresh shell on lxplus:
    ```bash
    $ setupATLAS
    $ cd $TEST_AREA
    # download the package
    $ git clone https://github.com/duartej/DV_xAODAnalysis.git
    # setup the RootCore package, (you can find last available version on rcsetup -r)
    $ lsetup 'rcsetup Base,2.3.36'   
    $ rc find_packages
    # build
    $ rc compile

    ```
 2. Run the code: __runDVAna__ The executable needs a configuration python file populated with some variables, use the -h option of the executable to see the available variables. Therefore, populate the `analyses` list variable with your favorite analyses, give some input files and run it!
   ```bash
   $ runDVAna configfile.py

   ```   

    
### Quick start: athena-based mode
1. First time setup. Start from a fresh shell on lxplus:
    ```bash
    $ setupATLAS
    $ cd $TEST_AREA
    # download the package
    $ git clone https://github.com/duartej/DV_xAODAnalysis.git
    # setup the AhtAnalysis package, (you can find last available version on rcsetup -r)
    $ lsetup 'asetup AthAnalysisBase,2.3.36,here'   
    # build the subpackages, for each subpackage SUBPKG in Cuts, Tools, Analyses
    $ cd $SUBPKG/cmt
    $ cmt config && cmt make -j8

    ```
2. Run the code: prepare a `jobOptions.py` and schedule in the top sequence the DV analysis algorithms you want to run and/or associated to the `ToolSvc` the tools/cuts you want to use. Then, use the `athena.py` executable to rund it
   ```bash
   $ athena.py jobOptions.py 2>&1 | tee logout.out

   ```   

