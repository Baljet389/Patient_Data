#include "mainwindow.h"

#include <QApplication>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "io_data.h"

#include "timefunctions.h"

io_data::io_data(int ID,string vorname,string nachname,string geburt,string geschlecht,string adresse,string tel_nummer,string mail,string datum,string diagnose,string behandlung){
    this->ID=ID;
    this->vorname=vorname;
    this->nachname=nachname;
    this->geburt=geburt;
    this->geschlecht=geschlecht;
    this->adresse=adresse;
    this->tel_nummer=tel_nummer;
    this->mail=mail;
    this->datum=datum;
    this->diagnose=diagnose;
    this->behandlung=behandlung;
}
int io_data::returnAge(){
    geburt = "02.09.1984";
    gebdatum = zerlegeDatum(geburt);
    return berechneAlter(gebdatum[0], gebdatum[1], gebdatum[2]);
}

int io_data::returnAge() {
    std::string geburt = "02.09.1984";
    int tag, monat, jahr;

    timefunctions zeitfunktion;
    zeitfunktion.zerlegeDatum(geburt, tag, monat, jahr);

     return tf.berechneAlter(geburt);
}

void printData(){
    cout << "ID: " << ID << endl;
    cout << "Vorname: " << vorname << endl;
    cout << "Nachname: " << nachname << endl;
    cout << "Geburtstag: " << geburtstag << endl;
    cout << "Geschlecht: " << geschlecht << endl;
    cout << "Adresse: " << adresse << endl;
    cout << "Telefonnummer: " << tel_nummer << endl;
    cout << "Email: " << mail << endl;
    cout << "Datum: " << datum << endl;
    cout << "Diagnose: " << diagnose << endl;
    cout << "Behandlung: " << behandlung << endl;
}


    // while(getline(datei,zeile))
    // {
    //     stringstream zeilenpuffer(zeile);
    //     zeilenpuffer >> nummer >> vorname >> nachname >> geburt >> geschlecht >> adresse >> telnummer >> mail >> datum >> diagnose >> behandlung;
    //     cout << nummer << "\n" << vorname << "\n" << nachname << "\n" << geburt << "\n" << geschlecht << "\n" << adresse << "\n" << telnummer << "\n" << mail << "\n" << datum << "\n" << diagnose << "\n" << behandlung << "\n";
    // }
