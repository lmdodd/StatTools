# StatTools
StatTools interface for making plots and datacards 


# instuctions for download
```
cmsrel CMSSW_7_4_7
cd CMSSW_7_4_7/src/
cmsenv
git cms-init
git clone https://github.com/cms-analysis/HiggsAnalysis-CombinedLimit.git HiggsAnalysis/CombinedLimit
mkdir UWAnalysis
git clone git@github.com:lmdodd/StatTools.git UWAnalysis/StatTools 
```

to make plots make a folder in data 
```
cd UWAnalysis/StatTools/data
cd sm_higgs
./makeMuTauPlots
root -l -b -q makeTemplatePlotsMuTau.C #check the output directory here 
```

Combine hard to set up. Always make a clean build, as scram doesn't always see updates to src/LinkDef.h. See https://twiki.cern.ch/twiki/bin/viewauth/CMS/SWGuideHiggsAnalysisCombinedLimit for more
```
cd $CMSSW_BASE/src/HiggsAnalysis/CombinedLimit
git fetch origin
git checkout v6.2.1
scramv1 b clean; scramv1 b 
```


