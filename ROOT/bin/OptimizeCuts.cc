#include "TCanvas.h"
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1.h"
#include "PhysicsTools/FWLite/interface/CommandLineParser.h" 
#include "UWAnalysis/ROOT/interface/CutOptimizer.h"

int main (int argc, char* argv[]) 
{

  gROOT->SetBatch(kTRUE);
  gROOT->SetStyle("Plain");


   optutl::CommandLineParser parser ("OptimizeCuts");

   //Input Files-------------------

   //Data
   parser.addOption("dataFile",optutl::CommandLineParser::kString,"File with the data","DATA-ntuple.root");
   parser.addOption("significance",optutl::CommandLineParser::kString,"Significance","SIG/sqrt(SIG+BKG)");
   parser.addOption("signalPreselection",optutl::CommandLineParser::kString,"SignalPreselection","charge==0&&TYPE==1&&abs(muIP)<0.05&&tauLooseIso");
   parser.addOption("bkgPreselection",optutl::CommandLineParser::kString,"BkgPreselection","charge==0&&TYPE!=1&&abs(muIP)<0.05&&tauLooseIso");
   parser.addOption("cuts",optutl::CommandLineParser::kStringVector,"Cuts");
   parser.addOption("dirs",optutl::CommandLineParser::kStringVector,"Dirs");
   parser.addOption("min",optutl::CommandLineParser::kDoubleVector,"min");
   parser.addOption("max",optutl::CommandLineParser::kDoubleVector,"max");
   parser.addOption("step",optutl::CommandLineParser::kDoubleVector,"step");


   parser.parseArguments (argc, argv);

   std::vector<string> cuts = parser.stringVector("cuts");
   std::vector<string> dirs = parser.stringVector("dirs");
   std::vector<double> min = parser.doubleVector("min");
   std::vector<double> max = parser.doubleVector("max");
   std::vector<double> step = parser.doubleVector("step");


   TChain * c = new TChain("tree");
   c->AddFile(parser.stringValue("dataFile").c_str());

   CutOptimizer optimizer(c,c);
   optimizer.setPreselections(parser.stringValue("signalPreselection"),parser.stringValue("bkgPreselection"));
   optimizer.setSignificance(parser.stringValue("significance"));

     for(unsigned short i=0;i<cuts.size();++i)
       {
	 optimizer.addCut(cuts[i],dirs[i],min[i],max[i],step[i]);
       }

     optimizer.run();

     
}
