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
  string aux_shapes = string(getenv("CMSSW_BASE")) + "/src/UWAnalysis/StatTools/data/sm_higgs_ltau/datacardsRename/";

  typedef vector<string> VString;
  typedef vector<pair<int, string>> Categories;

  // Create an empty CombineHarvester instance that will hold all of the
  // datacard configuration and histograms etc.
  ch::CombineHarvester cb;
  // Uncomment this next line to see a *lot* of debug information
  // cb.SetVerbosity(3);
  VString chns =
  {"et","mt"};


  // Here we will just define two categories for an 13TeV analysis. Each entry in
  // the vector below specifies a bin name and corresponding bin_id.

  //ch::Categories cats = {
  //    {1, "_inclusive"}
  //};
  // ch::Categories is just a typedef of vector<pair<int, string>>
  //! [part1]
  map<string, VString> bkg_procs;
  bkg_procs["et"] = {"ZTT", "W", "QCD", "ZL", "ZJ", "TT", "VV"};
  bkg_procs["mt"] = {"ZTT", "W", "QCD", "ZL", "ZJ", "TT", "VV"};

  map<string, Categories> cats;
  cats["et"] = {
      {1, "et_0jet"},
      {2, "et_1jet"},
      {3, "et_2jet"}};

  cats["mt"] = {
      {1, "mt_0jet"},
      {2, "mt_1jet"},
      {3, "mt_2jet"}};

  //! [part2]
  // Or equivalently, specify the mass points explicitly:
  vector<string> sig_procs = {"ggH","qqH"};
  vector<string> masses = ch::MassesFromRange("120-130:5");
  //vector<string> masses = {"125"};
  //! [part2]
  for (auto chn : chns) {
      cb.AddObservations(
              {"*"}, {"htt"}, {"13TeV"}, {chn}, cats[chn]);
      cb.AddProcesses(
              {"*"}, {"htt"}, {"13TeV"}, {chn}, bkg_procs[chn], cats[chn], false);
      cb.AddProcesses(
              masses, {"htt"}, {"13TeV"}, {chn}, sig_procs, cats[chn], true);
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

  cb.cp().process(ch::JoinStr({sig_procs,{"ZTT", "W", "ZL", "ZJ", "TT", "VV"} }))
      .AddSyst(cb, "CMS_lumi", "lnN", SystMap<>::init(1.062));

  //! [part6]
  cb.cp().process({"ggH"})
      .AddSyst(cb, "pdf_gg", "lnN", SystMap<>::init(1.1));

  cb.cp().process(ch::JoinStr({sig_procs, {"ZTT", "TT"}}))
      .AddSyst(cb, "CMS_eff_t", "lnN", SystMap<>::init(1.06));

  cb.cp().process({"ZTT", "ZL", "ZJ", "ZLL"}).AddSyst(cb,
    "CMS_htt_zjXsec_13TeV", "lnN", SystMap<>::init(1.03));

  cb.cp().process({"TT"})
      .AddSyst(cb, "CMS_norm_TT_", "lnN", SystMap<>::init(1.1));

  cb.cp().process({"TT","QCD"})
      .AddSyst(cb, "CMS_norm_TT_btag", "lnN", SystMap<>::init(1.1));


  cb.cp().process({"QCD"})
      .AddSyst(cb, "CMS_QCD_Syst", "lnN", SystMap<>::init(1.3));

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
    "CMS_htt_vvXsec_13TeV", "lnN", SystMap<>::init(1.30));

  cb.cp().process({"W"})
      .AddSyst(cb, "CMS_norm_W ", "lnN", SystMap<>::init(1.1));



  //! [part5]


  for (string chn : chns) {
      string file = aux_shapes + "htt_" + chn +
          "_withbtagveto.inputs-sm-13TeV-m_vis.root";
      cb.cp().channel({chn}).backgrounds().ExtractShapes(
              file, "$BIN/$PROCESS", "$BIN/$PROCESS_$SYSTEMATIC");
      cb.cp().channel({chn}).signals().ExtractShapes(
              file, "$BIN/$PROCESS$MASS", "$BIN/$PROCESS$MASS_$SYSTEMATIC");
  }

  //! [part8]
  auto bbb = ch::BinByBinFactory()
      .SetAddThreshold(0.1)
      .SetMergeThreshold(0.3)
      .SetFixNorm(true);
  bbb.MergeBinErrors(cb.cp().backgrounds());
  bbb.AddBinByBin(cb.cp().backgrounds(), cb);


  // This function modifies every entry to have a standardised bin name of
  // the form: {analysis}_{channel}_{bin_id}_{era}
  // which is commonly used in the htt analyses
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
  writer.WriteCards("output/htt_cards/cmb", cb);
  for (auto chn : cb.channel_set()) {
    writer.WriteCards("output/htt_cards/" + chn, cb.cp().channel({chn}));
  }
  TFile output("output/htt_cards/htt_mvis.input.root", "RECREATE");


}
