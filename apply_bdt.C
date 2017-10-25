#include "reweight_x.C"

void apply_bdt(int factor=10)
{
    int nevt = 1000;
    TMVA::Reader* reader = new TMVA::Reader("!Color:!Silent");
    float xx, yy, ww, mva, mva_rewgt;
    reader->AddVariable("x", &xx);
    reader->AddVariable("y", &yy);
    reader->BookMVA("BDT", "weights/TMVA_BDT.weights.xml");
    reader->BookMVA("BDT_use_rewgt", "weights/TMVA_BDT_use_rewgt.weights.xml");

    TFile* ofile = new TFile("bdt_output_data.root", "recreate");
    double x, y;
    TTree* signal_tree = new TTree("signal_tree", "signal_tree");
    signal_tree->Branch("x", &xx, "x/F"); 
    signal_tree->Branch("y", &yy, "y/F"); 
    signal_tree->Branch("w", &ww, "w/F"); 
    signal_tree->Branch("mva", &mva, "mva/F"); 
    signal_tree->Branch("mva_rewgt", &mva_rewgt, "mva_rewgt/F"); 
    TF2* sig_pdf = new TF2("sig_pdf", Form("1/(%d*x*y+1)", factor), 0, 1, 0, 1);
    TH2F* sig_hist = new TH2F("sig_hist", "sig_hist", 50, 0, 1, 50, 0, 1);
    for (int i = 0; i < nevt; ++i)
    {
        sig_pdf->GetRandom2(x, y);
        xx = x;
        yy = y;
        ww = rewgt(x);
        mva = reader->EvaluateMVA("BDT");
        mva_rewgt = reader->EvaluateMVA("BDT_use_rewgt");
        sig_hist->Fill(x, y);
        signal_tree->Fill();
    }
    signal_tree->Write();
    TTree* background_tree = new TTree("background_tree", "background_tree");
    background_tree->Branch("x", &xx, "x/F"); 
    background_tree->Branch("y", &yy, "y/F"); 
    background_tree->Branch("w", &ww, "w/F"); 
    background_tree->Branch("mva", &mva, "mva/F"); 
    background_tree->Branch("mva_rewgt", &mva_rewgt, "mva_rewgt/F"); 
    TF2* bkg_pdf = new TF2("bkg_pdf", Form("1/(%d*(x-1)*(y-1)+1)", factor), 0, 1, 0, 1);
    bkg_pdf->Draw();
    TH2F* bkg_hist = new TH2F("bkg_hist", "bkg_hist", 50, 0, 1, 50, 0, 1);
    for (int i = 0; i < nevt; ++i)
    {
        bkg_pdf->GetRandom2(x, y);
        xx = x;
        yy = y;
        ww = rewgt(x);
        mva = reader->EvaluateMVA("BDT");
        mva_rewgt = reader->EvaluateMVA("BDT_use_rewgt");
        bkg_hist->Fill(x, y);
        background_tree->Fill();
    }
    background_tree->Write();
    sig_hist->Write();
    bkg_hist->Write();
    ofile->Close();
}
