#!/bin/sh
python makeSignalZp.py muTauPlots/muTau_nbtagNoSF.root MuTau/signal mt_inclusive nbjets 'nbjets' Zp500
python makeSignalZp.py muTauPlots/muTau_njets.root MuTau/signal mt_inclusive njets 'njets' Zp500
python makeSignalZp.py muTauPlots/muTau_mt_1.root MuTau/signal mt_inclusive mt_1 'M_{T} MET+#mu' Zp500
python makeSignalZp.py muTauPlots/muTau_mt_2.root MuTau/signal mt_inclusive mt_2 'M_{T} MET+#tau' Zp500
python makeSignalZp.py muTauPlots/muTau_mt.root MuTau/signal mt_inclusive mt 'Total M_{T}' Zp500
python makeSignalZp.py muTauPlots/muTau_met.root MuTau/signal mt_inclusive met 'Missing E_{T} (pfmet type 1)' Zp500
python makeSignalZp.py muTauPlots/muTau_m_vis.root MuTau/signal mt_inclusive m_vis 'Visible Mass #mu#tau pair' Zp500
python makeSignalZp.py muTauPlots/muTau_dR.root MuTau/signal mt_inclusive dR '#mu#tau pair #Delta R' Zp500
python makeSignalZp.py muTauPlots/muTau_pt.root MuTau/signal mt_inclusive pt '#mu#tau pair p_T' Zp500

python makeSignalZp.py muTauPlots/muTau_nbtagNoSF.root MuTau/signal mt_inclusive nbjets 'nbjets' Zp1000
python makeSignalZp.py muTauPlots/muTau_njets.root MuTau/signal mt_inclusive njets 'njets' Zp1000
python makeSignalZp.py muTauPlots/muTau_mt_1.root MuTau/signal mt_inclusive mt_1 'M_{T} MET+#mu' Zp1000
python makeSignalZp.py muTauPlots/muTau_mt_2.root MuTau/signal mt_inclusive mt_2 'M_{T} MET+#tau' Zp1000
python makeSignalZp.py muTauPlots/muTau_mt.root MuTau/signal mt_inclusive mt 'Total M_{T}' Zp1000
python makeSignalZp.py muTauPlots/muTau_met.root MuTau/signal mt_inclusive met 'Missing E_{T} (pfmet type 1)' Zp1000
python makeSignalZp.py muTauPlots/muTau_m_vis.root MuTau/signal mt_inclusive m_vis 'Visible Mass #mu#tau pair' Zp1000
python makeSignalZp.py muTauPlots/muTau_dR.root MuTau/signal mt_inclusive dR '#mu#tau pair #Delta R' Zp1000
python makeSignalZp.py muTauPlots/muTau_pt.root MuTau/signal mt_inclusive pt '#mu#tau pair p_T' Zp1000

