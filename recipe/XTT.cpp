#include <string>
#include <map>
#include <set>
#include <iostream>
#include <utility>
#include <vector>
#include <cstdlib>
#include "CombineHarvester/CombineTools/interface/CombineHarvester.h"
#include "CombineHarvester/CombineTools/interface/CardWriter.h"
#include "CombineHarvester/CombineTools/interface/Observation.h"
#include "CombineHarvester/CombineTools/interface/Process.h"
#include "CombineHarvester/CombineTools/interface/Utilities.h"
#include "CombineHarvester/CombineTools/interface/Systematics.h"
#include "CombineHarvester/CombineTools/interface/BinByBin.h"

using namespace std;

int main() {
  //! [part1]
  // First define the location of the "auxiliaries" directory where we can
  // source the input files containing the datacard shapes
  string aux_shapes = string(getenv("CMSSW_BASE")) + "/src/UWAnalysis/StatTools/data/monohiggs/datacards/";

  typedef vector<string> VString;
  typedef vector<pair<int, string>> Categories;

  // Create an empty CombineHarvester instance that will hold all of the
  // datacard configuration and histograms etc.
  ch::CombineHarvester cb;
  // Uncomment this next line to see a *lot* of debug information
  // cb.SetVerbosity(3);
  VString chns =
  {"mt","et"};


  // Here we will just define two categories for an 13TeV analysis. Each entry in
  // the vector below specifies a bin name and corresponding bin_id.

  //ch::Categories cats = {
  //    {1, "_inclusive"}
  //};
  // ch::Categories is just a typedef of vector<pair<int, string>>
  //! [part1]
  map<string, VString> bkg_procs;
  bkg_procs["et"] = {"ZTT", "W", "QCD", "ZL", "ZJ", "TT", "VV","ZVV"};
  bkg_procs["mt"] = {"ZTT", "W", "QCD", "ZL", "ZJ", "TT", "VV","ZVV"};

  map<string, Categories> cats;
  cats["et"] = {
      {1, "et_inclusive"}};
  cats["mt"] = {
      {1, "mt_inclusive"}};

  //! 
  //Option 1
  //vector<string> massesA = ch::MassesFromRange("400-800:100");
  //vector<string> sig_procs = {"Zprime1000A","Zprime1200A","Zprime1400A"};

  //Option 2
  //vector<string> masses = {"600", "800", "1000", "1200","1400","1700","2000","2500"};
  //vector<string> sig_procs = {"ZprimeA400Z"};
  vector<string> masses = {"1000", "1200","1400","1700","2000","2500"};
  vector<string> sig_procs = {"ZprimeA400Z","ZprimeA500Z","ZprimeA600Z","ZprimeA700Z","ZprimeA800Z"};
  // Or equivalently, specify the mass points explicitly:
  //! [part2]
  for (auto chn : chns) {
      cb.AddObservations(
              {"*"}, {"xtt"}, {"13TeV"}, {chn}, cats[chn]);
      cb.AddProcesses(
              {"*"}, {"xtt"}, {"13TeV"}, {chn}, bkg_procs[chn], cats[chn], false);
      cb.AddProcesses(
              masses, {"xtt"}, {"13TeV"}, {chn}, sig_procs, cats[chn], true);
  }

  //! [part4]

  //! [part4]


  //Some of the code for this is in a nested namespace, so
  // we'll make some using declarations first to simplify things a bit.
  using ch::syst::SystMap;
  using ch::syst::era;
  using ch::syst::channel;
  using ch::syst::bin_id;
  using ch::syst::process;


  //! [part5]
 // cb.cp().signals()
 //     .AddSyst(cb, "lumi_$ERA", "lnN", SystMap<era>::init
 //             ({"13TeV"}, 1.062));

  cb.cp().process(ch::JoinStr({sig_procs,{"ZTT", "W", "ZL", "ZJ", "TT", "VV","ZVV"} }))
      .AddSyst(cb, "CMS_lumi", "lnN", SystMap<>::init(1.062));


  //! [part5]

  //! [part6]
  //cb.cp().process({"Zprime1200A"})
  //    .AddSyst(cb, "pdf_gg", "lnN", SystMap<>::init(1.1));

  cb.cp().process(ch::JoinStr({sig_procs, {"ZTT", "TT"}}))
      .AddSyst(cb, "CMS_eff_t", "lnN", SystMap<>::init(1.06));

  cb.cp().process({"ZTT", "ZL", "ZJ", "ZLL"}).AddSyst(cb,
    "CMS_htt_zjXsec_13TeV", "lnN", SystMap<>::init(1.04));

  cb.cp().process({"TT","QCD"})
      .AddSyst(cb, "CMS_norm_TT_btag ", "lnN", SystMap<>::init(1.13));

  cb.cp().process({"QCD"})
      .AddSyst(cb, "CMS_QCD_Syst ", "lnN", SystMap<>::init(1.3));

  //TT_CMS_htt_ttbarShape_13TeVUp
  cb.cp().process({"TT"})
      .AddSyst(cb, "CMS_htt_ttbarShape_$ERA", "shape", SystMap<>::init(1.00));

  // Electron and muon efficiencies
  cb.cp().AddSyst(cb, "CMS_eff_m", "lnN", SystMap<channel, process>::init
    ({"mt"}, ch::JoinStr({sig_procs, {"ZTT", "TT", "VV", "ZL", "ZJ"}}),  1.02));
  
  cb.cp().AddSyst(cb, "CMS_eff_e", "lnN", SystMap<channel, process>::init
    ({"et"}, ch::JoinStr({sig_procs, {"ZTT", "TT", "VV", "ZL", "ZJ"}}),  1.02));


  // Diboson and ttbar Normalisation - fully correlated
  cb.cp().process({"VV"}).AddSyst(cb,
    "CMS_htt_vvXsec_13TeV", "lnN", SystMap<>::init(1.05));

  cb.cp().process({"TT"}).AddSyst(cb,
    "CMS_htt_tjXsec_13TeV", "lnN", SystMap<>::init(1.06));
  
  // W norm, just for em and tt where MC norm is from MC
  cb.cp().process({"W"}).channel({"et","mt"}).AddSyst(cb,
    "CMS_htt_wjXsec_13TeV", "lnN", SystMap<>::init(1.04));

  cb.cp().process({"W"})
      .AddSyst(cb, "CMS_norm_W ", "lnN", SystMap<>::init(1.1));





  //add btagging uncertainties
  //add lepton energy shifts
  //
  //
  //cb.cp().process(ch::JoinStr({sig_procs, {"ZTT"}}))
  //    .AddSyst(cb, "CMS_scale_t_mutau_$ERA", "shape", SystMap<>::init(1.00));

  //cb.cp().process(ch::JoinStr({sig_procs, {"ZL"}}))
  //    .AddSyst(cb, "CMS_scale_ZLL_$ERA", "shape", SystMap<>::init(1.00));



  for (string chn : chns) {
      string file = aux_shapes + "xtt_" + chn +
          ".inputs-13TeV-mt.root";
      cb.cp().channel({chn}).backgrounds().ExtractShapes(
              file, "$BIN/$PROCESS", "$BIN/$PROCESS_$SYSTEMATIC");
      cb.cp().channel({chn}).signals().ExtractShapes(
              file, "$BIN/$PROCESS$MASS", "$BIN/$PROCESS$MASS_$SYSTEMATIC");
  }

  //! [part8]
  auto bbb = ch::BinByBinFactory()
      .SetAddThreshold(0.1)
      .SetMergeThreshold(0.5)
      .SetFixNorm(true);
  bbb.MergeBinErrors(cb.cp().backgrounds());
  bbb.AddBinByBin(cb.cp().backgrounds(), cb);

  // This function modifies every entry to have a standardised bin name of
  // the form: {analysis}_{channel}_{bin_id}_{era}
  // which is commonly used in the xtt analyses
  ch::SetStandardBinNames(cb);
  //! [part8]

  //! [part9]
  // First we generate a set of bin names:
  set<string> bins = cb.bin_set();
  // This method will produce a set of unique bin names by considering all
  // Observation, Process and Systematic entries in the CombineHarvester
  // instance.

  // We create the output root file that will contain all the shapes.
  // Here we define a CardWriter with a template for how the text datacard
  // and the root files should be named.
  ch::CardWriter writer("$TAG/$MASS/$ANALYSIS_$CHANNEL_$BINID_$ERA.txt",
          "$TAG/common/$ANALYSIS_$CHANNEL.input_$ERA.root");
  // writer.SetVerbosity(1);
  writer.WriteCards("output/xtt_cards/cmb", cb);
  for (auto chn : cb.channel_set()) {
      writer.WriteCards("output/xtt_cards/" + chn, cb.cp().channel({chn}));
  }
  TFile output("output/xtt_cards/xtt_mt.input.root", "RECREATE");


}
