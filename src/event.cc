#include "event.hh"

MyEventAction::MyEventAction(MyRunAction* runAction)
:fRunAction(runAction)
{
    G4RunManager::GetRunManager()->SetPrintProgress(0);

    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetVerboseLevel(0);

    fOutputFile = runAction->GetOutputFile();
}

MyEventAction::~MyEventAction()
{}

G4THitsMap<G4double>* MyEventAction::GetHitsCollection(G4int hcID, const G4Event *event) const
{
    auto hitsCollection = static_cast<G4THitsMap<G4double>*>(event->GetHCofThisEvent()->GetHC(hcID));

    if(!hitsCollection)
    {
        G4ExceptionDescription msg;
        msg << "Cannot access hitsCollection ID " << hcID;
        G4Exception("EventAction::GetHitsCollection()", "MyCode0003", FatalException, msg);
    }

    return hitsCollection;
}

G4double MyEventAction::GetSum(G4THitsMap<G4double>* hitsMap) const
{
    G4double sumValue = 0;

    for(auto it:*hitsMap->GetMap())
    {
        sumValue += *(it.second);
    }

    return sumValue;
}

void MyEventAction::BeginOfEventAction(const G4Event *event)
{}

void MyEventAction::EndOfEventAction(const G4Event *event)
{
    if(fAbsoEdepHCID == -1)
    {
        fAbsoEdepHCID = G4SDManager::GetSDMpointer()->GetCollectionID("energyDet/Edep");  //numele la MultiFunctionalDetector si PrimitiveScorer
    }

    G4double energy = GetSum(GetHitsCollection(fAbsoEdepHCID, event));

    auto analysisManager = G4AnalysisManager::Instance();

    if(energy > 0.)
    {
        fRunAction->AddEdep(energy);  //functia dependenta de variabila de tip G4double "energy" numita "AddEdep()"

        //*fOutputFile << "Event no." << event->GetEventID() << " -> Deposited energy of X-rays: " << energy / keV << " [keV]" << G4endl;
        *fOutputFile << energy / keV << G4endl;  //umplerea fisierului .txt pentru a putea fi analizat intr-un alt program de analiza de date

        analysisManager->FillH1(0, energy / keV);  //ID-ul histogramei, valoarea
        
        analysisManager->FillNtupleDColumn(0, energy / keV);
        analysisManager->AddNtupleRow();
    }
}
