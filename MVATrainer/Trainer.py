

import ROOT


class Trainer(object):
    '''MVA Trainer'''

    def __init__(self,**kwargs):
        outputFileName = kwargs.pop('outputFileName','mvaTraining.root')
        jobName = kwargs.pop('jobName','training')

        ROOT.TMVA.Tools.Instance()

        self.outFile = ROOT.TFile(outputFileName,'recreate')
        self.factory = ROOT.TMVA.Factory(jobName,self.outFile)

    def train(self):
        self.factory.TrainAllMethods()
        self.factory.TestAllMethods()
        self.factory.EvaluateAllMethods()


