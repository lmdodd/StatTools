#include "makePlot.C"

void makeTemplatePlotsETau(){
   makeLTauStack("~/www/ztt/2016/ETau/XTT2017/eleTau_m_vis","eleTauPlots/eleTau_m_vis.root","et_inclusive",0,"visible m_vis","GeV",false,"#tau_{e}#tau_{h}","35.9",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT2017/eleTau_mt_1","eleTauPlots/eleTau_mt_1.root","et_inclusive",0,"#mu+ME_{T} M_{T}","GeV",false,"#tau_{e}#tau_{h}","35.9",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT2017/eleTau_mt_2","eleTauPlots/eleTau_mt_2.root","et_inclusive",0,"#tau+ME_{T} M_{T}","GeV",false,"#tau_{e}#tau_{h}","35.9",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT2017/eleTau_mt","eleTauPlots/eleTau_mt.root","et_inclusive",0,"Total M_{T}","GeV",false,"#tau_{e}#tau_{h}","35.9",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT2017/eleTau_dR","eleTauPlots/eleTau_dR.root","et_inclusive",0,"dR","GeV",false,"#tau_{e}#tau_{h}","35.9",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT2017/eleTau_m_vis","eleTauPlots/eleTau_m_vis.root","et_inclusive",0,"visible m_vis","GeV",false,"#tau_{e}#tau_{h}","35.9",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT2017/eleTau_m_vis_SS","eleTauPlots/eleTau_m_vis_SS.root","et_inclusive",0,"visible m_vis SS","GeV",false,"#tau_{e}#tau_{h}","35.9",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT2017/eleTau_m_vis_AI","eleTauPlots/eleTau_m_vis_AI.root","et_inclusive",0,"visible m_vis AI","GeV",false,"#tau_{e}#tau_{h}","35.9",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT2017/eleTau_m_vis_AI_SS","eleTauPlots/eleTau_m_vis_AI_SS.root","et_inclusive",0,"visible m_vis AI SS","GeV",false,"#tau_{e}#tau_{h}","35.9",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT2017/eleTau_phi_1","eleTauPlots/eleTau_phi1.root","et_inclusive",0,"e #phi ","",false,"#tau_{e}#tau_{h}","35.9",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT2017/eleTau_phi_2","eleTauPlots/eleTau_phi2.root","et_inclusive",0,"#tau #phi","",false,"#tau_{e}#tau_{h}","35.9",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT2017/eleTau_relIso","eleTauPlots/eleTau_relIso.root","et_inclusive",0,"(charged+neutral)/pt","GeV",false,"#tau_{e}#tau_{h}","35.9",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT2017/eleTau_chIso","eleTauPlots/eleTau_chIso.root","et_inclusive",0,"Charged #tau Iso","GeV",false,"#tau_{e}#tau_{h}","35.9",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT2017/eleTau_neuIso","eleTauPlots/eleTau_neuIso.root","et_inclusive",0,"Neutral #tau Iso","GeV",false,"#tau_{e}#tau_{h}","35.9",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT2017/eleTau_puIso","eleTauPlots/eleTau_puIso.root","et_inclusive",0,"PU #tau Iso","GeV",false,"#tau_{e}#tau_{h}","35.9",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT2017/eleTau_mt_2","eleTauPlots/eleTau_mt_2.root","et_inclusive",0,"#tau+MET m_{T}","GeV",false,"#tau_{e}#tau_{h}","35.9",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT2017/eleTau_iso_1","eleTauPlots/eleTau_iso_1.root","et_inclusive",0,"e iso dR=0.3","GeV",false,"#tau_{e}#tau_{h}","35.9",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT2017/eleTau_iso04_1","eleTauPlots/eleTau_iso04_1.root","et_inclusive",0,"e iso dR=0.4","GeV",false,"#tau_{e}#tau_{h}","35.9",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT2017/eleTau_met","eleTauPlots/eleTau_met.root","et_inclusive",0,"ME_{T}","GeV",false,"#tau_{e}#tau_{h}","35.9",false,false,true,150);
   makeLTauStack("~/www/ztt/2016/ETau/XTT2017/eleTau_mt","eleTauPlots/eleTau_mt.root","et_inclusive",0,"Total M_{T}","GeV",false,"#tau_{e}#tau_{h}","35.9",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT2017/eleTau_pt_1","eleTauPlots/eleTau_pt_1.root","et_inclusive",0,"e P_{T} ","GeV",false,"#tau_{e}#tau_{h}","35.9",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT2017/eleTau_tauIso","eleTauPlots/eleTau_tauIso.root","et_inclusive",0,"e tauIso","GeV",false,"#tau_{e}#tau_{h}","35.9",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT2017/eleTau_pt_2","eleTauPlots/eleTau_pt_2.root","et_inclusive",0,"#tau P_{T}","GeV",false,"#tau_{e}#tau_{h}","35.9",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT2017/eleTau_eta_1","eleTauPlots/eleTau_eta_1.root","et_inclusive",0,"#eta_{e}","",false,"#tau_{e}#tau_{h}","35.9",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT2017/eleTau_eta_2","eleTauPlots/eleTau_eta_2.root","et_inclusive",0,"#eta_{#tau}","",false,"#tau_{e}#tau_{h}","35.9",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT2017/eleTau_jeta_1","eleTauPlots/eleTau_jeta_1.root","et_inclusive",0,"Leading Jet #eta","",false,"#tau_{e}#tau_{h}","35.9",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT2017/eleTau_jeta_2","eleTauPlots/eleTau_jeta_2.root","et_inclusive",0,"Sub-Leading Jet #eta","",false,"#tau_{e}#tau_{h}","35.9",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT2017/eleTau_pth","eleTauPlots/eleTau_pth.root","et_inclusive",0,"e-#tau Pt","GeV",false,"#tau_{e}#tau_{h}","35.9",true,false,true,200);
   makeLTauStack("~/www/ztt/2016/ETau/XTT2017/eleTau_q_1","eleTauPlots/eleTau_q_1.root","et_inclusive",0,"q_{e}","",false,"#tau_{e}#tau_{h}","35.9",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT2017/eleTau_q_2","eleTauPlots/eleTau_q_2.root","et_inclusive",0,"q_{#tau}","",false,"#tau_{e}#tau_{h}","35.9",false,false,true);

   makeLTauStack("~/www/ztt/2016/ETau/XTT2017/eleTau_OldMVAIso","eleTauPlots/eleTau_OldMVAIso.root","et_inclusive",0,"OldMVAIso","",false,"#tau_{e}#tau_{h}","35.9",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT2017/eleTau_vertices","eleTauPlots/eleTau_vertices.root","et_inclusive",0,"vertices","",false,"#tau_{e}#tau_{h}","35.9",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT2017/eleTau_npv","eleTauPlots/eleTau_npv.root","et_inclusive",0,"npv","",false,"#tau_{e}#tau_{h}","35.9",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT2017/eleTau_nCands","eleTauPlots/eleTau_nCands.root","et_inclusive",0,"nCand pairs","",false,"#tau_{e}#tau_{h}","35.9",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT2017/eleTau_tauDecayMode","eleTauPlots/eleTau_tauDecayMode.root","et_inclusive",0,"#tau decay mode","",false,"#tau_{e}#tau_{h}","35.9",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT2017/eleTau_tauZIP","eleTauPlots/eleTau_tauZIP.root","et_inclusive",0,"#tau ZIP","",false,"#tau_{e}#tau_{h}","35.9",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT2017/eleTau_tauNIsoTracks","eleTauPlots/eleTau_tauNIsoTracks.root","et_inclusive",0,"Number Isolated tracks","",false,"#tau_{e}#tau_{h}","35.9",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT2017/eleTau_m_2","eleTauPlots/eleTau_m_2.root","et_inclusive",0,"M_{#tau}","GeV",false,"#tau_{e}#tau_{h}","35.9",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT2017/eleTau_mjj","eleTauPlots/eleTau_mjj.root","et_inclusive",0,"M_{JJ}","GeV",false,"#tau_{e}#tau_{h}","35.9",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT2017/eleTau_njetspt20","eleTauPlots/eleTau_njetspt20.root","et_inclusive",0,"Number Jets Pt>20","GeV",false,"#tau_{e}#tau_{h}","35.9",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT2017/eleTau_njets","eleTauPlots/eleTau_njets.root","et_inclusive",0,"Number Jets Pt>30","GeV",false,"#tau_{e}#tau_{h}","35.9",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT2017/eleTau_nbtag","eleTauPlots/eleTau_nbtag.root","et_inclusive",0,"Number CISV Medium Jets Pt>20","",false,"#tau_{e}#tau_{h}","35.9",true,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT2017/eleTau_jpt_1","eleTauPlots/eleTau_jpt_1.root","et_inclusive",0,"Leading Jet Pt","GeV",false,"#tau_{e}#tau_{h}","35.9",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT2017/eleTau_jpt_2","eleTauPlots/eleTau_jpt_2.root","et_inclusive",0,"Sub-Leading Jet Pt","GeV",false,"#tau_{e}#tau_{h}","35.9",false,false,true);
   }


