#include "run.hh"

MyRunAction::MyRunAction()
{
    G4RunManager::GetRunManager()->SetPrintProgress(0);

    G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();

    accumulableManager->RegisterAccumulable(totalEnergy);


    // Creare histograma si ntuple pentru spectrul razelor X

    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetNtupleMerging(true);

    analysisManager->CreateH1("Eabs", "Edep in logicUnion", 150, 0.001*keV, 150*keV);  //nbins, e_min, e_max
    
    analysisManager->CreateNtuple("Edep", "Edep in logicUnion");
    analysisManager->CreateNtupleDColumn("Eabs");
    analysisManager->FinishNtuple();
}

MyRunAction::~MyRunAction()
{
    if(fOutputFile.is_open()) fOutputFile.close();
}

void MyRunAction::BeginOfRunAction(const G4Run* run)
{
    G4int runID = run->GetRunID();
    G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();

    accumulableManager->Reset();

    G4RunManager::GetRunManager()->SetRandomNumberStore(false);

    fOutputFile.open(("outp_R" + std::to_string(runID) + ".txt").c_str());  //numele fisierului de output incarcat dupa fiecare event de succes


    auto analysisManager = G4AnalysisManager::Instance();

    G4String fileName = ("spectrum_Run" + std::to_string(runID) + ".root").c_str();
    analysisManager->OpenFile(fileName);  //deschiderea fisierului .root la inceput de Run
}

void MyRunAction::EndOfRunAction(const G4Run* run)
{
    G4int runID = run->GetRunID();  //ID-ul Run-ului curent

    G4int nOfEvents = run->GetNumberOfEvent();
    if(nOfEvents == 0) return;

    G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
    accumulableManager->Merge();

    if(fOutputFile.is_open()) fOutputFile.close();

    G4cout << G4endl << "*************** End of Run " << runID << " ***************" << G4endl << "Number of events: " << nOfEvents << G4endl << "Total energy deposited by X-rays: " << (totalEnergy.GetValue()) / keV << " [keV]" << G4endl << "********************************************" << G4endl << G4endl;



    // Umplerea fisierului .root la final de Run

    auto analysisManager = G4AnalysisManager::Instance();

    analysisManager->Write();
    analysisManager->CloseFile();
}

std::ofstream* MyRunAction::GetOutputFile()
{
    return &fOutputFile;  //fisierul de output incarcat dupa fiecare event de succes
}
