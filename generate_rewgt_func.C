void generate_rewgt_func()
{
    TFile* f = new TFile("validate_data.root");
    TTree* signal_tree = (TTree*) f->Get("signal_tree");
    TTree* background_tree = (TTree*) f->Get("background_tree");
    signal_tree->Draw("x>>h_signal_x(100,0,1)","");
    TH1F* h_signal_x = (TH1F*) gDirectory->Get("h_signal_x");
    background_tree->Draw("x>>h_background_x(100,0,1)","");
    TH1F* h_background_x = (TH1F*) gDirectory->Get("h_background_x");
    TH1F* ratio = (TH1F*) h_signal_x->Clone("ratio");
    ratio->Divide(h_signal_x, h_background_x);
    ofstream funcfile;
    funcfile.open("reweight_x.C");
    funcfile << "float rewgt(float x)" << std::endl;
    funcfile << "{" << std::endl;
    for (int i = 1; i <= ratio->GetNbinsX(); ++i)
    {
        funcfile << "    if (x < " << ratio->GetBinLowEdge(i) + ratio->GetBinWidth(i) << ") ";
        funcfile << "return " << ratio->GetBinContent(i) << ";" << std::endl;
    }
    funcfile << "}" << std::endl;
    funcfile.close();
}
