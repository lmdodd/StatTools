void renameDataCard(TString inputFile, TString outputFile, TString originalDatacard, TString newDatacard) {

    TFile f1(inputFile);
    TFile *f2 = new TFile(outputFile,"Update");
    
    f1.cd(originalDatacard);
    TDirectory *old = gDirectory;
    old->ReadAll();
    
    f2->cd();
    f2->mkdir(newDatacard);
    f2->cd(newDatacard);
    old->GetList()->Write();
    
    f2->Close();

}
