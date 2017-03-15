#include "makePlot.C"

void makeDatacards(){
   //makeLTauStack("~/www/ztt/2016/XTTDatacards/xtt_tt.inputs-13TeV-mt","datacards/xtt_tt.inputs-13TeV-mt.root","tt_inclusive",3,"m_{T}","GeV",false,"#tau_{h}#tau_{h}","36.8",false,false,false,true);
   makeLTauStack("~/www/ztt/2016/XTTDatacards/xtt_mt.inputs-13TeV-mt","datacards/xtt_mt.inputs-13TeV-mt.root","mt_inclusive",3,"m_{T}","GeV",false,"#mu#tau_{h}","36.8",false,false,true,200);
   makeLTauStack("~/www/ztt/2016/XTTDatacards/xtt_et.inputs-13TeV-mt","datacards/xtt_et.inputs-13TeV-mt.root","et_inclusive",3,"m_{T}","GeV",false,"e#tau_{h}","36.8",false,false,true,200);
   makeLTauStack("~/www/ztt/2016/XTTDatacards/xtt_tt.inputs-13TeV-mt","datacards/xtt_tt.inputs-13TeV-mt.root","tt_inclusive",3,"m_{T}","GeV",false,"#tau_{h}#tau_{h}","36.8",true,false,true,200);

}
