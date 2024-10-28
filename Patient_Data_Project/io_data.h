#ifndef IO_DATA_H
#define IO_DATA_H
#include <string>
using namespace std;
class io_data
{
public:
    int ID;
    string vorname;
    string nachname;
    string geburt;
    string geschlecht;
    string adresse;
    string tel_nummer;
    string mail;
    string datum;
    string diagnose;
    string behandlung;
    string zeile;
    io_data(int ID,string vorname,string nachname,string geburt,string geschlecht,string adresse,string tel_nummer,string mail,string datum,string diagnose,string behandlung);
    int returnAge();
    void printData();
};
#endif // IO_DATA_H
