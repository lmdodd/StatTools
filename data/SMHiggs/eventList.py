#!/bin/env python

import sys
from ROOT import TFile, TTree

from optparse import OptionParser

parser = OptionParser()

parser.add_option("-c", "--cut", dest="cut",
                          default='pt_1>23&&pt_2>20&&npv>0&&diLeptons==0&&iso04_1<0.15&&byTightIsolationMVArun2v1DBoldDMwLT_2>0.5&&againstMuonTight3_2>0&&againstElectronVLooseMVA6_2>0&&diLeptons==0&&mtRecoil_1<50&&charge==0&&pt_2>30&&njets==2&&vbfMass>300',
                          #default='pt_1>23&&pt_2>20&&npv>0&&diLeptons==0&&iso04_1<0.15&&byTightIsolationMVArun2v1DBoldDMwLT_2>0.5&&againstMuonTight3_2>0&&againstElectronVLooseMVA6_2>0&&diLeptons==0&&(HLT_IsoMu22_v_fired>0||HLT_IsoTkMu22_v_fired>0)&&mtRecoil_1<50&&charge==0&&pt_2>30&&njets==2&&vbfMass>300&&evt==180259',
                                            help='cut to define passing events')

parser.add_option("-t", "--tree", dest="tree",
                          default='muTauEventTree/eventTree',
                                            help='name of the tree')

(options,args) = parser.parse_args()
if len(args)!=1:
        print 'provide input root file'
        sys.exit(1)

file = TFile( args[0] )
treeName = options.tree
cut = options.cut

tree = file.Get(treeName)
tree.SetScanField(0)
tree.Scan('evt:m_sv:vbfMass:__WEIGHT__*12900:topWeight:gen_match_2', cut, "colsize=15")
