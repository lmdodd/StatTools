#include "makePlot.C"

void makeTemplatePlotsTauTau(){
   makeLTauStack("~/www/ztt/2016/TauTau/XTT2017/tautau_m_vis","tauTauPlots/tautau-m_vis.root","tt_inclusive",0,"M_{vis}","GeV",false,"#tau_{h}#tau_{h}","36.8",false,false,false,true);
   makeLTauStack("~/www/ztt/2016/TauTau/XTT2017/tautau_m_vis_SS","tauTauPlots/tautau-m_vis_SS.root","tt_inclusive",0,"M_{vis} SS","GeV",false,"#tau_{h}#tau_{h}","36.8",false,false,false,true);
   makeLTauStack("~/www/ztt/2016/TauTau/XTT2017/tautau_m_vis_AI","tauTauPlots/tautau-m_vis_AI.root","tt_inclusive",0,"M_{vis} AI","GeV",false,"#tau_{h}#tau_{h}","36.8",false,false,false,true);
   makeLTauStack("~/www/ztt/2016/TauTau/XTT2017/tautau_m_vis_AI_SS","tauTauPlots/tautau-m_vis_AI_SS.root","tt_inclusive",0,"M_{vis} AI SS","GeV",false,"#tau_{h}#tau_{h}","36.8",false,false,false,true);
   makeLTauStack("~/www/ztt/2016/TauTau/XTT2017/tautau_mt_1","tauTauPlots/tautau-mt_1.root","tt_inclusive",3,"#tau+MET M_{T}","GeV",false,"#tau_{h}#tau_{h}","36.8",false,false,false,true);
   makeLTauStack("~/www/ztt/2016/TauTau/XTT2017/tautau_mt_2","tauTauPlots/tautau-mt_2.root","tt_inclusive",3,"#tau+MET M_{T}","GeV",false,"#tau_{h}#tau_{h}","36.8",false,false,false,true);
   makeLTauStack("~/www/ztt/2016/TauTau/XTT2017/tautau_met","tauTauPlots/tautau-met.root","tt_inclusive",3,"ME_{T}","GeV",false,"#tau_{h}#tau_{h}","36.8",false,false,false,true);
   makeLTauStack("~/www/ztt/2016/TauTau/XTT2017/tautau_mt12","tauTauPlots/tautau-mt12.root","tt_inclusive",3,"M_{T} Total","GeV",false,"#tau_{h}#tau_{h}","36.8",false,false,false,true);
   makeLTauStack("~/www/ztt/2016/TauTau/XTT2017/tautau_pth","tauTauPlots/tautau-pth.root","tt_inclusive",3,"P_{T} Total","GeV",false,"#tau_{h}#tau_{h}","36.8",false,false,false,true);
   makeLTauStack("~/www/ztt/2016/TauTau/XTT2017/tautau_pt_1","tauTauPlots/tautau-pt_1.root","tt_inclusive",3,"#tau_{1} P_{T}","GeV",false,"#tau_{h}#tau_{h}","36.8",false,false,false,true);
   makeLTauStack("~/www/ztt/2016/TauTau/XTT2017/tautau_pt_2","tauTauPlots/tautau-pt_2.root","tt_inclusive",3,"#tau_{2} P_{T}","GeV",false,"#tau_{h}#tau_{h}","36.8",false,false,false,true);
   makeLTauStack("~/www/ztt/2016/TauTau/XTT2017/tautau_eta_1","tauTauPlots/tautau-eta_1.root","tt_inclusive",3,"#tau_{1} #eta_{T}","GeV",false,"#tau_{h}#tau_{h}","36.8",false,false,false,true);
   makeLTauStack("~/www/ztt/2016/TauTau/XTT2017/tautau_eta_2","tauTauPlots/tautau-eta_2.root","tt_inclusive",3,"#tau_{2} #eta_{T}","GeV",false,"#tau_{h}#tau_{h}","36.8",false,false,false,true);
   makeLTauStack("~/www/ztt/2016/TauTau/XTT2017/tautau_phi_1","tauTauPlots/tautau-phi_1.root","tt_inclusive",3,"#tau_{1} #phi_{T}","GeV",false,"#tau_{h}#tau_{h}","36.8",false,false,false,true);
   makeLTauStack("~/www/ztt/2016/TauTau/XTT2017/tautau_phi_2","tauTauPlots/tautau-phi_2.root","tt_inclusive",3,"#tau_{2} #phi_{T}","GeV",false,"#tau_{h}#tau_{h}","36.8",false,false,false,true);

   makeLTauStack("~/www/ztt/2016/TauTau/XTT2017/tautau_npv","tauTauPlots/tautau-npv.root","tt_inclusive",3,"npv","GeV",false,"#tau_{h}#tau_{h}","36.8",false,false,false,true);
   makeLTauStack("~/www/ztt/2016/TauTau/XTT2017/tautau_dR","tauTauPlots/tautau-dR.root","tt_inclusive",3,"dR","",false,"#tau_{h}#tau_{h}","36.8",false,false,false,true);
   makeLTauStack("~/www/ztt/2016/TauTau/XTT2017/tautau_tauDecayMode","tauTauPlots/tautau-tauDecayMode.root","tt_inclusive",3,"tauDecayMode","GeV",false,"#tau_{h}#tau_{h}","36.8",false,false,false,true);
}
