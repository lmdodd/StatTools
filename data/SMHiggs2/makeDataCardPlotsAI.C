//`
#include "../../ROOT/plotMacros/makePlot2016.C"
//#include "makePlot2016.C"


void makeDataCardPlotsAI(){

    makeLTauStack("~/www/ztt/2016/SMDatacardsAI/et_0jet","datacards/htt_et.inputs-sm-13TeV-m_vis-1-antiiso.root","et_0jet",3,"visible mass","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
    makeLTauStack("~/www/ztt/2016/SMDatacardsAI/et_boosted","datacards/htt_et.inputs-sm-13TeV-m_sv-2-antiiso.root","et_boosted",3,"sv mass","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
    makeLTauStack("~/www/ztt/2016/SMDatacardsAI/et_vbf","datacards/htt_et.inputs-sm-13TeV-m_sv-3-antiiso.root","et_vbf",3,"sv mass","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);

    makeLTauStack("~/www/ztt/2016/SMDatacardsAI/mt_0jet","datacards/htt_mt.inputs-sm-13TeV-m_vis-1-antiiso.root","mt_0jet",3,"visible mass","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
    makeLTauStack("~/www/ztt/2016/SMDatacardsAI/mt_boosted","datacards/htt_mt.inputs-sm-13TeV-m_sv-2-antiiso.root","mt_boosted",3,"sv mass","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
    makeLTauStack("~/www/ztt/2016/SMDatacardsAI/mt_vbf","datacards/htt_mt.inputs-sm-13TeV-m_sv-3-antiiso.root","mt_vbf",3,"sv mass","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);




}
