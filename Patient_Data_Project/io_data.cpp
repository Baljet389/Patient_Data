#include "mainwindow.h"

#include <QApplication>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "io_data.h"



io_data::io_data(int ID,string vorname,string nachname,string geburt,string geschlecht,string adresse,string tel_nummer,string mail,string datum,string diagnose,string behandlung){
    this->ID=ID;
    this->Vorname=vorname;
    this->Nachname=nachname;
    this->Geburtstag=geburt;
    this->Geschlecht=geschlecht;
    this->Adresse=adresse;
    this->Telefonnummerstring=tel_nummer;
    this->Email=mail;
    this->Datum=datum;
    this->Diagnose=diagnose;
    this->Behandlung=behandlung
}
int io_data::returnAge(){
    return 0;
}


    // while(getline(datei,zeile))
    // {
    //     stringstream zeilenpuffer(zeile);
    //     zeilenpuffer >> nummer >> vorname >> nachname >> geburt >> geschlecht >> adresse >> telnummer >> mail >> datum >> diagnose >> behandlung;
    //     cout << nummer << "\n" << vorname << "\n" << nachname << "\n" << geburt << "\n" << geschlecht << "\n" << adresse << "\n" << telnummer << "\n" << mail << "\n" << datum << "\n" << diagnose << "\n" << behandlung << "\n";
    // }
