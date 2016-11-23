//`
#include "../../ROOT/plotMacros/makePlot2016.C"
//#include "makePlot2016.C"


void makeDataCardPlots(){
/*    
    makeLTauStack("~/www/ztt/2016/SMDatacards/et_0jet","datacards/htt_et.inputs-sm-13TeV-m_vis-1.root","et_0jet",3,"visible mass","GeV",false,"#tau_{e}#tau_{h}","ICHEP",false,false,true);
    makeLTauStack("~/www/ztt/2016/SMDatacards/et_boosted","datacards/htt_et.inputs-sm-13TeV-m_sv-2.root","et_boosted",3,"sv mass","GeV",false,"#tau_{e}#tau_{h}","ICHEP",false,false,true);
    makeLTauStack("~/www/ztt/2016/SMDatacards/et_vbf","datacards/htt_et.inputs-sm-13TeV-m_sv-3.root","et_vbf_low",3,"sv mass","GeV",false,"#tau_{e}#tau_{h}","ICHEP",false,false,true);

    makeLTauStack("~/www/ztt/2016/SMDatacards/mt_0jet","datacards/htt_mt.inputs-sm-13TeV-m_sv-1.root","mt_0jet",3,"visible mass","GeV",false,"#tau_{#mu}#tau_{h}","ICHEP",false,false,true);
    makeLTauStack("~/www/ztt/2016/SMDatacards/mt_boosted","datacards/htt_mt.inputs-sm-13TeV-m_sv-2.root","mt_boosted",3,"sv mass","GeV",false,"#tau_{#mu}#tau_{h}","ICHEP",false,false,true);
    makeLTauStack("~/www/ztt/2016/SMDatacards/mt_vbf","datacards/htt_mt.inputs-sm-13TeV-m_sv-3.root","mt_vbf_low",3,"sv mass","GeV",false,"#tau_{#mu}#tau_{h}","ICHEP",false,false,true);
*/
    makeLTauStack("~/www/ztt/2016/SMDatacards/mt_0jet_wjets","datacards/htt_mt.inputs-sm-13TeV-mt1-1-wjets.root","mt_0jet",3,"m_{T}","GeV",false,"#tau_{#mu}#tau_{h}","ICHEP",false,false,true);
    makeLTauStack("~/www/ztt/2016/SMDatacards/mt_boosted_wjets","datacards/htt_mt.inputs-sm-13TeV-mt1-2-wjets.root","mt_boosted",3,"m_{T}","GeV",false,"#tau_{#mu}#tau_{h}","ICHEP",false,false,true);
    makeLTauStack("~/www/ztt/2016/SMDatacards/mt_vbf_wjets","datacards/htt_mt.inputs-sm-13TeV-mt1-3-wjets.root","mt_vbf",3,"m_{T}","GeV",false,"#tau_{#mu}#tau_{h}","ICHEP",false,false,true);

    makeLTauStack("~/www/ztt/2016/SMDatacards/et_0jet_wjets","datacards/htt_et.inputs-sm-13TeV-mt1-1-wjets.root","et_0jet",3,"m_{T}","GeV",false,"#tau_{e}#tau_{h}","ICHEP",false,false,true);
    makeLTauStack("~/www/ztt/2016/SMDatacards/et_boosted_wjets","datacards/htt_et.inputs-sm-13TeV-mt1-2-wjets.root","et_boosted",3,"m_{T}","GeV",false,"#tau_{e}#tau_{h}","ICHEP",false,false,true);
    makeLTauStack("~/www/ztt/2016/SMDatacards/et_vbf_wjets","datacards/htt_et.inputs-sm-13TeV-mt1-3-wjets.root","et_vbf",3,"m_{T}","GeV",false,"#tau_{e}#tau_{h}","ICHEP",false,false,true);

    makeLTauStack("~/www/ztt/2016/SMDatacards/mt_0jet_antiiso","datacards/htt_mt.inputs-sm-13TeV-m_vis-1-antiiso.root","mt_0jet",3,"visible mass","GeV",false,"#tau_{#mu}#tau_{h}","ICHEP",false,false,true);
    makeLTauStack("~/www/ztt/2016/SMDatacards/mt_boosted_antiiso","datacards/htt_mt.inputs-sm-13TeV-m_sv-2-antiiso.root","mt_boosted",3,"sv mass","GeV",false,"#tau_{#mu}#tau_{h}","ICHEP",false,false,true);
    makeLTauStack("~/www/ztt/2016/SMDatacards/mt_vbf_antiiso","datacards/htt_mt.inputs-sm-13TeV-m_sv-3-antiiso.root","mt_vbf",3,"sv mass","GeV",false,"#tau_{#mu}#tau_{h}","ICHEP",false,false,true);

    makeLTauStack("~/www/ztt/2016/SMDatacards/et_0jet_antiiso","datacards/htt_et.inputs-sm-13TeV-m_vis-1-antiiso.root","et_0jet",3,"visible mass","GeV",false,"#tau_{e}#tau_{h}","ICHEP",false,false,true);
    makeLTauStack("~/www/ztt/2016/SMDatacards/et_boosted_antiiso","datacards/htt_et.inputs-sm-13TeV-m_sv-2-antiiso.root","et_boosted",3,"sv mass","GeV",false,"#tau_{e}#tau_{h}","ICHEP",false,false,true);
    makeLTauStack("~/www/ztt/2016/SMDatacards/et_vbf_antiiso","datacards/htt_et.inputs-sm-13TeV-m_sv-3-antiiso.root","et_vbf",3,"sv mass","GeV",false,"#tau_{e}#tau_{h}","ICHEP",false,false,true);


}
