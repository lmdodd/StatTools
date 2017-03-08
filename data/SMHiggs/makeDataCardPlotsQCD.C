//`
#include "../../ROOT/plotMacros/makePlot2016.C"
//#include "makePlot2016.C"


void makeDataCardPlotsQCD(){

    makeLTauStack("~/www/ztt/2016/QCDAI/et_0jet","qcddatacards/htt_et.inputs-sm-13TeV-m_vis-1-antiiso-NOSF.root","et_0jet",3,"visible mass","GeV",false,"#tau_{e}#tau_{h}","ICHEP",false,false,true);
    makeLTauStack("~/www/ztt/2016/QCDAI/et_boosted","qcddatacards/htt_et.inputs-sm-13TeV-m_sv-2-antiiso-NOSF.root","et_boosted",3,"sv mass","GeV",false,"#tau_{e}#tau_{h}","ICHEP",false,false,true);
    makeLTauStack("~/www/ztt/2016/QCDAI/et_vbf","qcddatacards/htt_et.inputs-sm-13TeV-m_sv-3-antiiso-NOSF.root","et_vbf",3,"sv mass","GeV",false,"#tau_{e}#tau_{h}","ICHEP",false,false,true);

    makeLTauStack("~/www/ztt/2016/QCDAI/mt_0jet","qcddatacards/htt_mt.inputs-sm-13TeV-m_vis-1-antiiso-NOSF.root","mt_0jet",3,"visible mass","GeV",false,"#tau_{#mu}#tau_{h}","ICHEP",false,false,true);
    makeLTauStack("~/www/ztt/2016/QCDAI/mt_boosted","qcddatacards/htt_mt.inputs-sm-13TeV-m_sv-2-antiiso-NOSF.root","mt_boosted",3,"sv mass","GeV",false,"#tau_{#mu}#tau_{h}","ICHEP",false,false,true);
    makeLTauStack("~/www/ztt/2016/QCDAI/mt_vbf","qcddatacards/htt_mt.inputs-sm-13TeV-m_sv-3-antiiso-NOSF.root","mt_vbf",3,"sv mass","GeV",false,"#tau_{#mu}#tau_{h}","ICHEP",false,false,true);


}
