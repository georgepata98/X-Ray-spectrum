{
    TCanvas *c1 = new TCanvas("c1", "MyCanvas", 800, 600);
    TH1F *h1 = new TH1F("h1", "histogram", 150, 0, 150);

    fstream file;
    string fileName;
    float counts;


    cout << "---------- Program care face spectrul razelor X ----------" << endl << endl;

    cout << "Numele fisierului .txt cu valorile energiilor depozitate de razele X:" << endl;
    cin >> fileName;

    file.open(fileName + ".txt", ios::in);

    if(!file)
    {
        cout << "EROARE: Nu a fost gasit fisierul " << fileName << ".txt !" << endl;
        return 1;
    }

    while(!file.eof())
    {
        file >> counts;
        h1->Fill(counts);
    }

    file.close();


    h1->SetTitle("Geant4 X-rays spectrum for 150 kVp, 0.8 mm Be");
    h1->GetXaxis()->SetTitle("Energy [keV]");
    h1->GetYaxis()->SetTitle("Counts");
    h1->Draw();
}
