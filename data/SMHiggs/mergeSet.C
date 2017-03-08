#include "renameDataCard.C"

void mergeSet(){   
    renameDataCard("datacards/htt_et.inputs-sm-13TeV-m_vis-1-antiiso.root","datacardsAdvanced/htt_et.inputs-sm-13TeV.root","et_0jet","et_antiiso_0jet_cr");
    renameDataCard("datacards/htt_et.inputs-sm-13TeV-m_sv-2-antiiso.root","datacardsAdvanced/htt_et.inputs-sm-13TeV.root","et_boosted","et_antiiso_boosted_cr");
    renameDataCard("datacards/htt_et.inputs-sm-13TeV-m_sv-3-antiiso.root","datacardsAdvanced/htt_et.inputs-sm-13TeV.root","et_vbf","et_antiiso_vbf_cr");

    renameDataCard("datacards/htt_et.inputs-sm-13TeV-mt1-1-wjets.root","datacardsAdvanced/htt_et.inputs-sm-13TeV.root","et_0jet","et_wjets_0jet_cr");
    renameDataCard("datacards/htt_et.inputs-sm-13TeV-mt1-2-wjets.root","datacardsAdvanced/htt_et.inputs-sm-13TeV.root","et_boosted","et_wjets_boosted_cr");
    renameDataCard("datacards/htt_et.inputs-sm-13TeV-mt1-3-wjets.root","datacardsAdvanced/htt_et.inputs-sm-13TeV.root","et_vbf","et_wjets_vbf_cr");

    renameDataCard("datacards/htt_mt.inputs-sm-13TeV-m_vis-1-antiiso.root","datacardsAdvanced/htt_mt.inputs-sm-13TeV.root","mt_0jet","mt_antiiso_0jet_cr");
    renameDataCard("datacards/htt_mt.inputs-sm-13TeV-m_sv-2-antiiso.root","datacardsAdvanced/htt_mt.inputs-sm-13TeV.root","mt_boosted","mt_antiiso_boosted_cr");
    renameDataCard("datacards/htt_mt.inputs-sm-13TeV-m_sv-3-antiiso.root","datacardsAdvanced/htt_mt.inputs-sm-13TeV.root","mt_vbf","mt_antiiso_vbf_cr");

    renameDataCard("datacards/htt_mt.inputs-sm-13TeV-mt1-1-wjets.root","datacardsAdvanced/htt_mt.inputs-sm-13TeV.root","mt_0jet","mt_wjets_0jet_cr");
    renameDataCard("datacards/htt_mt.inputs-sm-13TeV-mt1-2-wjets.root","datacardsAdvanced/htt_mt.inputs-sm-13TeV.root","mt_boosted","mt_wjets_boosted_cr");
    renameDataCard("datacards/htt_mt.inputs-sm-13TeV-mt1-3-wjets.root","datacardsAdvanced/htt_mt.inputs-sm-13TeV.root","mt_vbf","mt_wjets_vbf_cr");

    renameDataCard("datacards/htt_mt.inputs-sm-13TeV-m_vis-1-2D.root","datacardsAdvanced/htt_mt.inputs-sm-13TeV.root","mt_0jet","mt_0jet");
    renameDataCard("datacards/htt_mt.inputs-sm-13TeV-m_sv-2-2D.root","datacardsAdvanced/htt_mt.inputs-sm-13TeV.root","mt_boosted","mt_boosted");
    renameDataCard("datacards/htt_mt.inputs-sm-13TeV-m_sv-3-2D.root","datacardsAdvanced/htt_mt.inputs-sm-13TeV.root","mt_vbf","mt_vbf");

    renameDataCard("datacards/htt_et.inputs-sm-13TeV-m_vis-1-2D.root","datacardsAdvanced/htt_et.inputs-sm-13TeV.root","et_0jet","et_0jet");
    renameDataCard("datacards/htt_et.inputs-sm-13TeV-m_sv-2-2D.root","datacardsAdvanced/htt_et.inputs-sm-13TeV.root","et_boosted","et_boosted");
    renameDataCard("datacards/htt_et.inputs-sm-13TeV-m_sv-3-2D.root","datacardsAdvanced/htt_et.inputs-sm-13TeV.root","et_vbf","et_vbf");


}
