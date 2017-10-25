void generate_data(int factor=1)
{
    TFile* ofile = new TFile("bdt_input_data.root", "recreate");
    double x, y;
    float xx, yy;
    TTree* signal_tree = new TTree("signal_tree", "signal_tree");
    signal_tree->Branch("x", &xx, "x/F"); 
    signal_tree->Branch("y", &yy, "y/F"); 
    TF2* sig_pdf = new TF2("sig_pdf", Form("1/(%d*x*y+1)", factor), 0, 1, 0, 1);
    TH2F* sig_hist = new TH2F("sig_hist", "sig_hist", 50, 0, 1, 50, 0, 1);
    for (int i = 0; i < 1000000; ++i)
    {
        sig_pdf->GetRandom2(x, y);
        xx = x;
        yy = y;
        sig_hist->Fill(x, y);
        signal_tree->Fill();
    }
    signal_tree->Write();
    TTree* background_tree = new TTree("background_tree", "background_tree");
    background_tree->Branch("x", &xx, "x/F"); 
    background_tree->Branch("y", &yy, "y/F"); 
    TF2* bkg_pdf = new TF2("bkg_pdf", Form("1/(%d*(x-1)*(y-1)+1)", factor), 0, 1, 0, 1);
    bkg_pdf->Draw();
    TH2F* bkg_hist = new TH2F("bkg_hist", "bkg_hist", 50, 0, 1, 50, 0, 1);
    for (int i = 0; i < 1000000; ++i)
    {
        bkg_pdf->GetRandom2(x, y);
        xx = x;
        yy = y;
        bkg_hist->Fill(x, y);
        background_tree->Fill();
    }
    background_tree->Write();
    sig_hist->Write();
    bkg_hist->Write();
    ofile->Close();
}
