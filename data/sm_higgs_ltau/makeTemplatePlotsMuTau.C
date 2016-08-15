{
   gROOT->ProcessLine(".L ../../../../UWAnalysis/ROOT/macros/plotScripts/templatePlots/makePlot2016.C");

//makeLTauStack(TString name,TString file,TString dir,int s,TString labelX,TString units = "GeV",bool left=false,TString channel = "",TString json = "Golden",bool log = false,bool dndm=false,bool doRatio = false)

   //makeLTauStack("~/www/Research/muTau_m_vis","muTauPlots/muTau_m_vis.root","mt_inclusive",0,"visible mass","GeV",false,"#tau_{#mu}#tau_{h}","Golden",true,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_m_vis","muTauPlots/muTau_m_vis.root","mt_inclusive",0,"visible mass","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_npv","muTauPlots/muTau_npv.root","mt_inclusive",0,"npv","",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_OldMVAIso","muTauPlots/muTau_OldMVAIso.root","mt_inclusive",0,"Raw MVAIsolation","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_tauIso","muTauPlots/muTau_tauIso.root","mt_inclusive",0,"Raw Tau Isolation","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_puCorrPtSum_2","muTauPlots/muTau_puCorrPtSum_2.root","mt_inclusive",0,"puCorrPtSum_2","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_neutralIsoPtSum_2","muTauPlots/muTau_neutralIsoPtSum_2.root","mt_inclusive",0,"neutralIsoPtSum_2","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_chargedIsoPtSum_2","muTauPlots/muTau_chargedIsoPtSum_2.root","mt_inclusive",0,"chargedIsoPtSum_2","GeV",false,"#tau_{#mu}#tau_{h}","Golden",true,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_chargedIsoPtSumdR03_2","muTauPlots/muTau_chargedIsoPtSumdR03_2.root","mt_inclusive",0,"chargedIsoPtSumdR03_2","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);

   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_n_photons_total","muTauPlots/muTau_n_photons_total.root","mt_inclusive",0,"n_photons_total","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_nIsoGamma","muTauPlots/muTau_nIsoGamma.root","mt_inclusive",0,"n Isolated Gamma","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_nIsoNeutral","muTauPlots/muTau_nIsoNeutral.root","mt_inclusive",0,"n Isolated Neutral","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);

   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_footprintCorrection_2","muTauPlots/muTau_footprintCorrection_2.root","mt_inclusive",0,"footprintCorrection_2","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_tau_pt_weighted_dr_signal","muTauPlots/muTau_tau_pt_weighted_dr_signal.root","mt_inclusive",0,"tau_pt_weighted_dr_signal","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_tau_pt_weighted_dr_iso","muTauPlots/muTau_tau_pt_weighted_dr_iso.root","mt_inclusive",0,"tau_pt_weighted_dr_iso","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_tau_pt_weighted_dphi_strip","muTauPlots/muTau_tau_pt_weighted_dphi_strip.root","mt_inclusive",0,"tau_pt_weighted_dphi_strip","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_tau_pt_weighted_deta_strip","muTauPlots/muTau_tau_pt_weighted_deta_strip.root","mt_inclusive",0,"tau_pt_weighted_deta_strip","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   //makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_photonPtSumOutsideSignalConedR03_2","muTauPlots/muTau_photonPtSumOutsideSignalConedR03_2.root","mt_inclusive",0,"photonPtSumOutsideSignalConedR03_2","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_photonPtSumOutsideSignalCone_2","muTauPlots/muTau_photonPtSumOutsideSignalCone_2.root","mt_inclusive",0,"photonPtSumOutsideSignalCone_2","GeV",false,"#tau_{#mu}#tau_{h}","Golden",true,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_photonPtSumOutsideSignalConedR03_2","muTauPlots/muTau_photonPtSumOutsideSignalConedR03_2.root","mt_inclusive",0,"photonPtSumOutsideSignalConedR03_2","GeV",false,"#tau_{#mu}#tau_{h}","Golden",true,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_neutralIsoPtSumWeight_2","muTauPlots/muTau_neutralIsoPtSumWeight_2.root","mt_inclusive",0,"neutralIsoPtSumWeight_2","GeV",false,"#tau_{#mu}#tau_{h}","Golden",true ,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_neutralIsoPtSumdR03_2","muTauPlots/muTau_neutralIsoPtSumdR03_2.root","mt_inclusive",0,"neutralIsoPtSumdR03_2","GeV",false,"#tau_{#mu}#tau_{h}","Golden",true,false,true);

   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_sumChargedHadronPt04_1","muTauPlots/muTau_sumChargedHadronPt04_1.root","mt_inclusive",0,"sumChargedHadronPt04_1","GeV",false,"#tau_{#mu}#tau_{h}","Golden",true,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_sumChargedHadronPt03_1","muTauPlots/muTau_sumChargedHadronPt03_1.root","mt_inclusive",0,"sumChargedHadronPt03_1","GeV",false,"#tau_{#mu}#tau_{h}","Golden",true,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_sumNeutralHadronPt04_1","muTauPlots/muTau_sumNeutralHadronPt04_1.root","mt_inclusive",0,"sumNeutralHadronPt04_1","GeV",false,"#tau_{#mu}#tau_{h}","Golden",true,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_sumNeutralHadronPt03_1","muTauPlots/muTau_sumNeutralHadronPt03_1.root","mt_inclusive",0,"sumNeutralHadronPt03_1","GeV",false,"#tau_{#mu}#tau_{h}","Golden",true,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_sumPhotonPt04_1","muTauPlots/muTau_sumPhotonPt04_1.root","mt_inclusive",0,"sumPhotonPt04_1","GeV",false,"#tau_{#mu}#tau_{h}","Golden",true,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_sumPUPt04_1","muTauPlots/muTau_sumPUPt04_1.root","mt_inclusive",0,"sumPUPt04_1","GeV",false,"#tau_{#mu}#tau_{h}","Golden",true,false,true);

   
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_mt_1","muTauPlots/muTau_mt_1.root","mt_inclusive",0,"m_{T}","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_phi_1","muTauPlots/muTau_phi_1.root","mt_inclusive",0,"#mu #phi","",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_phi_2","muTauPlots/muTau_phi_2.root","mt_inclusive",0,"#tau #phi","",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_mt_2","muTauPlots/muTau_mt_2.root","mt_inclusive",0,"#mu +MET m_{T}","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_mt","muTauPlots/muTau_mt.root","mt_inclusive",0,"Total m_{T}","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   //makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_m_vis_nocut","muTauPlots/muTau_m_vis_nocut.root","mt_inclusive",0,"visible mass","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_iso_1","muTauPlots/muTau_iso_1.root","mt_inclusive",0,"Isolation #mu DR=0.3","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_iso04_1","muTauPlots/muTau_iso04_1.root","mt_inclusive",0,"Isolation #mu DR=0.4","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_met","muTauPlots/muTau_met.root","mt_inclusive",0,"ME_{T}","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_mvamet","muTauPlots/muTau_mvamet.root","mt_inclusive",0,"MVA ME_{T}","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_mvamt1","muTauPlots/muTau_mvamt_1.root","mt_inclusive",0,"MVA M_{T}","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_puppimet","muTauPlots/muTau_puppimet.root","mt_inclusive",0,"puppi ME_{T}","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_puppimt1","muTauPlots/muTau_puppimt_1.root","mt_inclusive",0,"puppi M_{T}","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_pt_1","muTauPlots/muTau_pt_1.root","mt_inclusive",0,"#mu P_{T} ","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   //makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_genVisPt1","muTauPlots/muTau_genVisPt1.root","mt_inclusive",0,"Gen Visible #mu P_{T}","GeV",false,"#tau_{#mu}#tau_{h}","Golden",true,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_pt_2","muTauPlots/muTau_pt_2.root","mt_inclusive",0,"#tau P_{T}","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   //makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_genVisPt2","muTauPlots/muTau_genVisPt2.root","mt_inclusive",0,"Gen Visible #tau P_{T}","GeV",false,"#tau_{#mu}#tau_{h}","Golden",true,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_eta_1","muTauPlots/muTau_eta_1.root","mt_inclusive",0,"#eta_{#mu}","",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_eta_2","muTauPlots/muTau_eta_2.root","mt_inclusive",0,"#eta_{#tau}","",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_m_vis","muTauPlots/muTau_m_vis.root","mt_inclusive",0,"visible mass","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_pt","muTauPlots/muTau_pt.root","mt_inclusive",0,"di-tau Pt","GeV",false,"#tau_{#mu}#tau_{h}","Golden",true,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_q_1","muTauPlots/muTau_q_1.root","mt_inclusive",0,"q_{#mu}","",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_q_2","muTauPlots/muTau_q_2.root","mt_inclusive",0,"q_{#tau}","",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);

   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_nCands","muTauPlots/muTau_nCands.root","mt_inclusive",0,"nCands","",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_tauDecayMode","muTauPlots/muTau_tauDecayMode.root","mt_inclusive",0,"#tau decay mode","",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_m_2","muTauPlots/muTau_m_2.root","mt_inclusive",0,"M_{#tau}","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_dxy_2","muTauPlots/muTau_dxy_2.root","mt_inclusive",0,"dxy_2","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_dxy_Sig_2","muTauPlots/muTau_dxy_Sig_2.root","mt_inclusive",0,"dxy_Sig_2","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_flightLength_2","muTauPlots/muTau_flightLength_2.root","mt_inclusive",0,"flightLength_2","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_flightLengthSig_2","muTauPlots/muTau_flightLengthSig_2.root","mt_inclusive",0,"flightLengthSig_2","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_mjj","muTauPlots/muTau_mjj.root","mt_inclusive",0,"M_{JJ}","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_njetspt20","muTauPlots/muTau_njetspt20.root","mt_inclusive",0,"Number Jets Pt>20","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_njets","muTauPlots/muTau_njets.root","mt_inclusive",0,"Number Jets Pt>30","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   //makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_nbtag","muTauPlots/muTau_nbtag.root","mt_inclusive",0,"Number Jets Pt>30","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_jeta_1","muTauPlots/muTau_jeta_1.root","mt_inclusive",0,"Leading jet #eta","",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_jeta_2","muTauPlots/muTau_jeta_2.root","mt_inclusive",0,"Sub-Leading jet #eta","",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_tauNIsoTracks","muTauPlots/muTau_tauNIsoTracks.root","mt_inclusive",0,"Number Isolated Tracks","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);

   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_jpt_1","muTauPlots/muTau_jpt_1.root","mt_inclusive",0,"Leading jet Pt","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/MuTau/HTT_BkgdMethod6/muTau_jpt_2","muTauPlots/muTau_jpt_2.root","mt_inclusive",0,"Sub-Leading jet Pt","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);

}
