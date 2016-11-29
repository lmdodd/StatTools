#include "makePlot.C"

void makeTemplatePlotsMuTau(){
   makeLTauStack("~/www/ztt/2016/MuTau/XTT/muTau_mt_1","muTauPlots/muTau_mt_1.root","mt_inclusive",3,"#mu+ME_{T} m_{T}","GeV",false,"#tau_{#mu}#tau_{h}","28.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT/muTau_mt_2","muTauPlots/muTau_mt_2.root","mt_inclusive",3,"#tau+ME_{T} m_{T}","GeV",false,"#tau_{#mu}#tau_{h}","28.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT/muTau_mt","muTauPlots/muTau_mt.root","mt_inclusive",3,"Total m_{T}","GeV",false,"#tau_{#mu}#tau_{h}","28.8",true,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT/muTau_pt","muTauPlots/muTau_pt.root","mt_inclusive",3,"#mu-#tau Pt","GeV",false,"#tau_{#mu}#tau_{h}","28.8",true,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT/muTau_met","muTauPlots/muTau_met.root","mt_inclusive",3,"ME_{T}","GeV",false,"#tau_{#mu}#tau_{h}","28.8",true,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT/muTau_m_vis","muTauPlots/muTau_m_vis.root","mt_inclusive",3,"visible mass","GeV",false,"#tau_{#mu}#tau_{h}","28.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT/muTau_npv","muTauPlots/muTau_npv.root","mt_inclusive",3,"npv","",false,"#tau_{#mu}#tau_{h}","28.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT/muTau_OldMVAIso","muTauPlots/muTau_OldMVAIso.root","mt_inclusive",3,"Raw MVAIsolation","GeV",false,"#tau_{#mu}#tau_{h}","28.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT/muTau_tauIso","muTauPlots/muTau_tauIso.root","mt_inclusive",3,"Raw Tau Isolation","GeV",false,"#tau_{#mu}#tau_{h}","28.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT/muTau_chargedIso","muTauPlots/muTau_chargedIso.root","mt_inclusive",3,"Raw Tau Charged Isolation","GeV",false,"#tau_{#mu}#tau_{h}","28.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT/muTau_neutralIso","muTauPlots/muTau_neutralIso.root","mt_inclusive",3,"Raw Tau Neutral Isolation","GeV",false,"#tau_{#mu}#tau_{h}","28.8",false,false,true);

   makeLTauStack("~/www/ztt/2016/MuTau/XTT/muTau_phi_1","muTauPlots/muTau_phi_1.root","mt_inclusive",3,"#mu #phi","",false,"#tau_{#mu}#tau_{h}","28.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT/muTau_phi_2","muTauPlots/muTau_phi_2.root","mt_inclusive",3,"#tau #phi","",false,"#tau_{#mu}#tau_{h}","28.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT/muTau_mt_2","muTauPlots/muTau_mt_2.root","mt_inclusive",3,"#tau +MET m_{T}","GeV",false,"#tau_{#mu}#tau_{h}","28.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT/muTau_mt","muTauPlots/muTau_mt.root","mt_inclusive",3,"Total m_{T}","GeV",false,"#tau_{#mu}#tau_{h}","28.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT/muTau_ht","muTauPlots/muTau_ht.root","mt_inclusive",3,"Generator HT","GeV",false,"#tau_{#mu}#tau_{h}","28.8",true,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT/muTau_dR","muTauPlots/muTau_dR.root","mt_inclusive",3," #mu #tau DR","GeV",false,"#tau_{#mu}#tau_{h}","28.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT/muTau_iso_1","muTauPlots/muTau_iso_1.root","mt_inclusive",3,"Isolation #mu DR=0.3","GeV",false,"#tau_{#mu}#tau_{h}","28.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT/muTau_iso04_1","muTauPlots/muTau_iso04_1.root","mt_inclusive",3,"Isolation #mu DR=0.4","GeV",false,"#tau_{#mu}#tau_{h}","28.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT/muTau_met","muTauPlots/muTau_met.root","mt_inclusive",3,"ME_{T}","GeV",false,"#tau_{#mu}#tau_{h}","28.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT/muTau_pt_1","muTauPlots/muTau_pt_1.root","mt_inclusive",3,"#mu P_{T} ","GeV",false,"#tau_{#mu}#tau_{h}","28.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT/muTau_pt_2","muTauPlots/muTau_pt_2.root","mt_inclusive",3,"#tau P_{T}","GeV",false,"#tau_{#mu}#tau_{h}","28.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT/muTau_eta_1","muTauPlots/muTau_eta_1.root","mt_inclusive",3,"#eta_{#mu}","",false,"#tau_{#mu}#tau_{h}","28.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT/muTau_eta_2","muTauPlots/muTau_eta_2.root","mt_inclusive",3,"#eta_{#tau}","",false,"#tau_{#mu}#tau_{h}","28.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT/muTau_m_vis","muTauPlots/muTau_m_vis.root","mt_inclusive",3,"visible mass","GeV",false,"#tau_{#mu}#tau_{h}","28.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT/muTau_pt","muTauPlots/muTau_pt.root","mt_inclusive",3,"#mu-#tau Pt","GeV",false,"#tau_{#mu}#tau_{h}","28.8",true,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT/muTau_q_1","muTauPlots/muTau_q_1.root","mt_inclusive",3,"q_{#mu}","",false,"#tau_{#mu}#tau_{h}","28.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT/muTau_q_2","muTauPlots/muTau_q_2.root","mt_inclusive",3,"q_{#tau}","",false,"#tau_{#mu}#tau_{h}","28.8",false,false,true);

   makeLTauStack("~/www/ztt/2016/MuTau/XTT/muTau_nCands","muTauPlots/muTau_nCands.root","mt_inclusive",3,"nCands","",false,"#tau_{#mu}#tau_{h}","28.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT/muTau_tauDecayMode","muTauPlots/muTau_tauDecayMode.root","mt_inclusive",3,"#tau decay mode","",false,"#tau_{#mu}#tau_{h}","28.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT/muTau_m_2","muTauPlots/muTau_m_2.root","mt_inclusive",3,"M_{#tau}","GeV",false,"#tau_{#mu}#tau_{h}","28.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT/muTau_mjj","muTauPlots/muTau_mjj.root","mt_inclusive",3,"M_{JJ}","GeV",false,"#tau_{#mu}#tau_{h}","28.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT/muTau_njetspt20","muTauPlots/muTau_njetspt20.root","mt_inclusive",3,"Number Jets Pt>20","GeV",false,"#tau_{#mu}#tau_{h}","28.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT/muTau_njets","muTauPlots/muTau_njets.root","mt_inclusive",3,"Number Jets Pt>30","GeV",false,"#tau_{#mu}#tau_{h}","28.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT/muTau_nbtagNoSF","muTauPlots/muTau_nbtagNoSF.root","mt_inclusive",3,"Number B-tag Jets Pt>30","GeV",false,"#tau_{#mu}#tau_{h}","28.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT/muTau_jeta_1","muTauPlots/muTau_jeta_1.root","mt_inclusive",3,"Leading jet #eta","",false,"#tau_{#mu}#tau_{h}","28.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT/muTau_jeta_2","muTauPlots/muTau_jeta_2.root","mt_inclusive",3,"Sub-Leading jet #eta","",false,"#tau_{#mu}#tau_{h}","28.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT/muTau_tauNIsoTracks","muTauPlots/muTau_tauNIsoTracks.root","mt_inclusive",3,"Number Isolated Tracks","GeV",false,"#tau_{#mu}#tau_{h}","28.8",false,false,true);

   makeLTauStack("~/www/ztt/2016/MuTau/XTT/muTau_jpt_1","muTauPlots/muTau_jpt_1.root","mt_inclusive",3,"Leading jet Pt","GeV",false,"#tau_{#mu}#tau_{h}","28.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT/muTau_jpt_2","muTauPlots/muTau_jpt_2.root","mt_inclusive",3,"Sub-Leading jet Pt","GeV",false,"#tau_{#mu}#tau_{h}","28.8",false,false,true);
   }
