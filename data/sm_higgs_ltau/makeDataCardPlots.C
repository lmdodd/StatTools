//`
#include "../../ROOT/plotMacros/makePlot2016.C"
//#include "makePlot2016.C"


void makeDataCardPlots(){

    makeLTauStack("~/www/ztt/2016/SMDatacards/mt_inclusive_m_vis","datacards/htt_mt.inputs-sm-13TeV-m_vis.root","mt_inclusive",3,"visible mass","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
    makeLTauStack("~/www/ztt/2016/SMDatacards/mt_inclusive_mt","datacards/htt_mt.inputs-sm-13TeV-mt.root","mt_inclusive",3,"Total m_{T}","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
    makeLTauStack("~/www/ztt/2016/SMDatacards/mt_0j_m_vis","datacards/htt_mt.inputs-sm-13TeV-m_vis.root","mt_0j",3,"visible mass","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
    makeLTauStack("~/www/ztt/2016/SMDatacards/mt_0j_mt","datacards/htt_mt.inputs-sm-13TeV-mt.root","mt_0j",3,"Total m_{T}","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
    makeLTauStack("~/www/ztt/2016/SMDatacards/mt_1j_m_vis","datacards/htt_mt.inputs-sm-13TeV-m_vis.root","mt_1j",3,"visible mass","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
    makeLTauStack("~/www/ztt/2016/SMDatacards/mt_1j_mt","datacards/htt_mt.inputs-sm-13TeV-mt.root","mt_1j",3,"Total m_{T}","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
    makeLTauStack("~/www/ztt/2016/SMDatacards/mt_vbf_m_vis","datacards/htt_mt.inputs-sm-13TeV-m_vis.root","mt_vbf",3,"visible mass","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
    makeLTauStack("~/www/ztt/2016/SMDatacards/mt_vbf_mt","datacards/htt_mt.inputs-sm-13TeV-mt.root","mt_vbf",3,"Total m_{T}","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);

    makeLTauStack("~/www/ztt/2016/SMDatacards/et_inclusive_m_vis","datacards/htt_et.inputs-sm-13TeV-m_vis.root","et_inclusive",3,"visible mass","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
    makeLTauStack("~/www/ztt/2016/SMDatacards/et_inclusive_mt","datacards/htt_et.inputs-sm-13TeV-mt.root","et_inclusive",3,"Total m_{T}","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
    makeLTauStack("~/www/ztt/2016/SMDatacards/et_0j_m_vis","datacards/htt_et.inputs-sm-13TeV-m_vis.root","et_0j",3,"visible mass","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
    makeLTauStack("~/www/ztt/2016/SMDatacards/et_0j_mt","datacards/htt_et.inputs-sm-13TeV-mt.root","et_0j",3,"Total m_{T}","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
    makeLTauStack("~/www/ztt/2016/SMDatacards/et_1j_m_vis","datacards/htt_et.inputs-sm-13TeV-m_vis.root","et_1j",3,"visible mass","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
    makeLTauStack("~/www/ztt/2016/SMDatacards/et_1j_mt","datacards/htt_et.inputs-sm-13TeV-mt.root","et_1j",3,"Total m_{T}","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
    makeLTauStack("~/www/ztt/2016/SMDatacards/et_vbf_m_vis","datacards/htt_et.inputs-sm-13TeV-m_vis.root","et_vbf",3,"visible mass","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
    makeLTauStack("~/www/ztt/2016/SMDatacards/et_vbf_mt","datacards/htt_et.inputs-sm-13TeV-mt.root","et_vbf",3,"Total m_{T}","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);

}
