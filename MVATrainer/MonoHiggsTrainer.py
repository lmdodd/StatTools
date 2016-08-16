import glob
from itertools import product, combinations_with_replacement

import ROOT

from DevTools.MVATrainer.Trainer import Trainer
from DevTools.Plotter.NtupleWrapper import NtupleWrapper
from DevTools.Plotter.higgsUtilities import *
from DevTools.Utilities.utilities import ZMASS

class MonoHiggsTrainer(Trainer):
    '''Mono Higgs MVA Trainer'''
    '''Thanks Devin! '''
    def __init__(self,**kwargs):
        inputTreeName = kwargs.pop('inputTreeName','muTauEventTree/eventTree')
        mass = kwargs.pop('mass',500)
        nTaus = kwargs.pop('nTaus',0)
        super(Hpp4lTrainer,self).__init__(**kwargs)
        
        sigMap = getSigMap('Hpp4l')
        genRecoMap = getGenRecoChannelMap('Hpp4l')
        genChannels = getGenChannels('Hpp4l')
        selectedGenChannels = [x for x in genChannels['PP'] if x[:2].count('t')==nTaus and x[2:].count('t')==nTaus]
        selectedRecoChannels = []
        for gen in selectedGenChannels:
            for reco in genRecoMap[gen]:
                if reco not in selectedRecoChannels: selectedRecoChannels += [reco]

        allsamples = ['W','T','TT','TTVall','Z','WW','VHall','WZ','VVV','ZZall']
        signals = ['HppHmm{0}GeV'.format(mass)]

        # get the trees
        ntupleMap = {}
        for s in allsamples+signals:
            for sampleName in sigMap[s]:
                ntupleMap[sampleName] = NtupleWrapper('Hpp4l',sampleName)

        # add to factory
        for s in signals:
            for sig in sigMap[s]:
                if not ntupleMap[sig].getTree().GetEntries(): continue
                self.factory.AddSignalTree(ntupleMap[sig].getTree(),ntupleMap[sig].getIntLumi()/ntupleMap[sig].getSampleLumi())
        for s in allsamples:
            for bg in sigMap[s]:
                if not ntupleMap[bg].getTree().GetEntries(): continue
                self.factory.AddBackgroundTree(ntupleMap[bg].getTree(),ntupleMap[bg].getIntLumi()/ntupleMap[bg].getSampleLumi())

        # per event weight
        weight = 'hpp1_mediumScale*hpp2_mediumScale*hmm1_mediumScale*hmm2_mediumScale*genWeight*pileupWeight*triggerEfficiency'
        self.factory.SetWeightExpression(weight)

        # variables
        #self.factory.AddVariable('hppWindow := fabs(hpp_mass-{0})'.format(mass), '|m_{++}-m_{#Phi}|', 'GeV', 'F')   # h++ symmetric window
        self.factory.AddVariable('hpp_mass','m_{++}','GeV','F')                                                     # h++
        #self.factory.AddVariable('hpp_pt','p_{T}^{++}','GeV','F')                                                   # h++ pt
        #self.factory.AddVariable('hpp_deltaR', '#Delta R(++)', '', 'F')                                             # h++ dR
        #self.factory.AddVariable('hmmWindow := fabs(hmm_mass-{0})'.format(mass), '|m_{--}-m_{#Phi}|', 'GeV', 'F')   # h-- symmetric window
        self.factory.AddVariable('hmm_mass','m_{--}','GeV','F')                                                     # h--
        #self.factory.AddVariable('hmm_pt','p_{T}^{--}','GeV','F')                                                   # h-- pt
        #self.factory.AddVariable('hmm_deltaR', '#Delta R(--)', '', 'F')                                             # h-- dR
        self.factory.AddVariable('st := hpp1_pt+hpp2_pt+hmm1_pt+hmm2_pt', 's_{T}', 'GeV', 'F')                        # 4l st
        self.factory.AddVariable('zWindow := fabs(z_mass-{0})'.format(ZMASS), '|m_{+-}-m_{Z}|', 'GeV', 'F')         # z symmetric window

        minMap = {
            0 : 0.9,
            1 : 0.4,
            2 : 0.3,
        }
        minMass = mass * minMap[nTaus]
        maxMass = mass * 1.1

        # preselection cut
        cutString = ' && '.join(['{0}_passMedium==1'.format(lep) for lep in ['hpp1','hpp2','hmm1','hmm2']])
        cutString += ' && ' + '(' + ' || '.join(['genChannel=="{0}"'.format(chan) for chan in selectedGenChannels + ['a']]) + ')'
        cutString += ' && ' + '(' + ' || '.join(['channel=="{0}"'.format(chan) for chan in selectedRecoChannels]) + ')'
        passCut = ROOT.TCut(cutString)
        self.factory.PrepareTrainingAndTestTree(
            passCut,
            ":".join(
                [
                "nTrain_Signal=0",
                "nTrain_Background=0",
                "SplitMode=Random",
                "NormMode=NumEvents",
                "!V",
                ]
            )
        )

        # options:
        # H : display help
        # V : turn on verbosity
        # IgnoreNegWeightsInTraining : ignore events with negative weights for training, keep for testing

        # book methods
        cuts = self.factory.BookMethod(
            ROOT.TMVA.Types.kCuts,
            "Cuts",
            ":".join(
                [
                    "VarProp=FSmart",
                    #"CutRangeMin[1]=0.", # set max window for Z at 80
                    #"CutRangeMax[1]=80.",
                ]
            )
        )

        #bdt = self.factory.BookMethod(
        #    ROOT.TMVA.Types.kBDT,
        #    "BDT",
        #    ":".join(
        #        [
        #            "NTrees=850",
        #            "MaxDepth=3",
        #            "BoostType=AdaBoost",
        #            "AdaBoostBeta=0.5",
        #            "SeparationType=GiniIndex",
        #            "nCuts=20",
        #            "PruneMethod=NoPruning",
        #        ]
        #    )
        #)

