#include<iostream>
#include<algorithm>
#include<string>
#include<time.h>
#include<stdlib.h>
#include<vector>
#include<queue>
#include<stack>
#include<fstream>
#include<ctime>

using namespace std;

struct Club
{
    string name;
    int MP, W, D, L, GF, GA, GD, PTS, ID;
};

//vector donde se guardan los clubes que jugaran la liga
vector<Club> clubesLaLiga;

//variables globales
bool clubesDoneJ = false;
int Around = 1;

//ordenar clubes de LaLiga en la tabla de clasificacion
bool sortingout(Club a, Club b){
	if(a.PTS == b.PTS){
        if(a.GF == b.GF){
            return a.GA < b.GA;
        }
        else return a.GF > b.GF;
    }

    return a.PTS > b.PTS;
}

//prototipos de las funciones
void AddClubes();

int main(){

    Club aClub;
    bool follow = true;

    do{
        char option = 0;

        cout << "\n\033[36m--------------------LaLiga--------------------\033[0m\n" << endl;
		cout << "J-Jugar Jornada (" << Around << ")\t\t\tX-Tabla de Posiciones Liga\n";
		cout << "L-Emparejamientos de Liga\t\tA-Ingresar Jugadores\t\t\tS-Salir\n";
		cout << "Opcion: "; cin >> option; cin.ignore();

		switch(option){
			case 'J': //funcion jugar
            case 'j': break; 
            case 'X': //funcion de tablas de posiciones 
			case 'x': break; 
            case 'L': //emparejamiento de jornadas
            case 'l': break;
			case 'A': //ingresar jugadores
            case 'a': AddClubes(); break;
			case 'S': //salir
            case 's': follow = false; break;
            default : cout << "\nOpcion invalida!" << endl;
		}
    }while(follow);

    return 0;
}

//metodo para agregar los clubes
void AddClubes(){
    Club aClub;

    int k = 0;

    //ya hay liga
    if(clubesDoneJ){
        cout << "\nLaLiga ya ha iniciado, podra inscribir clubes al final de la temporada!" << endl;
    }
    //no hay liga
    else{
        cout << "\nINSCRIBA 20 CLUBES!\n\n" << endl;

        //se ingresan los 20 clubes
        do{
            cout << "Ingrese el nombre del club: "; getline(cin, aClub.name);

            if(aClub.name.length() > 15){
                        cout << "\n¡Maximo 15 caracteres!\n" << endl;
            }
            else{
                //la cadena del nombre se ajusta al tamanio requerido
                if(aClub.name.length() < 15){
                    do{
                        aClub.name = aClub.name + " ";
                    }while(aClub.name.length() < 15);
                }

                //se inicializan los valores de cada club
                aClub.MP = 0;
                aClub.W = 0;
                aClub.D = 0;
                aClub.L = 0;
                aClub.GF = 0;
                aClub.GA = 0;
                aClub.GD = 0;
                aClub.PTS = 0;
                aClub.ID = k + 1;

                clubesLaLiga.insert(clubesLaLiga.end(), aClub);

                k++;
            }        
        }while(k < 20);

        clubesDoneJ = true;//variable que dice que la liga ha iniciado
    }
}