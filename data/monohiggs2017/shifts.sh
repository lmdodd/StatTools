#!/bin/sh
python makeShifts.py datacards/xtt_mt.inputs-13TeV-mt.root MuTau/XTT mt_inclusive 'TT' mtTES t
python makeShifts.py datacards/xtt_mt.inputs-13TeV-mt.root MuTau/XTT mt_inclusive 'QCD' mtTES t
python makeShifts.py datacards/xtt_mt.inputs-13TeV-mt.root MuTau/XTT mt_inclusive 'W' mtTES t
python makeShifts.py datacards/xtt_mt.inputs-13TeV-mt.root MuTau/XTT mt_inclusive 'ZTT' mtTES t
python makeShifts.py datacards/xtt_mt.inputs-13TeV-mt.root MuTau/XTT mt_inclusive 'SMH' mtTES t
python makeShifts.py datacards/xtt_mt.inputs-13TeV-mt.root MuTau/XTT mt_inclusive 'Zprime1200A300' mtTES t

python makeShifts.py datacards/xtt_mt.inputs-13TeV-mt.root MuTau/XTT mt_inclusive 'TT' mtUES m
python makeShifts.py datacards/xtt_mt.inputs-13TeV-mt.root MuTau/XTT mt_inclusive 'QCD' mtUES m
python makeShifts.py datacards/xtt_mt.inputs-13TeV-mt.root MuTau/XTT mt_inclusive 'W' mtUES m
python makeShifts.py datacards/xtt_mt.inputs-13TeV-mt.root MuTau/XTT mt_inclusive 'ZTT' mtUES m
python makeShifts.py datacards/xtt_mt.inputs-13TeV-mt.root MuTau/XTT mt_inclusive 'SMH' mtUES m
python makeShifts.py datacards/xtt_mt.inputs-13TeV-mt.root MuTau/XTT mt_inclusive 'Zprime1200A300' mtUES m
