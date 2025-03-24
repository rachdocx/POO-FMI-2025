Sistem metrorex("Metrorex");
metrorex.loadMagistrale();
//Tren viena;
//ifstream f("trenuri.txt");
//f>>viena;
Depou a;
a.loadTrenuri();
a.afisTrenuri();
Tren b(9, 100,"Paris", "M1:", 5);
a.addTrenuri(b);
a.saveTrenuri();
cout<<"1. Pentru Gestiunea Magistralelor"<<endl;
cout<<"2. Pentru Gestiunea Trenurilor"<<endl;
cout<<"0. Pentru a iesi din aplicatie"<<endl;
int optiune;

cout<<"1. Adaugare Statie!"<<endl;
cout<<"2. Adaugare Magistrala!"<<endl;
cout<<"3. Afisare Sistem!"<<endl;
cin>>op;
int op1;
while (op!=0) {
    switch (op) {
        case 1:{
            cout<<"Introduceti numele magistralei unde introduceti statia"<<endl;
            cin>>nume_magistrala;
            cout<<"Introduceti numele statiei si ID-UL"<<endl;
            cin>>nume_statie>>id;
            cout<<"1 Pentru push_back, 2 Pentru push_front"<<endl;
            cin>>op1;
            Magistrala* m = metrorex.getMagistrala(nume_magistrala);
            if(op1==1){
                if (m) {
                    m->adaugareStatie(nume_statie, id);
                } else {
                    cout << "Magistrala nu a fost gasita!" << endl;
                }
            }
            }
            else{
                if (m) {
                    m->adaugareStatieBack(nume_statie, id);
                } else {
                    cout << "Magistrala nu a fost gasita!" << endl;
                }
            }
        }
        break;
        case 2: {
            cout << "Introduceti numele magistralei: ";
            cin >> nume_magistrala;
            Magistrala temp2(nume_magistrala);
            metrorex.adaugareMagistrala(temp2, 0);
            break;
        }
        case 3:
            metrorex.afisMagistrale();
        break;
    }
    cout<<"1. Adaugare Statie!"<<endl;
    cout<<"2. Adaugare Magistrala!"<<endl;
    cout<<"3. Afisare Sistem!"<<endl;
    cin>>op;
}

metrorex.saveMagistrale();