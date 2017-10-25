#include <cstdlib>
#include <iostream>
#include <map>
#include <string>

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"

//#include "TMVAGui.C"

//#if not defined(__CINT__) || defined(__MAKECINT__)
// needs to be included when makecint runs (ACLIC)
#include "TMVA/Factory.h"
#include "TMVA/Tools.h"
//#include "TMVA/Category.h"
//#endif

void train_bdt(bool use_rewgt=false)
{
    // Initialize TMVA
    TMVA::Tools::Instance();
//    (TMVA::gConfig().GetVariablePlotting()).fNbinsXOfROCCurve = 400;

    TFile* outputFile = 0;
    if (use_rewgt)
        outputFile = TFile::Open("BDT_use_rewgt.root", "RECREATE");
    else
        outputFile = TFile::Open("BDT.root", "RECREATE");

    TMVA::Factory *factory = new TMVA::Factory("TMVA", outputFile, "V:DrawProgressBar=True:Transformations=I;D;P;G:AnalysisType=Classification");

    factory->AddVariable("x", 'F');
    factory->AddVariable("y", 'F');
     
    // -----------------------------
    //  Input File & Tree
    // -----------------------------
    TFile* inputSignal = TFile::Open("bdt_input_data.root");
    TFile* inputBkg    = TFile::Open("bdt_input_data.root");
    TTree *signal     = (TTree*)inputSignal->Get("signal_tree");
    TTree *background = (TTree*)inputBkg->Get("background_tree");
    // global event weights per tree (see below for setting event-wise weights)
    Double_t signalWeight     = 1.0;
    Double_t backgroundWeight = 1.0;
    // You can add an arbitrary number of signal or background trees
    factory->AddSignalTree(signal);
    factory->AddBackgroundTree(background);
    // ---------------------------
    //  Training
    // ---------------------------
    // Apply additional cuts on the signal and background samples (can be different)
    TString prepare_nevents = "nTrain_Signal=100000:nTrain_Background=100000:nTest_Signal=100000:nTest_Background=100000:SplitMode=Alternate:NormMode=NumEvents:!V";
    factory->PrepareTrainingAndTestTree("", "", prepare_nevents);
    if (use_rewgt)
    {
        factory->SetSignalWeightExpression("1");
        factory->SetBackgroundWeightExpression("w");
    }

    TString option = "!H:V:NTrees=200:BoostType=Grad:Shrinkage=0.10:!UseBaggedGrad:nCuts=2000:MinNodeSize=0.1%:PruneStrength=5:PruneMethod=CostComplexity:MaxDepth=3:CreateMVAPdfs";
    if (use_rewgt)
        factory->BookMethod(TMVA::Types::kBDT, "BDT_use_rewgt", option);
    else
        factory->BookMethod(TMVA::Types::kBDT, "BDT", option);
    factory->TrainAllMethods();
    factory->TestAllMethods();
    factory->EvaluateAllMethods();
    // Save the output
    outputFile->Close();
    std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl;
    std::cout << "==> TMVAClassification is done!" << std::endl;
    delete factory;
}
