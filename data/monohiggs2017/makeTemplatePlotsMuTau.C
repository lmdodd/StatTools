#include "makePlot.C"

void makeTemplatePlotsMuTau(){
//void makeLTauStack(TString name,TString file,TString dir,int s,TString labelX,TString units = "GeV",bool left=false,TString channel = "",TString json = "Golden",bool log = false,bool dndm=false,bool doRatio = false, int blind=900, bool doFit=false)
   //
   //
   makeLTauStack("~/www/ztt/2016/MuTau/XTT2017/muTau_m_visi_Incl","muTauPlotsIncl/muTau_m_vis.root","mt_inclusive",3,"visible mass","GeV",false,"#tau_{#mu}#tau_{h}","36.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT2017/muTau_pt_2_AI","muTauPlots/muTau_pt_2_AI.root","mt_inclusive",3,"#tau Pt AntiIso","GeV",false,"#tau_{#mu}#tau_{h}","36.8",false,false,true,900,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT2017/muTau_m_vis","muTauPlots/muTau_m_vis.root","mt_inclusive",3,"visible mass","GeV",false,"#tau_{#mu}#tau_{h}","36.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT2017/muTau_mt_1","muTauPlots/muTau_mt_1.root","mt_inclusive",3,"#mu+ME_{T} m_{T}","GeV",false,"#tau_{#mu}#tau_{h}","36.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT2017/muTau_mt_2","muTauPlots/muTau_mt_2.root","mt_inclusive",3,"#tau+ME_{T} m_{T}","GeV",false,"#tau_{#mu}#tau_{h}","36.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT2017/muTau_mt","muTauPlots/muTau_mt.root","mt_inclusive",3,"Total m_{T}","GeV",false,"#tau_{#mu}#tau_{h}","36.8",true,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT2017/muTau_pt","muTauPlots/muTau_pt.root","mt_inclusive",3,"#mu-#tau Pt","GeV",false,"#tau_{#mu}#tau_{h}","36.8",true,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT2017/muTau_m_vis","muTauPlots/muTau_m_vis.root","mt_inclusive",3,"visible mass","GeV",false,"#tau_{#mu}#tau_{h}","36.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT2017/muTau_m_vis_AI","muTauPlots/muTau_m_vis_AI.root","mt_inclusive",3,"visible mass","GeV",false,"#tau_{#mu}#tau_{h}","36.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT2017/muTau_mt_1_AI","muTauPlots/muTau_mt_1_AI.root","mt_inclusive",3,"mt_1","GeV",false,"#tau_{#mu}#tau_{h}","36.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT2017/muTau_mt_2_AI","muTauPlots/muTau_mt_2_AI.root","mt_inclusive",3,"mt_2","GeV",false,"#tau_{#mu}#tau_{h}","36.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT2017/muTau_mt_AI","muTauPlots/muTau_mt_AI.root","mt_inclusive",3,"mt","GeV",false,"#tau_{#mu}#tau_{h}","36.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT2017/muTau_m_vis_noZweight","muTauPlots/muTau_m_vis_noZweight.root","mt_inclusive",3,"visible mass","GeV",false,"#tau_{#mu}#tau_{h}","36.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT2017/muTau_m_vis_SS","muTauPlots/muTau_m_vis_SS.root","mt_inclusive",3,"visible mass","GeV",false,"#tau_{#mu}#tau_{h}","36.8",false,false,true);
   //makeLTauStack("~/www/ztt/2016/MuTau/XTT2017/muTau_Wweight","muTauPlots/muTau_Wweight.root","mt_inclusive",3,"W-weight","GeV",false,"#tau_{#mu}#tau_{h}","36.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT2017/muTau_npv","muTauPlots/muTau_npv.root","mt_inclusive",3,"npv","",false,"#tau_{#mu}#tau_{h}","36.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT2017/muTau_npv_noweight","muTauPlots/muTau_vertices.root","mt_inclusive",3,"npv","",false,"#tau_{#mu}#tau_{h}","36.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT2017/muTau_OldMVAIso","muTauPlots/muTau_OldMVAIso.root","mt_inclusive",3,"Raw MVAIsolation","GeV",false,"#tau_{#mu}#tau_{h}","36.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT2017/muTau_tauIso","muTauPlots/muTau_tauIso.root","mt_inclusive",3,"Raw Tau Isolation","GeV",false,"#tau_{#mu}#tau_{h}","36.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT2017/muTau_chargedIso","muTauPlots/muTau_chargedIso.root","mt_inclusive",3,"Raw Tau Charged Isolation","GeV",false,"#tau_{#mu}#tau_{h}","36.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT2017/muTau_neutralIso","muTauPlots/muTau_neutralIso.root","mt_inclusive",3,"Raw Tau Neutral Isolation","GeV",false,"#tau_{#mu}#tau_{h}","36.8",false,false,true);

   makeLTauStack("~/www/ztt/2016/MuTau/XTT2017/muTau_phi_1","muTauPlots/muTau_phi_1.root","mt_inclusive",3,"#mu #phi","",false,"#tau_{#mu}#tau_{h}","36.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT2017/muTau_phi_2","muTauPlots/muTau_phi_2.root","mt_inclusive",3,"#tau #phi","",false,"#tau_{#mu}#tau_{h}","36.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT2017/muTau_mt_2","muTauPlots/muTau_mt_2.root","mt_inclusive",3,"#tau +MET m_{T}","GeV",false,"#tau_{#mu}#tau_{h}","36.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT2017/muTau_mt","muTauPlots/muTau_mt.root","mt_inclusive",3,"Total m_{T}","GeV",false,"#tau_{#mu}#tau_{h}","36.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT2017/muTau_ht","muTauPlots/muTau_ht.root","mt_inclusive",3,"Generator HT","GeV",false,"#tau_{#mu}#tau_{h}","36.8",true,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT2017/muTau_dR","muTauPlots/muTau_dR.root","mt_inclusive",3," #mu #tau DR","GeV",false,"#tau_{#mu}#tau_{h}","36.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT2017/muTau_iso_1","muTauPlots/muTau_iso_1.root","mt_inclusive",3,"Isolation #mu DR=0.3","GeV",false,"#tau_{#mu}#tau_{h}","36.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT2017/muTau_iso04_1","muTauPlots/muTau_iso04_1.root","mt_inclusive",3,"Isolation #mu DR=0.4","GeV",false,"#tau_{#mu}#tau_{h}","36.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT2017/muTau_met","muTauPlots/muTau_met.root","mt_inclusive",3,"ME_{T}","GeV",false,"#tau_{#mu}#tau_{h}","36.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT2017/muTau_met_noTTweight","muTauPlots/muTau_met_noTTweight.root","mt_inclusive",3,"ME_{T}","GeV",false,"#tau_{#mu}#tau_{h}","36.8",false,false,true);

   makeLTauStack("~/www/ztt/2016/MuTau/XTT2017/muTau_pt_1","muTauPlots/muTau_pt_1.root","mt_inclusive",3,"#mu P_{T} ","GeV",false,"#tau_{#mu}#tau_{h}","36.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT2017/muTau_pt_2","muTauPlots/muTau_pt_2.root","mt_inclusive",3,"#tau P_{T}","GeV",false,"#tau_{#mu}#tau_{h}","36.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT2017/muTau_eta_1","muTauPlots/muTau_eta_1.root","mt_inclusive",3,"#eta_{#mu}","",false,"#tau_{#mu}#tau_{h}","36.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT2017/muTau_eta_2","muTauPlots/muTau_eta_2.root","mt_inclusive",3,"#eta_{#tau}","",false,"#tau_{#mu}#tau_{h}","36.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT2017/muTau_m_vis","muTauPlots/muTau_m_vis.root","mt_inclusive",3,"visible mass","GeV",false,"#tau_{#mu}#tau_{h}","36.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT2017/muTau_pt","muTauPlots/muTau_pt.root","mt_inclusive",3,"#mu-#tau Pt","GeV",false,"#tau_{#mu}#tau_{h}","36.8",true,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT2017/muTau_q_1","muTauPlots/muTau_q_1.root","mt_inclusive",3,"q_{#mu}","",false,"#tau_{#mu}#tau_{h}","36.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT2017/muTau_q_2","muTauPlots/muTau_q_2.root","mt_inclusive",3,"q_{#tau}","",false,"#tau_{#mu}#tau_{h}","36.8",false,false,true);

   makeLTauStack("~/www/ztt/2016/MuTau/XTT2017/muTau_nCands","muTauPlots/muTau_nCands.root","mt_inclusive",3,"num pair candidates","",false,"#tau_{#mu}#tau_{h}","36.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT2017/muTau_tauDecayMode","muTauPlots/muTau_tauDecayMode.root","mt_inclusive",3,"#tau decay mode","",false,"#tau_{#mu}#tau_{h}","36.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT2017/muTau_m_2","muTauPlots/muTau_m_2.root","mt_inclusive",3,"M_{#tau}","GeV",false,"#tau_{#mu}#tau_{h}","36.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT2017/muTau_mjj","muTauPlots/muTau_mjj.root","mt_inclusive",3,"M_{JJ}","GeV",false,"#tau_{#mu}#tau_{h}","36.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT2017/muTau_njetspt20","muTauPlots/muTau_njetspt20.root","mt_inclusive",3,"Number Jets Pt>20","GeV",false,"#tau_{#mu}#tau_{h}","36.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT2017/muTau_njets","muTauPlots/muTau_njets.root","mt_inclusive",3,"Number Jets Pt>30","GeV",false,"#tau_{#mu}#tau_{h}","36.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT2017/muTau_nbtagNoSF","muTauPlots/muTau_nbtagNoSF.root","mt_inclusive",3,"Number B-tag Jets Pt>30","GeV",false,"#tau_{#mu}#tau_{h}","36.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT2017/muTau_jeta_1","muTauPlots/muTau_jeta_1.root","mt_inclusive",3,"Leading jet #eta","",false,"#tau_{#mu}#tau_{h}","36.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT2017/muTau_jeta_2","muTauPlots/muTau_jeta_2.root","mt_inclusive",3,"Sub-Leading jet #eta","",false,"#tau_{#mu}#tau_{h}","36.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT2017/muTau_tauNIsoTracks","muTauPlots/muTau_tauNIsoTracks.root","mt_inclusive",3,"Number Isolated Tracks","GeV",false,"#tau_{#mu}#tau_{h}","36.8",false,false,true);

   makeLTauStack("~/www/ztt/2016/MuTau/XTT2017/muTau_jpt_1","muTauPlots/muTau_jpt_1.root","mt_inclusive",3,"Leading jet Pt","GeV",false,"#tau_{#mu}#tau_{h}","36.8",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTT2017/muTau_jpt_2","muTauPlots/muTau_jpt_2.root","mt_inclusive",3,"Sub-Leading jet Pt","GeV",false,"#tau_{#mu}#tau_{h}","36.8",false,false,true);
   }
