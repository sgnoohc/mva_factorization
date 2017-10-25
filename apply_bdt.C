void apply_bdt()
{
    TMVA::Reader* reader = new TMVA::Reader("!Color:!Silent");
    float xx, yy, mva;
    reader->AddVariable("x", &xx);
    reader->AddVariable("y", &yy);
    reader->BookMVA("BDT", "weights/TMVA_BDT.weights.xml");

    TFile* ofile = new TFile("bdt_data.root", "recreate");
    double x, y;
    TTree* signal_tree = new TTree("signal_tree", "signal_tree");
    signal_tree->Branch("x", &xx, "x/F"); 
    signal_tree->Branch("y", &yy, "y/F"); 
    signal_tree->Branch("mva", &mva, "mva/F"); 
    TF2* sig_pdf = new TF2("sig_pdf", "1/(x*y+1)", 0, 1, 0, 1);
    ofstream sig_data;
    sig_data.open("sig_data.txt");
    TH2F* sig_hist = new TH2F("sig_hist", "sig_hist", 50, 0, 1, 50, 0, 1);
    for (int i = 0; i < 1000000; ++i)
    {
        sig_pdf->GetRandom2(x, y);
        sig_data << x << " " << y << std::endl;
        xx = x;
        yy = y;
        mva = reader->EvaluateMVA("BDT");
        sig_hist->Fill(x, y);
        signal_tree->Fill();
    }
    sig_data.close();
    signal_tree->Write();
    TTree* background_tree = new TTree("background_tree", "background_tree");
    background_tree->Branch("x", &xx, "x/F"); 
    background_tree->Branch("y", &yy, "y/F"); 
    background_tree->Branch("mva", &mva, "mva/F"); 
    TF2* bkg_pdf = new TF2("bkg_pdf", "1/((x-1)*(y-1)+1)", 0, 1, 0, 1);
    bkg_pdf->Draw();
    ofstream bkg_data;
    bkg_data.open("bkg_data.txt");
    TH2F* bkg_hist = new TH2F("bkg_hist", "bkg_hist", 50, 0, 1, 50, 0, 1);
    for (int i = 0; i < 1000000; ++i)
    {
        bkg_pdf->GetRandom2(x, y);
        bkg_data << x << " " << y << std::endl;
        xx = x;
        yy = y;
        mva = reader->EvaluateMVA("BDT");
        bkg_hist->Fill(x, y);
        background_tree->Fill();
    }
    bkg_data.close();
    background_tree->Write();
    sig_hist->Write();
    bkg_hist->Write();
    ofile->Close();
}
