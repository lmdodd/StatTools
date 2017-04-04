#include "makePlot.C"

void makeTemplatePlotsMuTrack(){

   makeLTauStack("~/www/ztt/2016/MuTau/XTTBoosted/muTrack_m_vis","muTrackPlots/muTrack_m_vis.root","_inclusive",3,"visible mass","GeV",false,"#tau_{#mu}#tau_{1 prong}","36.2",true,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTTBoosted/muTrack_m_vis_SS","muTrackPlots/muTrack_m_vis_SS.root","_inclusive",3,"SS visible mass","GeV",false,"#tau_{#mu}#tau_{1 prong}","36.2",true,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTTBoosted/muTrack_mt_1","muTrackPlots/muTrack_mt_1.root","_inclusive",3,"M_{T,1}","GeV",false,"#tau_{#mu}#tau_{1 prong}","36.2",true,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTTBoosted/muTrack_mt_1_SS","muTrackPlots/muTrack_mt_1_SS.root","_inclusive",3,"SS M_{T,1}","GeV",false,"#tau_{#mu}#tau_{1 prong}","36.2",true,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTTBoosted/muTrack_lIso","muTrackPlots/muTrack_lIso.root","_inclusive",3,"lepton Iso","GeV",false,"#tau_{#mu}#tau_{1 prong}","36.2",true,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTTBoosted/muTrack_lIso_SS","muTrackPlots/muTrack_lIso_SS.root","_inclusive",3,"SS lepton Iso","GeV",false,"#tau_{#mu}#tau_{1 prong}","36.2",true,false,true);
   
   makeLTauStack("~/www/ztt/2016/MuTau/XTTBoosted/muTrack_mt_2","muTrackPlots/muTrack_mt_2.root","_inclusive",3,"#tau +MET m_{T}","GeV",false,"#tau_{#mu}#tau_{1 prong}","36.2",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTTBoosted/muTrack_mt_2_SS","muTrackPlots/muTrack_mt_2_SS.root","_inclusive",3,"SS #tau +MET m_{T}","GeV",false,"#tau_{#mu}#tau_{1 prong}","36.2",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTTBoosted/muTrack_mt","muTrackPlots/muTrack_mt.root","_inclusive",3,"Total m_{T}","GeV",false,"#tau_{#mu}#tau_{1 prong}","36.2",true,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTTBoosted/muTrack_st","muTrackPlots/muTrack_st.root","_inclusive",3,"Total S_{T}","GeV",false,"#tau_{#mu}#tau_{1 prong}","36.2",true,false,true);
   //makeLTauStack("~/www/ztt/2016/MuTau/XTTBoosted/muTrack_npv","muTrackPlots/muTrack_npv.root","_inclusive",3,"npv","",false,"#tau_{#mu}#tau_{1 prong}","36.2",false,false,true);

   makeLTauStack("~/www/ztt/2016/MuTau/XTTBoosted/muTrack_phi_1","muTrackPlots/muTrack_phi_1.root","_inclusive",3,"#mu #phi","",false,"#tau_{#mu}#tau_{1 prong}","36.2",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTTBoosted/muTrack_phi_2","muTrackPlots/muTrack_phi_2.root","_inclusive",3,"#tau #phi","",false,"#tau_{#mu}#tau_{1 prong}","36.2",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTTBoosted/muTrack_ht","muTrackPlots/muTrack_ht.root","_inclusive",3,"Generator HT","GeV",false,"#tau_{#mu}#tau_{1 prong}","36.2",true,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTTBoosted/muTrack_iso04_1","muTrackPlots/muTrack_iso04_1.root","_inclusive",3,"Isolation #mu DR=0.3","GeV",false,"#tau_{#mu}#tau_{1 prong}","36.2",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTTBoosted/muTrack_met","muTrackPlots/muTrack_met.root","_inclusive",3,"ME_{T}","GeV",false,"#tau_{#mu}#tau_{1 prong}","36.2",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTTBoosted/muTrack_pt_1","muTrackPlots/muTrack_pt_1.root","_inclusive",3,"#mu P_{T} ","GeV",false,"#tau_{#mu}#tau_{1 prong}","36.2",true,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTTBoosted/muTrack_pt_2","muTrackPlots/muTrack_pt_2.root","_inclusive",3,"#tau P_{T}","GeV",false,"#tau_{#mu}#tau_{1 prong}","36.2",true,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTTBoosted/muTrack_eta_1","muTrackPlots/muTrack_eta_1.root","_inclusive",3,"#eta_{#mu}","",false,"#tau_{#mu}#tau_{1 prong}","36.2",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTTBoosted/muTrack_eta_2","muTrackPlots/muTrack_eta_2.root","_inclusive",3,"#eta_{#tau}","",false,"#tau_{#mu}#tau_{1 prong}","36.2",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTTBoosted/muTrack_m_vis","muTrackPlots/muTrack_m_vis.root","_inclusive",3,"visible mass","GeV",false,"#tau_{#mu}#tau_{1 prong}","36.2",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTTBoosted/muTrack_pt","muTrackPlots/muTrack_pt.root","_inclusive",3,"#mu-#tau Pt","GeV",false,"#tau_{#mu}#tau_{1 prong}","36.2",false,false,true);
   //makeLTauStack("~/www/ztt/2016/MuTau/XTTBoosted/muTrack_q_1","muTrackPlots/muTrack_q_1.root","_inclusive",3,"q_{#mu}","",false,"#tau_{#mu}#tau_{1 prong}","36.2",false,false,true);
   //makeLTauStack("~/www/ztt/2016/MuTau/XTTBoosted/muTrack_q_2","muTrackPlots/muTrack_q_2.root","_inclusive",3,"q_{#tau}","",false,"#tau_{#mu}#tau_{1 prong}","36.2",false,false,true);

   makeLTauStack("~/www/ztt/2016/MuTau/XTTBoosted/muTrack_nCands","muTrackPlots/muTrack_nCands.root","_inclusive",3,"nCands","",false,"#tau_{#mu}#tau_{1 prong}","36.2",true,false,true);
   //makeLTauStack("~/www/ztt/2016/MuTau/XTTBoosted/muTrack_mjj","muTrackPlots/muTrack_mjj.root","_inclusive",3,"M_{JJ}","GeV",false,"#tau_{#mu}#tau_{1 prong}","36.2",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTTBoosted/muTrack_njetspt20","muTrackPlots/muTrack_njetspt20.root","_inclusive",3,"Number Jets Pt>20","GeV",false,"#tau_{#mu}#tau_{1 prong}","36.2",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTTBoosted/muTrack_njets","muTrackPlots/muTrack_njets.root","_inclusive",3,"Number Jets Pt>30","GeV",false,"#tau_{#mu}#tau_{1 prong}","36.2",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/XTTBoosted/muTrack_nbtagNoSF","muTrackPlots/muTrack_nbtagNoSF.root","_inclusive",3,"Number B-tag Jets Pt>30","GeV",false,"#tau_{#mu}#tau_{1 prong}","36.2",false,false,true);
   //makeLTauStack("~/www/ztt/2016/MuTau/XTTBoosted/muTrack_jeta_1","muTrackPlots/muTrack_jeta_1.root","_inclusive",3,"Leading jet #eta","",false,"#tau_{#mu}#tau_{1 prong}","36.2",false,false,true);
   //makeLTauStack("~/www/ztt/2016/MuTau/XTTBoosted/muTrack_jeta_2","muTrackPlots/muTrack_jeta_2.root","_inclusive",3,"Sub-Leading jet #eta","",false,"#tau_{#mu}#tau_{1 prong}","36.2",false,false,true);

   //makeLTauStack("~/www/ztt/2016/MuTau/XTTBoosted/muTrack_jpt_1","muTrackPlots/muTrack_jpt_1.root","_inclusive",3,"Leading jet Pt","GeV",false,"#tau_{#mu}#tau_{1 prong}","36.2",false,false,true);
   //makeLTauStack("~/www/ztt/2016/MuTau/XTTBoosted/muTrack_jpt_2","muTrackPlots/muTrack_jpt_2.root","_inclusive",3,"Sub-Leading jet Pt","GeV",false,"#tau_{#mu}#tau_{1 prong}","36.2",false,false,true);
}
