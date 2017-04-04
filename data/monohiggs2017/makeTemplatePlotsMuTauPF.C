#include "makePostfit.C"

void makeTemplatePlotsMuTauPF(){

   makeLTauStack("~/www/ztt/2016/tauTau_mt_PF","xtt_tt_shapes.root","xtt_tt_1_13TeV_postfit",0,"Total m_{T}","GeV",false,"#tau_{#tau_{h}}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/tauTau_mt_Pre","xtt_tt_shapes.root","xtt_tt_1_13TeV_prefit",0,"Total m_{T}","GeV",false,"#tau_{#tau_{h}}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/muTau_mt_PF","xtt_mt_shapes.root","xtt_mt_1_13TeV_postfit",0,"Total m_{T}","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/muTau_mt_Pre","xtt_mt_shapes.root","xtt_mt_1_13TeV_prefit",0,"Total m_{T}","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/eleTau_mt_PF","xtt_et_shapes.root","xtt_et_1_13TeV_postfit",0,"Total m_{T}","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/2016/eleTau_mt_Pre","xtt_et_shapes.root","xtt_et_1_13TeV_prefit",0,"Total m_{T}","GeV",false,"#tau_{e}#tau_{h}","Golden",false,false,true);

}
