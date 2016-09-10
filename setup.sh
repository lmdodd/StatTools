#!/bin/bash

pushd $CMSSW_BASE/src

cp UWAnalysis/StatTools/recipe/HTT.cpp CombineHarvester/CombineTools/bin/
cp UWAnalysis/StatTools/recipe/XTT.cpp CombineHarvester/CombineTools/bin/
cp UWAnalysis/StatTools/recipe/XTTBoost.cpp CombineHarvester/CombineTools/bin/
cp UWAnalysis/StatTools/recipe/BuildFile.xml CombineHarvester/CombineTools/bin/
cp UWAnalysis/StatTools/recipe/xtt_monoH_*txt CombineHarvester/CombineTools/input/xsecs_brs/

scram b -j8
