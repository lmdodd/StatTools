{
   gROOT->ProcessLine(".L ../../../../UWAnalysis/ROOT/macros/plotScripts/templatePlots/makePlot2016.C");

   //makeLTauStack(TString name,TString file,TString dir,int s,TString labelX,TString units = "GeV",bool left=false,TString channel = "",TString year = "2012",bool log = false,bool dndm=false,bool doRatio = false,true)
   //makeLTauStackPF("~/www/ztt/2016/ETau/HTT_BkgdMethod5/eleTau/MSSM8TeV/X/eleTau_highestJetEta","eleTauPlots/MSSM/eleTau_highestJetEta.root","eleTau_X",1,"Jet_{1} #eta","",true,"#tau_{#ele}#tau_{h}","2012",false,true,1,1,1,1,1,1,1,1,false,true);

   //makeLTauStack("~/www/ztt/2016/ETau/HTT_BkgdMethod5/eleTau_LLmass","eleTauPlots/eleTau_LLmass.root","et_inclusive",0,"di-lepton mass","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/HTT_BkgdMethod5/eleTau_m_vis","eleTauPlots/eleTau_m_vis.root","et_inclusive",3,"visible m_vis","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/HTT_BkgdMethod5/eleTau_phi_1","eleTauPlots/eleTau_phi1.root","et_inclusive",0,"phi 1","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/HTT_BkgdMethod5/eleTau_phi_2","eleTauPlots/eleTau_phi2.root","et_inclusive",0,"phi 2","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/HTT_BkgdMethod5/eleTau_relIso","eleTauPlots/eleTau_relIso.root","et_inclusive",0,"(charged+neutral)/pt","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/HTT_BkgdMethod5/eleTau_chIso","eleTauPlots/eleTau_chIso.root","et_inclusive",0,"Charged #tau Iso","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/HTT_BkgdMethod5/eleTau_neuIso","eleTauPlots/eleTau_neuIso.root","et_inclusive",0,"Neutral #tau Iso","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/HTT_BkgdMethod5/eleTau_puIso","eleTauPlots/eleTau_puIso.root","et_inclusive",0,"PU #tau Iso","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/HTT_BkgdMethod5/eleTau_mt_1","eleTauPlots/eleTau_mt_1.root","et_inclusive",0,"m_{T}","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/HTT_BkgdMethod5/eleTau_puppimt_1","eleTauPlots/eleTau_puppimt_1.root","et_inclusive",0,"puppi m_{T}","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/HTT_BkgdMethod5/eleTau_mvamt_1","eleTauPlots/eleTau_mvamt_1.root","et_inclusive",0,"MVA m_{T}","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/HTT_BkgdMethod5/eleTau_mt_2","eleTauPlots/eleTau_mt_2.root","et_inclusive",0,"e +MET m_{T}","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/HTT_BkgdMethod5/eleTau_mt_2_nocut","eleTauPlots/eleTau_mt_2_nocut.root","et_inclusive",0,"e +MET m_{T}(no mt cut)","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/HTT_BkgdMethod5/eleTau_iso_1","eleTauPlots/eleTau_iso_1.root","et_inclusive",0,"e iso dR=0.3","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/HTT_BkgdMethod5/eleTau_iso04_1","eleTauPlots/eleTau_iso04_1.root","et_inclusive",0,"e iso dR=0.4","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/HTT_BkgdMethod5/eleTau_met","eleTauPlots/eleTau_met.root","et_inclusive",0,"ME_{T}","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/HTT_BkgdMethod5/eleTau_mvamet","eleTauPlots/eleTau_mvamet.root","et_inclusive",0,"MVA ME_{T}","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/HTT_BkgdMethod5/eleTau_puppimet","eleTauPlots/eleTau_puppimet.root","et_inclusive",0,"puppi ME_{T}","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/HTT_BkgdMethod5/eleTau_mt","eleTauPlots/eleTau_mt.root","et_inclusive",0,"Total M_{T}","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/HTT_BkgdMethod5/eleTau_pt_1","eleTauPlots/eleTau_pt_1.root","et_inclusive",0,"e P_{T} ","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/HTT_BkgdMethod5/eleTau_tauIso","eleTauPlots/eleTau_tauIso.root","et_inclusive",0,"e tauIso","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   //makeLTauStack("~/www/ztt/2016/ETau/HTT_BkgdMethod5/eleTau_genVisPt1","eleTauPlots/eleTau_genVisPt1.root","et_inclusive",0,"Gen Visible e P_{T}","GeV",false,"#tau_{e}#tau_{h}","Golden",true,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/HTT_BkgdMethod5/eleTau_pt_2","eleTauPlots/eleTau_pt_2.root","et_inclusive",0,"#tau P_{T}","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   //makeLTauStack("~/www/ztt/2016/ETau/HTT_BkgdMethod5/eleTau_genVisPt2","eleTauPlots/eleTau_genVisPt2.root","et_inclusive",0,"Gen Visible #tau P_{T}","GeV",false,"#tau_{e}#tau_{h}","Golden",true,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/HTT_BkgdMethod5/eleTau_eta_1","eleTauPlots/eleTau_eta_1.root","et_inclusive",0,"#eta_{e}","",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/HTT_BkgdMethod5/eleTau_eta_2","eleTauPlots/eleTau_eta_2.root","et_inclusive",0,"#eta_{#tau}","",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/HTT_BkgdMethod5/eleTau_jeta_1","eleTauPlots/eleTau_jeta_1.root","et_inclusive",0,"Leading Jet #eta","",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/HTT_BkgdMethod5/eleTau_jeta_2","eleTauPlots/eleTau_jeta_2.root","et_inclusive",0,"Sub-Leading Jet #eta","",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/HTT_BkgdMethod5/eleTau_pth","eleTauPlots/eleTau_pth.root","et_inclusive",0,"Higgs Pt","GeV",false,"#tau_{e}#tau_{h}","Golden",true,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/HTT_BkgdMethod5/eleTau_q_1","eleTauPlots/eleTau_q_1.root","et_inclusive",0,"q_{e}","",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/HTT_BkgdMethod5/eleTau_q_2","eleTauPlots/eleTau_q_2.root","et_inclusive",0,"q_{#tau}","",false,"#tau_{e}#tau_{h}","Golden",false,false,true);

   makeLTauStack("~/www/ztt/2016/ETau/HTT_BkgdMethod5/eleTau_OldMVAIso","eleTauPlots/eleTau_OldMVAIso.root","et_inclusive",0,"OldMVAIso","",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/HTT_BkgdMethod5/eleTau_vertices","eleTauPlots/eleTau_vertices.root","et_inclusive",0,"vertices","",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/HTT_BkgdMethod5/eleTau_npv","eleTauPlots/eleTau_npvs.root","et_inclusive",0,"npv","",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/HTT_BkgdMethod5/eleTau_nCands","eleTauPlots/eleTau_nCands.root","et_inclusive",0,"nCand pairs","",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/HTT_BkgdMethod5/eleTau_tauDecayMode","eleTauPlots/eleTau_tauDecayMode.root","et_inclusive",0,"#tau decay mode","",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/HTT_BkgdMethod5/eleTau_tauZIP","eleTauPlots/eleTau_tauZIP.root","et_inclusive",0,"#tau ZIP","",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/HTT_BkgdMethod5/eleTau_tauNIsoTracks","eleTauPlots/eleTau_tauNIsoTracks.root","et_inclusive",0,"Number Isolated tracks","",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/HTT_BkgdMethod5/eleTau_m_2","eleTauPlots/eleTau_m_2.root","et_inclusive",0,"M_{#tau}","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/HTT_BkgdMethod5/eleTau_mjj","eleTauPlots/eleTau_mjj.root","et_inclusive",0,"M_{JJ}","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/HTT_BkgdMethod5/eleTau_njetspt20","eleTauPlots/eleTau_njetspt20.root","et_inclusive",0,"Number Jets Pt>20","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/HTT_BkgdMethod5/eleTau_njets","eleTauPlots/eleTau_njets.root","et_inclusive",0,"Number Jets Pt>30","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/HTT_BkgdMethod5/eleTau_nbtag","eleTauPlots/eleTau_nbtag.root","et_inclusive",0,"Number CISV Medium Jets Pt>20","",false,"#tau_{e}#tau_{h}","Golden",true,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/HTT_BkgdMethod5/eleTau_jpt_1","eleTauPlots/eleTau_jpt_1.root","et_inclusive",0,"Leading Jet Pt","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/HTT_BkgdMethod5/eleTau_jpt_2","eleTauPlots/eleTau_jpt_2.root","et_inclusive",0,"Sub-Leading Jet Pt","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
}


