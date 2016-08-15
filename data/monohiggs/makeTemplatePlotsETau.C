{
   gROOT->ProcessLine(".L makePlot.C");

   //makeLTauStack(TString name,TString file,TString dir,int s,TString labelX,TString units = "GeV",bool left=false,TString channel = "",TString year = "2012",bool log = false,bool dndm=false,bool doRatio = false,true)
   //makeLTauStackPF("~/www/ztt/2016/ETau/XTT/eleTau/MSSM8TeV/X/eleTau_highestJetEta","eleTauPlots/MSSM/eleTau_highestJetEta.root","eleTau_X",1,"Jet_{1} #eta","",true,"#tau_{#ele}#tau_{h}","2012",false,true,1,1,1,1,1,1,1,1,false,true);

   makeLTauStack("~/www/ztt/2016/ETau/XTT/eleTau_m_vis","eleTauPlots/eleTau_m_vis.root","et_inclusive",3,"visible m_vis","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
//}
   //makeLTauStack("~/www/ztt/2016/ETau/XTT/eleTau_LLmass","eleTauPlots/eleTau_LLmass.root","et_inclusive",3,"di-lepton mass","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT/eleTau_phi_1","eleTauPlots/eleTau_phi1.root","et_inclusive",3,"phi 1","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT/eleTau_phi_2","eleTauPlots/eleTau_phi2.root","et_inclusive",3,"phi 2","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT/eleTau_relIso","eleTauPlots/eleTau_relIso.root","et_inclusive",3,"(charged+neutral)/pt","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT/eleTau_chIso","eleTauPlots/eleTau_chIso.root","et_inclusive",3,"Charged #tau Iso","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT/eleTau_neuIso","eleTauPlots/eleTau_neuIso.root","et_inclusive",3,"Neutral #tau Iso","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT/eleTau_puIso","eleTauPlots/eleTau_puIso.root","et_inclusive",3,"PU #tau Iso","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT/eleTau_mt_1","eleTauPlots/eleTau_mt_1.root","et_inclusive",3,"m_{T}","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT/eleTau_mt_2","eleTauPlots/eleTau_mt_2.root","et_inclusive",3,"#tau+MET m_{T}","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT/eleTau_mt_2_nocut","eleTauPlots/eleTau_mt_2_nocut.root","et_inclusive",3,"e +MET m_{T}(no mt cut)","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT/eleTau_iso_1","eleTauPlots/eleTau_iso_1.root","et_inclusive",3,"e iso dR=0.3","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT/eleTau_iso04_1","eleTauPlots/eleTau_iso04_1.root","et_inclusive",3,"e iso dR=0.4","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT/eleTau_met","eleTauPlots/eleTau_met.root","et_inclusive",3,"ME_{T}","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT/eleTau_mt","eleTauPlots/eleTau_mt.root","et_inclusive",3,"Total M_{T}","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT/eleTau_pt_1","eleTauPlots/eleTau_pt_1.root","et_inclusive",3,"e P_{T} ","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT/eleTau_tauIso","eleTauPlots/eleTau_tauIso.root","et_inclusive",3,"e tauIso","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   //makeLTauStack("~/www/ztt/2016/ETau/XTT/eleTau_genVisPt1","eleTauPlots/eleTau_genVisPt1.root","et_inclusive",3,"Gen Visible e P_{T}","GeV",false,"#tau_{e}#tau_{h}","Golden",true,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT/eleTau_pt_2","eleTauPlots/eleTau_pt_2.root","et_inclusive",3,"#tau P_{T}","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   //makeLTauStack("~/www/ztt/2016/ETau/XTT/eleTau_genVisPt2","eleTauPlots/eleTau_genVisPt2.root","et_inclusive",3,"Gen Visible #tau P_{T}","GeV",false,"#tau_{e}#tau_{h}","Golden",true,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT/eleTau_eta_1","eleTauPlots/eleTau_eta_1.root","et_inclusive",3,"#eta_{e}","",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT/eleTau_eta_2","eleTauPlots/eleTau_eta_2.root","et_inclusive",3,"#eta_{#tau}","",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT/eleTau_jeta_1","eleTauPlots/eleTau_jeta_1.root","et_inclusive",3,"Leading Jet #eta","",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT/eleTau_jeta_2","eleTauPlots/eleTau_jeta_2.root","et_inclusive",3,"Sub-Leading Jet #eta","",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT/eleTau_pth","eleTauPlots/eleTau_pth.root","et_inclusive",3,"e-#tau Pt","GeV",false,"#tau_{e}#tau_{h}","Golden",true,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT/eleTau_q_1","eleTauPlots/eleTau_q_1.root","et_inclusive",3,"q_{e}","",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT/eleTau_q_2","eleTauPlots/eleTau_q_2.root","et_inclusive",3,"q_{#tau}","",false,"#tau_{e}#tau_{h}","Golden",false,false,true);

   makeLTauStack("~/www/ztt/2016/ETau/XTT/eleTau_OldMVAIso","eleTauPlots/eleTau_OldMVAIso.root","et_inclusive",3,"OldMVAIso","",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT/eleTau_vertices","eleTauPlots/eleTau_vertices.root","et_inclusive",3,"vertices","",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT/eleTau_npv","eleTauPlots/eleTau_npv.root","et_inclusive",3,"npv","",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT/eleTau_nCands","eleTauPlots/eleTau_nCands.root","et_inclusive",3,"nCand pairs","",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT/eleTau_tauDecayMode","eleTauPlots/eleTau_tauDecayMode.root","et_inclusive",3,"#tau decay mode","",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT/eleTau_tauZIP","eleTauPlots/eleTau_tauZIP.root","et_inclusive",3,"#tau ZIP","",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT/eleTau_tauNIsoTracks","eleTauPlots/eleTau_tauNIsoTracks.root","et_inclusive",3,"Number Isolated tracks","",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT/eleTau_m_2","eleTauPlots/eleTau_m_2.root","et_inclusive",3,"M_{#tau}","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT/eleTau_mjj","eleTauPlots/eleTau_mjj.root","et_inclusive",3,"M_{JJ}","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT/eleTau_njetspt20","eleTauPlots/eleTau_njetspt20.root","et_inclusive",3,"Number Jets Pt>20","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT/eleTau_njets","eleTauPlots/eleTau_njets.root","et_inclusive",3,"Number Jets Pt>30","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT/eleTau_nbtag","eleTauPlots/eleTau_nbtag.root","et_inclusive",3,"Number CISV Medium Jets Pt>20","",false,"#tau_{e}#tau_{h}","Golden",true,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT/eleTau_jpt_1","eleTauPlots/eleTau_jpt_1.root","et_inclusive",3,"Leading Jet Pt","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/ETau/XTT/eleTau_jpt_2","eleTauPlots/eleTau_jpt_2.root","et_inclusive",3,"Sub-Leading Jet Pt","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
}


