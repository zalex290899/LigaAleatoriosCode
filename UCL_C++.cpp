#include<iostream> //necesario para el programa
#include<algorithm>
#include<string>//cadenas de textos
#include<time.h>//randoms
#include<stdlib.h>//random
#include<vector>//para usar los vectores
#include<queue>//para las colas
#include<stack>//para las pilas

using namespace std;

//informacion del jugador
struct infoClub{
    string name;
    int ID, points, machP, machW, machL, machE, golesF, golesC;
    int skill;
};

typedef infoClub iClub;

//variables globales
bool clubsSignedUp = false, clubsInGroups = false, matchMaked = false, phaseGroupsFinish = false;
bool pairingPlayOffMaked = false;
int numPhase = 1, removed = 16;

//vectores
vector<infoClub> vClubsGroupF, vClubsMessy, vClubs1Groups, vClubs2Groups, vClubsPlayOff;
vector<infoClub> vGroupA, vGroupB, vGroupC, vGroupD, vGroupE, vGroupF, vGroupG, vGroupH;

//pilas
stack<infoClub> pilaVS1GA, pilaVS2GA, pila1Groups, pila2Groups, pilaVSPlayOff1, pilaVSPlayOff2;
stack<infoClub> pilaGA, pilaGB, pilaGC, pilaGD, pilaGE, pilaGF, pilaGG, pilaGH;//emparejamientos
stack<infoClub> pilaVSGA, pilaVSGB, pilaVSGC, pilaVSGD, pilaVSGE, pilaVSGF, pilaVSGG, pilaVSGH;//resultados

//PROTOTIPOS DE FUNCIONES
//generales
void InscriptionsClubs();
//sistema de fase de grupos
void ViewGroups(), MessingUpClubs(), SortGroups(), MatchClubs(), SaveClubsInPilasPairing(), SaveClubsInPilasResults(), PlayPhase();
//sistema de eliminatorias
void Save1y2GroupsClubs(), MenuTittlePlayOff(), View8vos(), ViewPairingPlayOff(), PairingProcess(), PlayPlayOFF(), WinnerPlayOff();

//ordenando por puntos
bool sortingout(iClub a, iClub b){
    if(a.points == b.points){
        if(a.golesF == b.golesF){
            return a.golesC < b.golesC;
        }
        else return a.golesF > b.golesF;
    }

    return a.points > b.points;
}

int main(){
    srand(time(NULL));
    bool follow = true;
    do{
        char option = 0;
        cout << "\n----------UEFA CHAMPIONS LEAGUE----------\n" << endl;
        cout << "A-Inscribir clubes.\t\t\tB-Ver grupos." << endl;
        cout << "C-Emparejamientos Jornada " << numPhase << ".\t\tD-Jugar jornada." << endl;
        cout << "E-Emparejamientos Eliminatorias.\tF-Jugar Eliminatoria." << endl;
        cout << "G-Ver Ganador." << endl;

        cout << "Opcion: "; cin >> option; cin.ignore();
        cout << "\n";

        switch (option)
        {
            case 'A':
            case 'a': InscriptionsClubs(); break;
            case 'B':
            case 'b': ViewGroups(); break;
            case 'C':
            case 'c': 
            if(!phaseGroupsFinish) MatchClubs(); 
            else cout << "¡LA FASE DE GRUPOS A TERMINADO!" << endl;
            break;
            case 'D':
            case 'd': 
            if(!phaseGroupsFinish) PlayPhase(); 
            else cout << "¡LA FASE DE GRUPOS A TERMINADO!" << endl;
            break;
            case 'E':
            case 'e': 
            if(phaseGroupsFinish) ViewPairingPlayOff();
            else cout << "¡LA FASE DE GRUPOS NO HA TERMINADO!" << endl;
            break;
            case 'F':
            case 'f':
            if(phaseGroupsFinish) PlayPlayOFF();
            else cout << "¡LA FASE DE GRUPOS NO HA TERMINADO!" << endl;
            break;
            case 'G':
            case 'g':
            if(removed == 1) WinnerPlayOff();
            else cout << "¡LAS ELIMINATORIAS NO HAN TERMINADO!" << endl;
            break;
            default : cout << "Opcion invalida!\n" << endl;
        }
    }while(follow);

    return 0;
}

//se ingresan los equipos a competir
void InscriptionsClubs(){
    iClub club;
    int idClubs = 0;

    if(clubsSignedUp){
        cout << "¡YA HAY CLUBES INSCRITOS!" << endl;
    }
    else{
        do{
            cout << "Nombre del club: "; getline(cin, club.name);
            //cout << "Numero de skill del club (3 a 9): "; cin >> club.skill; cin.ignore();

            if(club.name.length() > 15){
                cout << "\n¡Maximo 15 caracteres!\n" << endl;
            }
            /*else if(club.skill < 3 || club.skill > 9){
                cout << "\n¡Rando de habilidad entre 3 y 9!\n" << endl;
            }*/
            else{

                //se le agregan espacios a los nombres para completar 15 caracteres
                if(club.name.length() < 15){
                    do{
                        club.name = club.name + " ";
                    }while(club.name.length() < 15);
                }

                club.ID = idClubs + 1;
                club.points = 0;
                club.machP = 0;
                club.machW = 0;
                club.machL = 0;
                club.machE = 0;
                club.golesF = 0;
                club.golesC = 0;
                club.skill = 9;
                idClubs++;

                vClubsGroupF.insert(vClubsGroupF.end(), club);
            }
            
        }while(idClubs < 32);

        clubsSignedUp = true;
    }    
}

//METODOS DEL SISTEMA DE JUEGO DE LA FASE DE GRUPOS
//se muestran los equipos en sus respectivos grupos
void ViewGroups(){

    if(!clubsInGroups && clubsSignedUp) {
        MessingUpClubs();
        clubsInGroups = true;
    }

    if(clubsSignedUp){
        SortGroups();

        //se muestran los equipos en los grupos
        cout << "\n\033[33m-----------------------------------GRUPO A----------------------------------\033[0m" << endl;
        for(int i = 0; i < vGroupA.size(); i++){

            if(i < 2) cout << vGroupA[i].name << " \033[34m|\033[0m MP: ";
            else if(i == 2) cout << vGroupA[i].name << " \033[33m|\033[0m MP: ";
            else cout << vGroupA[i].name << " | MP: ";

            cout << vGroupA[i].machP << " | MW: " << vGroupA[i].machW << " | ME: " << vGroupA[i].machE << " | ML: " << vGroupA[i].machL;   

            if(vGroupA[i].points > 9) cout << " | \033[33mPTS: " << vGroupA[i].points << "\033[0m | ";
            else cout << " | \033[33mPTS:  " << vGroupA[i].points << "\033[0m | ";
            if(vGroupA[i].golesF > 9) cout << "GF: " << vGroupA[i].golesF << " | ";
            else cout << "GF:  " << vGroupA[i].golesF << " | ";  
            if(vGroupA[i].golesC > 9) cout << "GC: " << vGroupA[i].golesC << endl;  
            else cout << "GC:  " << vGroupA[i].golesC << endl;  
        }

        cout << "\n\033[33m-----------------------------------GRUPO B----------------------------------\033[0m" << endl;
        for(int i = 0; i < vGroupB.size(); i++){

            if(i < 2) cout << vGroupB[i].name << " \033[34m|\033[0m MP: ";
            else if(i == 2) cout << vGroupB[i].name << " \033[33m|\033[0m MP: ";
            else cout << vGroupB[i].name << " | MP: ";

            cout << vGroupB[i].machP << " | MW: " << vGroupB[i].machW << " | ME: " << vGroupB[i].machE << " | ML: " << vGroupB[i].machL;

            if(vGroupB[i].points > 9) cout << " | \033[33mPTS: " << vGroupB[i].points << "\033[0m | ";
            else cout << " | \033[33mPTS:  " << vGroupB[i].points << "\033[0m | ";
            if(vGroupB[i].golesF > 9) cout << "GF: " << vGroupB[i].golesF << " | ";
            else cout << "GF:  " << vGroupB[i].golesF << " | ";  
            if(vGroupB[i].golesC > 9) cout << "GC: " << vGroupB[i].golesC << endl;  
            else cout << "GC:  " << vGroupB[i].golesC << endl; 
        }

        cout << "\n\033[33m-----------------------------------GRUPO C----------------------------------\033[0m" << endl;
        for(int i = 0; i < vGroupC.size(); i++){

            if(i < 2) cout << vGroupC[i].name << " \033[34m|\033[0m MP: ";
            else if(i == 2) cout << vGroupC[i].name << " \033[33m|\033[0m MP: ";
            else cout << vGroupC[i].name << " | MP: ";

            cout << vGroupC[i].machP << " | MW: " << vGroupC[i].machW << " | ME: " << vGroupC[i].machE << " | ML: " << vGroupC[i].machL;

            if(vGroupC[i].points > 9) cout << " | \033[33mPTS: " << vGroupC[i].points << "\033[0m | ";
            else cout << " | \033[33mPTS:  " << vGroupC[i].points << "\033[0m | ";
            if(vGroupC[i].golesF > 9) cout << "GF: " << vGroupC[i].golesF << " | ";
            else cout << "GF:  " << vGroupC[i].golesF << " | ";  
            if(vGroupC[i].golesC > 9) cout << "GC: " << vGroupC[i].golesC << endl;  
            else cout << "GC:  " << vGroupC[i].golesC << endl; 
        }

        cout << "\n\033[33m-----------------------------------GRUPO D----------------------------------\033[0m" << endl;
        for(int i = 0; i < vGroupD.size(); i++){

            if(i < 2) cout << vGroupD[i].name << " \033[34m|\033[0m MP: ";
            else if(i == 2) cout << vGroupD[i].name << " \033[33m|\033[0m MP: ";
            else cout << vGroupD[i].name << " | MP: ";

            cout << vGroupD[i].machP << " | MW: " << vGroupD[i].machW << " | ME: " << vGroupD[i].machE << " | ML: " << vGroupD[i].machL;

            if(vGroupD[i].points > 9) cout << " | \033[33mPTS: " << vGroupD[i].points << "\033[0m | ";
            else cout << " | \033[33mPTS:  " << vGroupD[i].points << "\033[0m | ";
            if(vGroupD[i].golesF > 9) cout << "GF: " << vGroupD[i].golesF << " | ";
            else cout << "GF:  " << vGroupD[i].golesF << " | ";  
            if(vGroupD[i].golesC > 9) cout << "GC: " << vGroupD[i].golesC << endl;  
            else cout << "GC:  " << vGroupD[i].golesC << endl;
        }

        cout << "\n\033[33m-----------------------------------GRUPO E----------------------------------\033[0m" << endl;
        for(int i = 0; i < vGroupE.size(); i++){
            
            if(i < 2) cout << vGroupE[i].name << " \033[34m|\033[0m MP: ";
            else if(i == 2) cout << vGroupE[i].name << " \033[33m|\033[0m MP: ";
            else cout << vGroupE[i].name << " | MP: ";

            cout << vGroupE[i].machP << " | MW: " << vGroupE[i].machW << " | ME: " << vGroupE[i].machE << " | ML: " << vGroupE[i].machL;

            if(vGroupE[i].points > 9) cout << " | \033[33mPTS: " << vGroupE[i].points << "\033[0m | ";
            else cout << " | \033[33mPTS:  " << vGroupE[i].points << "\033[0m | ";
            if(vGroupE[i].golesF > 9) cout << "GF: " << vGroupE[i].golesF << " | ";
            else cout << "GF:  " << vGroupE[i].golesF << " | ";  
            if(vGroupE[i].golesC > 9) cout << "GC: " << vGroupE[i].golesC << endl;  
            else cout << "GC:  " << vGroupE[i].golesC << endl;
        }

        cout << "\n\033[33m-----------------------------------GRUPO F----------------------------------\033[0m" << endl;
        for(int i = 0; i < vGroupF.size(); i++){

            if(i < 2) cout << vGroupF[i].name << " \033[34m|\033[0m MP: ";
            else if(i == 2) cout << vGroupF[i].name << " \033[33m|\033[0m MP: ";
            else cout << vGroupF[i].name << " | MP: ";

            cout << vGroupF[i].machP << " | MW: " << vGroupF[i].machW << " | ME: " << vGroupF[i].machE << " | ML: " << vGroupF[i].machL;

            if(vGroupF[i].points > 9) cout << " | \033[33mPTS: " << vGroupF[i].points << "\033[0m | ";
            else cout << " | \033[33mPTS:  " << vGroupF[i].points << "\033[0m | ";
            if(vGroupF[i].golesF > 9) cout << "GF: " << vGroupF[i].golesF << " | ";
            else cout << "GF:  " << vGroupF[i].golesF << " | ";  
            if(vGroupF[i].golesC > 9) cout << "GC: " << vGroupF[i].golesC << endl;  
            else cout << "GC:  " << vGroupF[i].golesC << endl; 
        }

        cout << "\n\033[33m-----------------------------------GRUPO G----------------------------------\033[0m" << endl;
        for(int i = 0; i < vGroupG.size(); i++){

            if(i < 2) cout << vGroupG[i].name << " \033[34m|\033[0m MP: ";
            else if(i == 2) cout << vGroupG[i].name << " \033[33m|\033[0m MP: ";
            else cout << vGroupG[i].name << " | MP: ";

            cout << vGroupG[i].machP << " | MW: " << vGroupG[i].machW << " | ME: " << vGroupG[i].machE << " | ML: " << vGroupG[i].machL;

            if(vGroupG[i].points > 9) cout << " | \033[33mPTS: " << vGroupG[i].points << "\033[0m | ";
            else cout << " | \033[33mPTS:  " << vGroupG[i].points << "\033[0m | ";
            if(vGroupG[i].golesF > 9) cout << "GF: " << vGroupG[i].golesF << " | ";
            else cout << "GF:  " << vGroupG[i].golesF << " | ";  
            if(vGroupG[i].golesC > 9) cout << "GC: " << vGroupG[i].golesC << endl;  
            else cout << "GC:  " << vGroupG[i].golesC << endl;
        }

        cout << "\n\033[33m-----------------------------------GRUPO H----------------------------------\033[0m" << endl;
        for(int i = 0; i < vGroupH.size(); i++){

            if(i < 2) cout << vGroupH[i].name << " \033[34m|\033[0m MP: ";
            else if(i == 2) cout << vGroupH[i].name << " \033[33m|\033[0m MP: ";
            else cout << vGroupH[i].name << " | MP: ";

            cout << vGroupH[i].machP << " | MW: " << vGroupH[i].machW << " | ME: " << vGroupH[i].machE << " | ML: " << vGroupH[i].machL;

            if(vGroupH[i].points > 9) cout << " | \033[33mPTS: " << vGroupH[i].points << "\033[0m | ";
            else cout << " | \033[33mPTS:  " << vGroupH[i].points << "\033[0m | ";
            if(vGroupH[i].golesF > 9) cout << "GF: " << vGroupH[i].golesF << " | ";
            else cout << "GF:  " << vGroupH[i].golesF << " | ";  
            if(vGroupH[i].golesC > 9) cout << "GC: " << vGroupH[i].golesC << endl;  
            else cout << "GC:  " << vGroupH[i].golesC << endl;
        }
    }
    else{
        cout << "¡AUN NO HAY CLUBES INSCRITOS!" << endl;
    }
}

//ordenar los grupos por puntos
void SortGroups(){
    sort(vGroupA.begin(), vGroupA.end(), sortingout);
    sort(vGroupB.begin(), vGroupB.end(), sortingout);
    sort(vGroupC.begin(), vGroupC.end(), sortingout);
    sort(vGroupD.begin(), vGroupD.end(), sortingout);
    sort(vGroupE.begin(), vGroupE.end(), sortingout);
    sort(vGroupF.begin(), vGroupF.end(), sortingout);
    sort(vGroupG.begin(), vGroupG.end(), sortingout);
    sort(vGroupH.begin(), vGroupH.end(), sortingout);
}

//se desordenan los clubes y se ponen en sus grupos
void MessingUpClubs(){    
    stack<iClub> pilaAuxClubs;
    iClub auxClub, auxClubErase;
    int numRandGroup = 0, numRand = 0;
    bool groupsFull = false;

    //guardando los clubes en el vector para desordenar
    for(int i = 0; i < vClubsGroupF.size(); i++){
        auxClub.name = vClubsGroupF[i].name;
        auxClub.ID = vClubsGroupF[i].ID;
        auxClub.points = vClubsGroupF[i].points;
        auxClub.machP = vClubsGroupF[i].machP;
        auxClub.machW = vClubsGroupF[i].machW;
        auxClub.machL = vClubsGroupF[i].machL;
        auxClub.machE = vClubsGroupF[i].machE;
        auxClub.golesF = vClubsGroupF[i].golesF;
        auxClub.golesC = vClubsGroupF[i].golesC;
        auxClub.skill = vClubsGroupF[i].skill;
        vClubsMessy.insert(vClubsMessy.end(), auxClub);
    }
    
    //se selecciona un club aleatorio para eliminarse y volver a guardarse (Desordenando)  
    for(int i = 0; i < 100; i++){

        numRand = 1 + rand()% 32;

        //se busca el club con id del random
        for(auto iter = vClubsMessy.begin(); iter != vClubsMessy.end(); ++iter){
            if(iter->ID == numRand){
                auxClubErase.name = iter->name;
                auxClubErase.ID = iter->ID;
                auxClubErase.points = iter->points;
                auxClubErase.machP = iter->machP;
                auxClubErase.machW = iter->machW;
                auxClubErase.machL = iter->machL;
                auxClubErase.machE = iter->machE;
                auxClubErase.golesF = iter->golesF;
                auxClubErase.golesC = iter->golesC;
                auxClubErase.skill = iter->skill;
                iter = vClubsMessy.erase(iter);
                break;
            }
        }

        //se vuelve a guardar el club en el vector de desorden
        vClubsMessy.insert(vClubsMessy.end(), auxClubErase);
    }

    //guardando los clubes en la pila aux
    for(int i = 0; i < vClubsMessy.size(); i++){
        auxClub.name = vClubsMessy[i].name;
        auxClub.ID = vClubsMessy[i].ID;
        auxClub.points = vClubsMessy[i].points;
        auxClub.machP = vClubsMessy[i].machP;
        auxClub.machW = vClubsMessy[i].machW;
        auxClub.machL = vClubsMessy[i].machL;
        auxClub.machE = vClubsMessy[i].machE;
        auxClub.golesF = vClubsMessy[i].golesF;
        auxClub.golesC = vClubsMessy[i].golesC;
        auxClub.skill = vClubsMessy[i].skill;
        pilaAuxClubs.push(auxClub);
    }

    //se ponen los clubes en cada grupo
    for(int i = 0; i < 4; i++){
        vGroupA.insert(vGroupA.end(), pilaAuxClubs.top());
        pilaAuxClubs.pop();
        vGroupB.insert(vGroupB.end(), pilaAuxClubs.top());
        pilaAuxClubs.pop();
        vGroupC.insert(vGroupC.end(), pilaAuxClubs.top());
        pilaAuxClubs.pop();
        vGroupD.insert(vGroupD.end(), pilaAuxClubs.top());
        pilaAuxClubs.pop();
        vGroupE.insert(vGroupE.end(), pilaAuxClubs.top());
        pilaAuxClubs.pop();
        vGroupF.insert(vGroupF.end(), pilaAuxClubs.top());
        pilaAuxClubs.pop();
        vGroupG.insert(vGroupG.end(), pilaAuxClubs.top());
        pilaAuxClubs.pop();
        vGroupH.insert(vGroupH.end(), pilaAuxClubs.top());
        pilaAuxClubs.pop();
    }

    //se ponen los clubes en las pilas de grupo para los emparejamietos
    SaveClubsInPilasPairing();
    SaveClubsInPilasResults();
}

//se emparejan cada club de cada grupo
void MatchClubs(){
    infoClub auxClub1, auxClub2;
    int k = 0;

    if(clubsInGroups){
        if(!matchMaked){
            cout << "JORNADA " << numPhase << endl;
    
            //EMPAREJAMIENTOS GRUPO A
            cout << "\nGrupo A\n" << endl;

            do{
                auxClub1 = pilaGA.top();
                pilaGA.pop();
                auxClub2 = pilaGA.top();
                pilaGA.pop();

                cout << auxClub1.name << "VS               " << auxClub2.name << endl;
                k++;
            }while(k < 2);

            k = 0;
            //EMPAREJAMIENTOS GRUPO B
            cout << "\nGrupo B\n" << endl;

            do{
                auxClub1 = pilaGB.top();
                pilaGB.pop();
                auxClub2 = pilaGB.top();
                pilaGB.pop();

                cout << auxClub1.name << "VS               " << auxClub2.name << endl;
                k++;
            }while(k < 2);

            k = 0;
            //EMPAREJAMIENTOS GRUPO C
            cout << "\nGrupo C\n" << endl;

            do{
                auxClub1 = pilaGC.top();
                pilaGC.pop();
                auxClub2 = pilaGC.top();
                pilaGC.pop();

                cout << auxClub1.name << "VS               " << auxClub2.name << endl;
                k++;
            }while(k < 2);

            k = 0;
            //EMPAREJAMIENTOS GRUPO D
            cout << "\nGrupo D\n" << endl;

            do{
                auxClub1 = pilaGD.top();
                pilaGD.pop();
                auxClub2 = pilaGD.top();
                pilaGD.pop();

                cout << auxClub1.name << "VS               " << auxClub2.name << endl;
                k++;
            }while(k < 2);

            k = 0;
            //EMPAREJAMIENTOS GRUPO E
            cout << "\nGrupo E\n" << endl;

            do{
                auxClub1 = pilaGE.top();
                pilaGE.pop();
                auxClub2 = pilaGE.top();
                pilaGE.pop();

                cout << auxClub1.name << "VS               " << auxClub2.name << endl;
                k++;
            }while(k < 2);

            k = 0;
            //EMPAREJAMIENTOS GRUPO F
            cout << "\nGrupo F\n" << endl;

            do{
                auxClub1 = pilaGF.top();
                pilaGF.pop();
                auxClub2 = pilaGF.top();
                pilaGF.pop();

                cout << auxClub1.name << "VS               " << auxClub2.name << endl;
                k++;
            }while(k < 2);

            k = 0;
            //EMPAREJAMIENTOS GRUPO G
            cout << "\nGrupo G\n" << endl;

            do{
                auxClub1 = pilaGG.top();
                pilaGG.pop();
                auxClub2 = pilaGG.top();
                pilaGG.pop();

                cout << auxClub1.name << "VS               " << auxClub2.name << endl;
                k++;
            }while(k < 2);

            k = 0;
            //EMPAREJAMIENTOS GRUPO H
            cout << "\nGrupo H\n" << endl;

            do{
                auxClub1 = pilaGH.top();
                pilaGH.pop();
                auxClub2 = pilaGH.top();
                pilaGH.pop();

                cout << auxClub1.name << "VS               " << auxClub2.name << endl;
                k++;
            }while(k < 2);

            matchMaked = true;
        }
        else{
            cout << "EMPAREJAMIENTOS HECHOS, JUEGUE UNA JORNADA ANTES." << endl;
        }
    }
    else{
        cout << "¡NO DISPONIBLE! LOS CLUBES NO ESTAN ORDENADOS EN SUS GRUPOS, VISITE LA OPCION B." << endl;
    }
}

//se juega una jornada, mostrando los resultados
void PlayPhase(){
    infoClub auxClub1, auxClub2;
    int k = 0, x = 0, y = 0;

    if(matchMaked){
        cout << "RESULTADOS DE LA JORNADA " << numPhase << endl;
    
        //RESULTADOS GRUPO A
        cout << "\nGrupo A\n" << endl;

        do{
            //se sacan los clubes de la pila resultados VS
            auxClub1 = pilaVSGA.top();
            pilaVSGA.pop();
            auxClub2 = pilaVSGA.top();
            pilaVSGA.pop();

            //se saca y se muestra el resultado
            x = 1 + rand()% auxClub1.skill;
            y = 1 + rand()% auxClub2.skill;

            cout << auxClub1.name << x <<"-"<< y << "               " << auxClub2.name << endl;

            //se le asigna los puntos correspondientes a cada club
            for(int i = 0; i < vGroupA.size(); i++){
                //JUGADOR 1
                if(vGroupA[i].ID == auxClub1.ID){
                    if(x > y) {
                        vGroupA[i].points += 3; vGroupA[i].machP += 1; vGroupA[i].machW += 1;
                    }
                    else if(x < y) {
                        vGroupA[i].machP += 1; vGroupA[i].machL += 1; 
                    }
                    else if(x == y) {
                        vGroupA[i].points += 1; vGroupA[i].machP += 1; vGroupA[i].machE += 1;
                    }

                    vGroupA[i].golesF += x;
                    vGroupA[i].golesC += y;
                }
                //JUGADOR 2
                if(vGroupA[i].ID == auxClub2.ID){
                    if(x < y) {
                        vGroupA[i].points += 3; vGroupA[i].machP += 1; vGroupA[i].machW += 1;
                    }
                    else if(x > y) {
                        vGroupA[i].machP += 1; vGroupA[i].machL += 1; 
                    }
                    else if(x == y) {
                        vGroupA[i].points += 1; vGroupA[i].machP += 1; vGroupA[i].machE += 1;
                    }

                    vGroupA[i].golesF += y;
                    vGroupA[i].golesC += x;
                }
            }
            k++;
        }while(k < 2);

        k = 0;
        //RESULTADOS GRUPO B
        cout << "\nGrupo B\n" << endl;

        do{
            //se sacan los clubes de la pila resultados VS
            auxClub1 = pilaVSGB.top();
            pilaVSGB.pop();
            auxClub2 = pilaVSGB.top();
            pilaVSGB.pop();

            //se saca y se muestra el resultado
            x = 1 + rand()% auxClub1.skill;
            y = 1 + rand()% auxClub2.skill;

            cout << auxClub1.name << x <<"-"<< y << "               " << auxClub2.name << endl;

            //se le asigna los puntos correspondientes a cada club
            for(int i = 0; i < vGroupB.size(); i++){
                //JUGADOR 1
                if(vGroupB[i].ID == auxClub1.ID){
                    if(x > y) {
                        vGroupB[i].points += 3; vGroupB[i].machP += 1; vGroupB[i].machW += 1;
                    }
                    else if(x < y) {
                        vGroupB[i].machP += 1; vGroupB[i].machL += 1; 
                    }
                    else if(x == y) {
                        vGroupB[i].points += 1; vGroupB[i].machP += 1; vGroupB[i].machE += 1;
                    }

                    vGroupB[i].golesF += x;
                    vGroupB[i].golesC += y;
                }
                //JUGADOR 2
                if(vGroupB[i].ID == auxClub2.ID){
                    if(x < y) {
                        vGroupB[i].points += 3; vGroupB[i].machP += 1; vGroupB[i].machW += 1;
                    }
                    else if(x > y) {
                        vGroupB[i].machP += 1; vGroupB[i].machL += 1; 
                    }
                    else if(x == y) {
                        vGroupB[i].points += 1; vGroupB[i].machP += 1; vGroupB[i].machE += 1;
                    }

                    vGroupB[i].golesF += y;
                    vGroupB[i].golesC += x;
                }
            }
            k++;
        }while(k < 2);

        k = 0;
        //RESULTADOS GRUPO C
        cout << "\nGrupo C\n" << endl;

        do{
            //se sacan los clubes de la pila resultados VS
            auxClub1 = pilaVSGC.top();
            pilaVSGC.pop();
            auxClub2 = pilaVSGC.top();
            pilaVSGC.pop();

            //se saca y se muestra el resultado
            x = 1 + rand()% auxClub1.skill;
            y = 1 + rand()% auxClub2.skill;

            cout << auxClub1.name << x <<"-"<< y << "               " << auxClub2.name << endl;

            //se le asigna los puntos correspondientes a cada club
            for(int i = 0; i < vGroupC.size(); i++){
                //JUGADOR 1
                if(vGroupC[i].ID == auxClub1.ID){
                    if(x > y) {
                        vGroupC[i].points += 3; vGroupC[i].machP += 1; vGroupC[i].machW += 1;
                    }
                    else if(x < y) {
                        vGroupC[i].machP += 1; vGroupC[i].machL += 1; 
                    }
                    else if(x == y) {
                        vGroupC[i].points += 1; vGroupC[i].machP += 1; vGroupC[i].machE += 1;
                    }

                    vGroupC[i].golesF += x;
                    vGroupC[i].golesC += y;
                }
                //JUGADOR 2
                if(vGroupC[i].ID == auxClub2.ID){
                    if(x < y) {
                        vGroupC[i].points += 3; vGroupC[i].machP += 1; vGroupC[i].machW += 1;
                    }
                    else if(x > y) {
                        vGroupC[i].machP += 1; vGroupC[i].machL += 1; 
                    }
                    else if(x == y) {
                        vGroupC[i].points += 1; vGroupC[i].machP += 1; vGroupC[i].machE += 1;
                    }

                    vGroupC[i].golesF += y;
                    vGroupC[i].golesC += x;
                }
            }
            k++;
        }while(k < 2);

        k = 0;
        //RESULTADOS GRUPO D
        cout << "\nGrupo D\n" << endl;

        do{
            //se sacan los clubes de la pila resultados VS
            auxClub1 = pilaVSGD.top();
            pilaVSGD.pop();
            auxClub2 = pilaVSGD.top();
            pilaVSGD.pop();

            //se saca y se muestra el resultado
            x = 1 + rand()% auxClub1.skill;
            y = 1 + rand()% auxClub2.skill;

            cout << auxClub1.name << x <<"-"<< y << "               " << auxClub2.name << endl;

            //se le asigna los puntos correspondientes a cada club
            for(int i = 0; i < vGroupD.size(); i++){
                //JUGADOR 1
                if(vGroupD[i].ID == auxClub1.ID){
                    if(x > y) {
                        vGroupD[i].points += 3; vGroupD[i].machP += 1; vGroupD[i].machW += 1;
                    }
                    else if(x < y) {
                        vGroupD[i].machP += 1; vGroupD[i].machL += 1; 
                    }
                    else if(x == y) {
                        vGroupD[i].points += 1; vGroupD[i].machP += 1; vGroupD[i].machE += 1;
                    }

                    vGroupD[i].golesF += x;
                    vGroupD[i].golesC += y;
                }
                //JUGADOR 2
                if(vGroupD[i].ID == auxClub2.ID){
                    if(x < y) {
                        vGroupD[i].points += 3; vGroupD[i].machP += 1; vGroupD[i].machW += 1;
                    }
                    else if(x > y) {
                        vGroupD[i].machP += 1; vGroupD[i].machL += 1; 
                    }
                    else if(x == y) {
                        vGroupD[i].points += 1; vGroupD[i].machP += 1; vGroupD[i].machE += 1;
                    }

                    vGroupD[i].golesF += y;
                    vGroupD[i].golesC += x;
                }
            }
            k++;
        }while(k < 2);

        k = 0;
        //RESULTADOS GRUPO E
        cout << "\nGrupo E\n" << endl;

        do{
            //se sacan los clubes de la pila resultados VS
            auxClub1 = pilaVSGE.top();
            pilaVSGE.pop();
            auxClub2 = pilaVSGE.top();
            pilaVSGE.pop();

            //se saca y se muestra el resultado
            x = 1 + rand()% auxClub1.skill;
            y = 1 + rand()% auxClub2.skill;

            cout << auxClub1.name << x <<"-"<< y << "               " << auxClub2.name << endl;

            //se le asigna los puntos correspondientes a cada club
            for(int i = 0; i < vGroupE.size(); i++){
                //JUGADOR 1
                if(vGroupE[i].ID == auxClub1.ID){
                    if(x > y) {
                        vGroupE[i].points += 3; vGroupE[i].machP += 1; vGroupE[i].machW += 1;
                    }
                    else if(x < y) {
                        vGroupE[i].machP += 1; vGroupE[i].machL += 1; 
                    }
                    else if(x == y) {
                        vGroupE[i].points += 1; vGroupE[i].machP += 1; vGroupE[i].machE += 1;
                    }

                    vGroupE[i].golesF += x;
                    vGroupE[i].golesC += y;
                }
                //JUGADOR 2
                if(vGroupE[i].ID == auxClub2.ID){
                    if(x < y) {
                        vGroupE[i].points += 3; vGroupE[i].machP += 1; vGroupE[i].machW += 1;
                    }
                    else if(x > y) {
                        vGroupE[i].machP += 1; vGroupE[i].machL += 1; 
                    }
                    else if(x == y) {
                        vGroupE[i].points += 1; vGroupE[i].machP += 1; vGroupE[i].machE += 1;
                    }

                    vGroupE[i].golesF += y;
                    vGroupE[i].golesC += x;
                }
            }
            k++;
        }while(k < 2);

        k = 0;
        //RESULTADOS GRUPO F
        cout << "\nGrupo F\n" << endl;

        do{
            //se sacan los clubes de la pila resultados VS
            auxClub1 = pilaVSGF.top();
            pilaVSGF.pop();
            auxClub2 = pilaVSGF.top();
            pilaVSGF.pop();

            //se saca y se muestra el resultado
            x = 1 + rand()% auxClub1.skill;
            y = 1 + rand()% auxClub2.skill;

            cout << auxClub1.name << x <<"-"<< y << "               " << auxClub2.name << endl;

            //se le asigna los puntos correspondientes a cada club
            for(int i = 0; i < vGroupF.size(); i++){
                //JUGADOR 1
                if(vGroupF[i].ID == auxClub1.ID){
                    if(x > y) {
                        vGroupF[i].points += 3; vGroupF[i].machP += 1; vGroupF[i].machW += 1;
                    }
                    else if(x < y) {
                        vGroupF[i].machP += 1; vGroupF[i].machL += 1; 
                    }
                    else if(x == y) {
                        vGroupF[i].points += 1; vGroupF[i].machP += 1; vGroupF[i].machE += 1;
                    }

                    vGroupF[i].golesF += x;
                    vGroupF[i].golesC += y;
                }
                //JUGADOR 2
                if(vGroupF[i].ID == auxClub2.ID){
                    if(x < y) {
                        vGroupF[i].points += 3; vGroupF[i].machP += 1; vGroupF[i].machW += 1;
                    }
                    else if(x > y) {
                        vGroupF[i].machP += 1; vGroupF[i].machL += 1; 
                    }
                    else if(x == y) {
                        vGroupF[i].points += 1; vGroupF[i].machP += 1; vGroupF[i].machE += 1;
                    }

                    vGroupF[i].golesF += y;
                    vGroupF[i].golesC += x;
                }
            }
            k++;
        }while(k < 2);

        k = 0;
        //RESULTADOS GRUPO G
        cout << "\nGrupo G\n" << endl;

        do{
            //se sacan los clubes de la pila resultados VS
            auxClub1 = pilaVSGG.top();
            pilaVSGG.pop();
            auxClub2 = pilaVSGG.top();
            pilaVSGG.pop();

            //se saca y se muestra el resultado
            x = 1 + rand()% auxClub1.skill;
            y = 1 + rand()% auxClub2.skill;

            cout << auxClub1.name << x <<"-"<< y << "               " << auxClub2.name << endl;

            //se le asigna los puntos correspondientes a cada club
            for(int i = 0; i < vGroupG.size(); i++){
                //JUGADOR 1
                if(vGroupG[i].ID == auxClub1.ID){
                    if(x > y) {
                        vGroupG[i].points += 3; vGroupG[i].machP += 1; vGroupG[i].machW += 1;
                    }
                    else if(x < y) {
                        vGroupG[i].machP += 1; vGroupG[i].machL += 1; 
                    }
                    else if(x == y) {
                        vGroupG[i].points += 1; vGroupG[i].machP += 1; vGroupG[i].machE += 1;
                    }

                    vGroupG[i].golesF += x;
                    vGroupG[i].golesC += y;
                }
                //JUGADOR 2
                if(vGroupG[i].ID == auxClub2.ID){
                    if(x < y) {
                        vGroupG[i].points += 3; vGroupG[i].machP += 1; vGroupG[i].machW += 1;
                    }
                    else if(x > y) {
                        vGroupG[i].machP += 1; vGroupG[i].machL += 1; 
                    }
                    else if(x == y) {
                        vGroupG[i].points += 1; vGroupG[i].machP += 1; vGroupG[i].machE += 1;
                    }

                    vGroupG[i].golesF += y;
                    vGroupG[i].golesC += x;
                }
            }
            k++;
        }while(k < 2);

        k = 0;
        //RESULTADOS GRUPO H
        cout << "\nGrupo H\n" << endl;

        do{
            //se sacan los clubes de la pila resultados VS
            auxClub1 = pilaVSGH.top();
            pilaVSGH.pop();
            auxClub2 = pilaVSGH.top();
            pilaVSGH.pop();

            //se saca y se muestra el resultado
            x = 1 + rand()% auxClub1.skill;
            y = 1 + rand()% auxClub2.skill;

            cout << auxClub1.name << x <<"-"<< y << "               " << auxClub2.name << endl;

            //se le asigna los puntos correspondientes a cada club
            for(int i = 0; i < vGroupH.size(); i++){
                //JUGADOR 1
                if(vGroupH[i].ID == auxClub1.ID){
                    if(x > y) {
                        vGroupH[i].points += 3; vGroupH[i].machP += 1; vGroupH[i].machW += 1;
                    }
                    else if(x < y) {
                        vGroupH[i].machP += 1; vGroupH[i].machL += 1; 
                    }
                    else if(x == y) {
                        vGroupH[i].points += 1; vGroupH[i].machP += 1; vGroupH[i].machE += 1;
                    }

                    vGroupH[i].golesF += x;
                    vGroupH[i].golesC += y;
                }
                //JUGADOR 2
                if(vGroupH[i].ID == auxClub2.ID){
                    if(x < y) {
                        vGroupH[i].points += 3; vGroupH[i].machP += 1; vGroupH[i].machW += 1;
                    }
                    else if(x > y) {
                        vGroupH[i].machP += 1; vGroupH[i].machL += 1; 
                    }
                    else if(x == y) {
                        vGroupH[i].points += 1; vGroupH[i].machP += 1; vGroupH[i].machE += 1;
                    }

                    vGroupH[i].golesF += y;
                    vGroupH[i].golesC += x;
                }
            }
            k++;
        }while(k < 2);

        if(numPhase < 6) numPhase++;
        else {
            phaseGroupsFinish = true;
            Save1y2GroupsClubs();
        }
        matchMaked = false;
    }
    else{
        cout << "¡VISITE LOS EMPAREJAMIENTOS ANTES DE JUGAR LA JORNADA!" << endl;
    }
}

//se guardan los clubes de cada grupo en sus pilas de emparejamientos
void SaveClubsInPilasPairing(){
    //Grupo A en pilaGA emparejamientos
    pilaGA.push(vGroupA[1]); pilaGA.push(vGroupA[2]); pilaGA.push(vGroupA[0]); pilaGA.push(vGroupA[3]);//J6 3021
    pilaGA.push(vGroupA[1]); pilaGA.push(vGroupA[3]); pilaGA.push(vGroupA[0]); pilaGA.push(vGroupA[2]);//J5 2031
    pilaGA.push(vGroupA[2]); pilaGA.push(vGroupA[3]); pilaGA.push(vGroupA[0]); pilaGA.push(vGroupA[1]);//J4 1032
    pilaGA.push(vGroupA[2]); pilaGA.push(vGroupA[1]); pilaGA.push(vGroupA[3]); pilaGA.push(vGroupA[0]);//J3 0312
    pilaGA.push(vGroupA[3]); pilaGA.push(vGroupA[1]); pilaGA.push(vGroupA[2]); pilaGA.push(vGroupA[0]);//J2 0213
    pilaGA.push(vGroupA[3]); pilaGA.push(vGroupA[2]); pilaGA.push(vGroupA[1]); pilaGA.push(vGroupA[0]);//J1 0123
    //Grupo B en pilaGB emparejamientos
    pilaGB.push(vGroupB[1]); pilaGB.push(vGroupB[2]); pilaGB.push(vGroupB[0]); pilaGB.push(vGroupB[3]);//J6 3021
    pilaGB.push(vGroupB[1]); pilaGB.push(vGroupB[3]); pilaGB.push(vGroupB[0]); pilaGB.push(vGroupB[2]);//J5 2031
    pilaGB.push(vGroupB[2]); pilaGB.push(vGroupB[3]); pilaGB.push(vGroupB[0]); pilaGB.push(vGroupB[1]);//J4 1032
    pilaGB.push(vGroupB[2]); pilaGB.push(vGroupB[1]); pilaGB.push(vGroupB[3]); pilaGB.push(vGroupB[0]);//J3 0312
    pilaGB.push(vGroupB[3]); pilaGB.push(vGroupB[1]); pilaGB.push(vGroupB[2]); pilaGB.push(vGroupB[0]);//J2 0213
    pilaGB.push(vGroupB[3]); pilaGB.push(vGroupB[2]); pilaGB.push(vGroupB[1]); pilaGB.push(vGroupB[0]);//J1 0123
    //Grupo C en pilaGC emparejamientos
    pilaGC.push(vGroupC[1]); pilaGC.push(vGroupC[2]); pilaGC.push(vGroupC[0]); pilaGC.push(vGroupC[3]);//J6 3021
    pilaGC.push(vGroupC[1]); pilaGC.push(vGroupC[3]); pilaGC.push(vGroupC[0]); pilaGC.push(vGroupC[2]);//J5 2031
    pilaGC.push(vGroupC[2]); pilaGC.push(vGroupC[3]); pilaGC.push(vGroupC[0]); pilaGC.push(vGroupC[1]);//J4 1032
    pilaGC.push(vGroupC[2]); pilaGC.push(vGroupC[1]); pilaGC.push(vGroupC[3]); pilaGC.push(vGroupC[0]);//J3 0312
    pilaGC.push(vGroupC[3]); pilaGC.push(vGroupC[1]); pilaGC.push(vGroupC[2]); pilaGC.push(vGroupC[0]);//J2 0213
    pilaGC.push(vGroupC[3]); pilaGC.push(vGroupC[2]); pilaGC.push(vGroupC[1]); pilaGC.push(vGroupC[0]);//J1 0123
    //Grupo D en pilaGD emparejamientos
    pilaGD.push(vGroupD[1]); pilaGD.push(vGroupD[2]); pilaGD.push(vGroupD[0]); pilaGD.push(vGroupD[3]);//J6 3021
    pilaGD.push(vGroupD[1]); pilaGD.push(vGroupD[3]); pilaGD.push(vGroupD[0]); pilaGD.push(vGroupD[2]);//J5 2031
    pilaGD.push(vGroupD[2]); pilaGD.push(vGroupD[3]); pilaGD.push(vGroupD[0]); pilaGD.push(vGroupD[1]);//J4 1032
    pilaGD.push(vGroupD[2]); pilaGD.push(vGroupD[1]); pilaGD.push(vGroupD[3]); pilaGD.push(vGroupD[0]);//J3 0312
    pilaGD.push(vGroupD[3]); pilaGD.push(vGroupD[1]); pilaGD.push(vGroupD[2]); pilaGD.push(vGroupD[0]);//J2 0213
    pilaGD.push(vGroupD[3]); pilaGD.push(vGroupD[2]); pilaGD.push(vGroupD[1]); pilaGD.push(vGroupD[0]);//J1 0123
    //Grupo E en pilaGE emparejamientos
    pilaGE.push(vGroupE[1]); pilaGE.push(vGroupE[2]); pilaGE.push(vGroupE[0]); pilaGE.push(vGroupE[3]);//J6 3021
    pilaGE.push(vGroupE[1]); pilaGE.push(vGroupE[3]); pilaGE.push(vGroupE[0]); pilaGE.push(vGroupE[2]);//J5 2031
    pilaGE.push(vGroupE[2]); pilaGE.push(vGroupE[3]); pilaGE.push(vGroupE[0]); pilaGE.push(vGroupE[1]);//J4 1032
    pilaGE.push(vGroupE[2]); pilaGE.push(vGroupE[1]); pilaGE.push(vGroupE[3]); pilaGE.push(vGroupE[0]);//J3 0312
    pilaGE.push(vGroupE[3]); pilaGE.push(vGroupE[1]); pilaGE.push(vGroupE[2]); pilaGE.push(vGroupE[0]);//J2 0213
    pilaGE.push(vGroupE[3]); pilaGE.push(vGroupE[2]); pilaGE.push(vGroupE[1]); pilaGE.push(vGroupE[0]);//J1 0123
    //Grupo F en pilaGF emparejamientos
    pilaGF.push(vGroupF[1]); pilaGF.push(vGroupF[2]); pilaGF.push(vGroupF[0]); pilaGF.push(vGroupF[3]);//J6 3021
    pilaGF.push(vGroupF[1]); pilaGF.push(vGroupF[3]); pilaGF.push(vGroupF[0]); pilaGF.push(vGroupF[2]);//J5 2031
    pilaGF.push(vGroupF[2]); pilaGF.push(vGroupF[3]); pilaGF.push(vGroupF[0]); pilaGF.push(vGroupF[1]);//J4 1032
    pilaGF.push(vGroupF[2]); pilaGF.push(vGroupF[1]); pilaGF.push(vGroupF[3]); pilaGF.push(vGroupF[0]);//J3 0312
    pilaGF.push(vGroupF[3]); pilaGF.push(vGroupF[1]); pilaGF.push(vGroupF[2]); pilaGF.push(vGroupF[0]);//J2 0213
    pilaGF.push(vGroupF[3]); pilaGF.push(vGroupF[2]); pilaGF.push(vGroupF[1]); pilaGF.push(vGroupF[0]);//J1 0123
    //Grupo G en pilaGG emparejamientos
    pilaGG.push(vGroupG[1]); pilaGG.push(vGroupG[2]); pilaGG.push(vGroupG[0]); pilaGG.push(vGroupG[3]);//J6 3021
    pilaGG.push(vGroupG[1]); pilaGG.push(vGroupG[3]); pilaGG.push(vGroupG[0]); pilaGG.push(vGroupG[2]);//J5 2031
    pilaGG.push(vGroupG[2]); pilaGG.push(vGroupG[3]); pilaGG.push(vGroupG[0]); pilaGG.push(vGroupG[1]);//J4 1032
    pilaGG.push(vGroupG[2]); pilaGG.push(vGroupG[1]); pilaGG.push(vGroupG[3]); pilaGG.push(vGroupG[0]);//J3 0312
    pilaGG.push(vGroupG[3]); pilaGG.push(vGroupG[1]); pilaGG.push(vGroupG[2]); pilaGG.push(vGroupG[0]);//J2 0213
    pilaGG.push(vGroupG[3]); pilaGG.push(vGroupG[2]); pilaGG.push(vGroupG[1]); pilaGG.push(vGroupG[0]);//J1 0123
    //Grupo H en pilaGH emparejamientos
    pilaGH.push(vGroupH[1]); pilaGH.push(vGroupH[2]); pilaGH.push(vGroupH[0]); pilaGH.push(vGroupH[3]);//J6 3021
    pilaGH.push(vGroupH[1]); pilaGH.push(vGroupH[3]); pilaGH.push(vGroupH[0]); pilaGH.push(vGroupH[2]);//J5 2031
    pilaGH.push(vGroupH[2]); pilaGH.push(vGroupH[3]); pilaGH.push(vGroupH[0]); pilaGH.push(vGroupH[1]);//J4 1032
    pilaGH.push(vGroupH[2]); pilaGH.push(vGroupH[1]); pilaGH.push(vGroupH[3]); pilaGH.push(vGroupH[0]);//J3 0312
    pilaGH.push(vGroupH[3]); pilaGH.push(vGroupH[1]); pilaGH.push(vGroupH[2]); pilaGH.push(vGroupH[0]);//J2 0213
    pilaGH.push(vGroupH[3]); pilaGH.push(vGroupH[2]); pilaGH.push(vGroupH[1]); pilaGH.push(vGroupH[0]);//J1 0123
}

//se guardan los clubes de cada grupo en sus pilas de resultados
void SaveClubsInPilasResults(){
    //Grupo A en pilaGA resultados
    pilaVSGA.push(vGroupA[1]); pilaVSGA.push(vGroupA[2]); pilaVSGA.push(vGroupA[0]); pilaVSGA.push(vGroupA[3]);//J6 3021
    pilaVSGA.push(vGroupA[1]); pilaVSGA.push(vGroupA[3]); pilaVSGA.push(vGroupA[0]); pilaVSGA.push(vGroupA[2]);//J5 2031
    pilaVSGA.push(vGroupA[2]); pilaVSGA.push(vGroupA[3]); pilaVSGA.push(vGroupA[0]); pilaVSGA.push(vGroupA[1]);//J4 1032
    pilaVSGA.push(vGroupA[2]); pilaVSGA.push(vGroupA[1]); pilaVSGA.push(vGroupA[3]); pilaVSGA.push(vGroupA[0]);//J3 0312
    pilaVSGA.push(vGroupA[3]); pilaVSGA.push(vGroupA[1]); pilaVSGA.push(vGroupA[2]); pilaVSGA.push(vGroupA[0]);//J2 0213
    pilaVSGA.push(vGroupA[3]); pilaVSGA.push(vGroupA[2]); pilaVSGA.push(vGroupA[1]); pilaVSGA.push(vGroupA[0]);//J1 0123
    //Grupo B en pilaGB  resultados
    pilaVSGB.push(vGroupB[1]); pilaVSGB.push(vGroupB[2]); pilaVSGB.push(vGroupB[0]); pilaVSGB.push(vGroupB[3]);//J6 3021
    pilaVSGB.push(vGroupB[1]); pilaVSGB.push(vGroupB[3]); pilaVSGB.push(vGroupB[0]); pilaVSGB.push(vGroupB[2]);//J5 2031
    pilaVSGB.push(vGroupB[2]); pilaVSGB.push(vGroupB[3]); pilaVSGB.push(vGroupB[0]); pilaVSGB.push(vGroupB[1]);//J4 1032
    pilaVSGB.push(vGroupB[2]); pilaVSGB.push(vGroupB[1]); pilaVSGB.push(vGroupB[3]); pilaVSGB.push(vGroupB[0]);//J3 0312
    pilaVSGB.push(vGroupB[3]); pilaVSGB.push(vGroupB[1]); pilaVSGB.push(vGroupB[2]); pilaVSGB.push(vGroupB[0]);//J2 0213
    pilaVSGB.push(vGroupB[3]); pilaVSGB.push(vGroupB[2]); pilaVSGB.push(vGroupB[1]); pilaVSGB.push(vGroupB[0]);//J1 0123
    //Grupo C en pilaGC  resultados
    pilaVSGC.push(vGroupC[1]); pilaVSGC.push(vGroupC[2]); pilaVSGC.push(vGroupC[0]); pilaVSGC.push(vGroupC[3]);//J6 3021
    pilaVSGC.push(vGroupC[1]); pilaVSGC.push(vGroupC[3]); pilaVSGC.push(vGroupC[0]); pilaVSGC.push(vGroupC[2]);//J5 2031
    pilaVSGC.push(vGroupC[2]); pilaVSGC.push(vGroupC[3]); pilaVSGC.push(vGroupC[0]); pilaVSGC.push(vGroupC[1]);//J4 1032
    pilaVSGC.push(vGroupC[2]); pilaVSGC.push(vGroupC[1]); pilaVSGC.push(vGroupC[3]); pilaVSGC.push(vGroupC[0]);//J3 0312
    pilaVSGC.push(vGroupC[3]); pilaVSGC.push(vGroupC[1]); pilaVSGC.push(vGroupC[2]); pilaVSGC.push(vGroupC[0]);//J2 0213
    pilaVSGC.push(vGroupC[3]); pilaVSGC.push(vGroupC[2]); pilaVSGC.push(vGroupC[1]); pilaVSGC.push(vGroupC[0]);//J1 0123
    //Grupo D en pilaGD  resultados
    pilaVSGD.push(vGroupD[1]); pilaVSGD.push(vGroupD[2]); pilaVSGD.push(vGroupD[0]); pilaVSGD.push(vGroupD[3]);//J6 3021
    pilaVSGD.push(vGroupD[1]); pilaVSGD.push(vGroupD[3]); pilaVSGD.push(vGroupD[0]); pilaVSGD.push(vGroupD[2]);//J5 2031
    pilaVSGD.push(vGroupD[2]); pilaVSGD.push(vGroupD[3]); pilaVSGD.push(vGroupD[0]); pilaVSGD.push(vGroupD[1]);//J4 1032
    pilaVSGD.push(vGroupD[2]); pilaVSGD.push(vGroupD[1]); pilaVSGD.push(vGroupD[3]); pilaVSGD.push(vGroupD[0]);//J3 0312
    pilaVSGD.push(vGroupD[3]); pilaVSGD.push(vGroupD[1]); pilaVSGD.push(vGroupD[2]); pilaVSGD.push(vGroupD[0]);//J2 0213
    pilaVSGD.push(vGroupD[3]); pilaVSGD.push(vGroupD[2]); pilaVSGD.push(vGroupD[1]); pilaVSGD.push(vGroupD[0]);//J1 0123
    //Grupo E en pilaGE  resultados
    pilaVSGE.push(vGroupE[1]); pilaVSGE.push(vGroupE[2]); pilaVSGE.push(vGroupE[0]); pilaVSGE.push(vGroupE[3]);//J6 3021
    pilaVSGE.push(vGroupE[1]); pilaVSGE.push(vGroupE[3]); pilaVSGE.push(vGroupE[0]); pilaVSGE.push(vGroupE[2]);//J5 2031
    pilaVSGE.push(vGroupE[2]); pilaVSGE.push(vGroupE[3]); pilaVSGE.push(vGroupE[0]); pilaVSGE.push(vGroupE[1]);//J4 1032
    pilaVSGE.push(vGroupE[2]); pilaVSGE.push(vGroupE[1]); pilaVSGE.push(vGroupE[3]); pilaVSGE.push(vGroupE[0]);//J3 0312
    pilaVSGE.push(vGroupE[3]); pilaVSGE.push(vGroupE[1]); pilaVSGE.push(vGroupE[2]); pilaVSGE.push(vGroupE[0]);//J2 0213
    pilaVSGE.push(vGroupE[3]); pilaVSGE.push(vGroupE[2]); pilaVSGE.push(vGroupE[1]); pilaVSGE.push(vGroupE[0]);//J1 0123
    //Grupo F en pilaGF  resultados
    pilaVSGF.push(vGroupF[1]); pilaVSGF.push(vGroupF[2]); pilaVSGF.push(vGroupF[0]); pilaVSGF.push(vGroupF[3]);//J6 3021
    pilaVSGF.push(vGroupF[1]); pilaVSGF.push(vGroupF[3]); pilaVSGF.push(vGroupF[0]); pilaVSGF.push(vGroupF[2]);//J5 2031
    pilaVSGF.push(vGroupF[2]); pilaVSGF.push(vGroupF[3]); pilaVSGF.push(vGroupF[0]); pilaVSGF.push(vGroupF[1]);//J4 1032
    pilaVSGF.push(vGroupF[2]); pilaVSGF.push(vGroupF[1]); pilaVSGF.push(vGroupF[3]); pilaVSGF.push(vGroupF[0]);//J3 0312
    pilaVSGF.push(vGroupF[3]); pilaVSGF.push(vGroupF[1]); pilaVSGF.push(vGroupF[2]); pilaVSGF.push(vGroupF[0]);//J2 0213
    pilaVSGF.push(vGroupF[3]); pilaVSGF.push(vGroupF[2]); pilaVSGF.push(vGroupF[1]); pilaVSGF.push(vGroupF[0]);//J1 0123
    //Grupo G en pilaGG  resultados
    pilaVSGG.push(vGroupG[1]); pilaVSGG.push(vGroupG[2]); pilaVSGG.push(vGroupG[0]); pilaVSGG.push(vGroupG[3]);//J6 3021
    pilaVSGG.push(vGroupG[1]); pilaVSGG.push(vGroupG[3]); pilaVSGG.push(vGroupG[0]); pilaVSGG.push(vGroupG[2]);//J5 2031
    pilaVSGG.push(vGroupG[2]); pilaVSGG.push(vGroupG[3]); pilaVSGG.push(vGroupG[0]); pilaVSGG.push(vGroupG[1]);//J4 1032
    pilaVSGG.push(vGroupG[2]); pilaVSGG.push(vGroupG[1]); pilaVSGG.push(vGroupG[3]); pilaVSGG.push(vGroupG[0]);//J3 0312
    pilaVSGG.push(vGroupG[3]); pilaVSGG.push(vGroupG[1]); pilaVSGG.push(vGroupG[2]); pilaVSGG.push(vGroupG[0]);//J2 0213
    pilaVSGG.push(vGroupG[3]); pilaVSGG.push(vGroupG[2]); pilaVSGG.push(vGroupG[1]); pilaVSGG.push(vGroupG[0]);//J1 0123
    //Grupo H en pilaGH  resultados
    pilaVSGH.push(vGroupH[1]); pilaVSGH.push(vGroupH[2]); pilaVSGH.push(vGroupH[0]); pilaVSGH.push(vGroupH[3]);//J6 3021
    pilaVSGH.push(vGroupH[1]); pilaVSGH.push(vGroupH[3]); pilaVSGH.push(vGroupH[0]); pilaVSGH.push(vGroupH[2]);//J5 2031
    pilaVSGH.push(vGroupH[2]); pilaVSGH.push(vGroupH[3]); pilaVSGH.push(vGroupH[0]); pilaVSGH.push(vGroupH[1]);//J4 1032
    pilaVSGH.push(vGroupH[2]); pilaVSGH.push(vGroupH[1]); pilaVSGH.push(vGroupH[3]); pilaVSGH.push(vGroupH[0]);//J3 0312
    pilaVSGH.push(vGroupH[3]); pilaVSGH.push(vGroupH[1]); pilaVSGH.push(vGroupH[2]); pilaVSGH.push(vGroupH[0]);//J2 0213
    pilaVSGH.push(vGroupH[3]); pilaVSGH.push(vGroupH[2]); pilaVSGH.push(vGroupH[1]); pilaVSGH.push(vGroupH[0]);//J1 0123
}

//METODOS DEL SISTEMA DE JUEGO DE LAS ELIMINATORIAS
//se guardan los clubes clasificados para las siguientes rondas de eliminatoria
void Save1y2GroupsClubs(){
    int numRand = 0, newIdeClubs = 1;
    iClub auxClubErase;
    SortGroups();

    //se guardan los primeros de grupo en su vector
    vClubs1Groups.insert(vClubs1Groups.end(), vGroupA[0]); vClubs1Groups.insert(vClubs1Groups.end(), vGroupB[0]);
    vClubs1Groups.insert(vClubs1Groups.end(), vGroupC[0]); vClubs1Groups.insert(vClubs1Groups.end(), vGroupD[0]);
    vClubs1Groups.insert(vClubs1Groups.end(), vGroupE[0]); vClubs1Groups.insert(vClubs1Groups.end(), vGroupF[0]);
    vClubs1Groups.insert(vClubs1Groups.end(), vGroupG[0]); vClubs1Groups.insert(vClubs1Groups.end(), vGroupH[0]);

    //id nuevo a los primeros de grupo
    for(int i = 0; i < vClubs1Groups.size(); i++){
        vClubs1Groups[i].ID = newIdeClubs;
        newIdeClubs++;
    }

    //se guardan los primeros de grupo en una pila
    for(int i = 0; i < vClubs1Groups.size(); i++){
        pila1Groups.push(vClubs1Groups[i]);
        pilaVSPlayOff1.push(vClubs1Groups[i]);
        vClubsPlayOff.insert(vClubsPlayOff.end(), vClubs1Groups[i]);
    }

    //se guardan los segundos de grupo en su vector
    vClubs2Groups.insert(vClubs2Groups.end(), vGroupA[1]); vClubs2Groups.insert(vClubs2Groups.end(), vGroupB[1]);
    vClubs2Groups.insert(vClubs2Groups.end(), vGroupC[1]); vClubs2Groups.insert(vClubs2Groups.end(), vGroupD[1]);
    vClubs2Groups.insert(vClubs2Groups.end(), vGroupE[1]); vClubs2Groups.insert(vClubs2Groups.end(), vGroupF[1]);
    vClubs2Groups.insert(vClubs2Groups.end(), vGroupG[1]); vClubs2Groups.insert(vClubs2Groups.end(), vGroupH[1]);

    //id nuevo a los segundos de grupo
    for(int i = 0; i < vClubs2Groups.size(); i++){
        vClubs2Groups[i].ID = newIdeClubs;
        newIdeClubs++;
    }

    //desordenando vector de segundos de grupo
    for(int i = 0; i < 50; i++){
        numRand = 1 + rand() % 8;

        numRand = numRand + 8;

        for(auto iter = vClubs2Groups.begin(); iter != vClubs2Groups.end(); ++iter){
            if(iter->ID == numRand){
                auxClubErase.name = iter->name;
                auxClubErase.ID = iter->ID;
                auxClubErase.points = iter->points;
                auxClubErase.machP = iter->machP;
                auxClubErase.machW = iter->machW;
                auxClubErase.machL = iter->machL;
                auxClubErase.machE = iter->machE;
                auxClubErase.golesF = iter->golesF;
                auxClubErase.golesC = iter->golesC;
                auxClubErase.skill = iter->skill;
                iter = vClubs2Groups.erase(iter);
                break;
            }
        }

        vClubs2Groups.insert(vClubs2Groups.end(), auxClubErase);
    }

    //se guardan los segundos de grupo en su pila
    for(int i = 0; i < vClubs2Groups.size(); i++){
        pila2Groups.push(vClubs2Groups[i]);
        pilaVSPlayOff2.push(vClubs2Groups[i]);
        vClubsPlayOff.insert(vClubsPlayOff.end(), vClubs2Groups[i]);
    }
}

//menu de titulo eliminatoria
void MenuTittlePlayOff(){
    if(removed == 16) cout << " OCTAVOS DE FINAL!";
    else if(removed == 8) cout << " CUARTOS DE FINAL!";
    else if(removed == 4) cout << " SEMIFINALES!";
    else if(removed == 2) cout << " FINAL!";
}

//se muestran los 8vos de final
void View8vos(){
    cout << "EMPAREJAMIENTOS DE ELIMINATORIA"; MenuTittlePlayOff(); cout << "\n" << endl;

    while(!pila1Groups.empty() || !pila2Groups.empty()){
        cout << pila1Groups.top().name << "VS               " << pila2Groups.top().name << endl;
        pila1Groups.pop();
        pila2Groups.pop();
    }
}

void ViewPairingPlayOff(){
    if(removed == 1){
        cout << "¡ELIMINATORIAS TERMINADAS! VEA EL GANADOR EN LA OPCION G" << endl; 
    }
    else{
        if(pairingPlayOffMaked){
            cout << "EMPAREJAMIENTOS HECHOS, JUEGUE UNA ELIMINATORIA." << endl;
        }
        else{
            if(removed == 16) View8vos();
            else{
                PairingProcess();

                cout << "EMPAREJAMIENTOS DE ELIMINATORIA"; MenuTittlePlayOff(); cout << "\n" << endl;

                while(!pila1Groups.empty() || !pila2Groups.empty()){
                    cout << pila1Groups.top().name << "VS               " << pila2Groups.top().name << endl;
                    pila1Groups.pop();
                    pila2Groups.pop();
                }
            }

            pairingPlayOffMaked = true;
        }
    }
}

void PairingProcess(){
    int numRand = 0;
    iClub auxClubErase;
    stack<infoClub> auxClubsPila;

    //desordenando vector de los clubes aun en eliminatorias
    for(int i = 0; i < 100; i++){
        numRand = 1 + rand() % removed;

        for(auto iter = vClubsPlayOff.begin(); iter != vClubsPlayOff.end(); ++iter){
            if(iter->ID == numRand){
                auxClubErase.name = iter->name;
                auxClubErase.ID = iter->ID;
                auxClubErase.points = iter->points;
                auxClubErase.machP = iter->machP;
                auxClubErase.machW = iter->machW;
                auxClubErase.machL = iter->machL;
                auxClubErase.machE = iter->machE;
                auxClubErase.golesF = iter->golesF;
                auxClubErase.golesC = iter->golesC;
                auxClubErase.skill = iter->skill;
                iter = vClubsPlayOff.erase(iter);
                break;
            }
        }

        vClubsPlayOff.insert(vClubsPlayOff.end(), auxClubErase);
    }

    //se guardan los equipos en una pila aux
    for(int i = 0; i < vClubsPlayOff.size(); i++){
        auxClubsPila.push(vClubsPlayOff[i]);
    }

    //se guardan los clubes en las pilas de emparejamientos y vs
    while(!auxClubsPila.empty()){
        pila1Groups.push(auxClubsPila.top());
        pilaVSPlayOff1.push(auxClubsPila.top());
        auxClubsPila.pop();
        pila2Groups.push(auxClubsPila.top());
        pilaVSPlayOff2.push(auxClubsPila.top());
        auxClubsPila.pop();
    }
}

void PlayPlayOFF(){
    int x = 0, y = 0;

    if(removed == 1){
        cout << "¡ELIMINATORIAS TERMINADAS! VEA EL GANADOR EN LA OPCION G" << endl; 
    }
    else{
        if(!pairingPlayOffMaked){
            cout << "VISITE LA OPCION DE EMPAREJAMIENTO DE ELIMINATORIA PRIMERO." << endl;
        }
        else{
            cout << "RESULTADOS DE LA ELIMINATORIA DE"; MenuTittlePlayOff(); cout << "\n" << endl;
            while(!pilaVSPlayOff1.empty() || !pilaVSPlayOff2.empty()){
                do{
                    x = 1 + rand()% pilaVSPlayOff1.top().skill;
                    y = 1 + rand()% pilaVSPlayOff2.top().skill;
                }while(x == y);

                cout << pilaVSPlayOff1.top().name << x <<"-"<< y << "               " << pilaVSPlayOff2.top().name << endl;

                //se elimina jugador uno si ha perdido
                for(auto iter = vClubsPlayOff.begin(); iter != vClubsPlayOff.end(); ++iter){
                    if(iter->ID == pilaVSPlayOff1.top().ID){
                        if(x < y){
                            iter = vClubsPlayOff.erase(iter);
                        }
                        break;
                    }
                }

                //se elimina jugador dos si ha perdido
                for(auto iter = vClubsPlayOff.begin(); iter != vClubsPlayOff.end(); ++iter){
                    if(iter->ID == pilaVSPlayOff2.top().ID){
                        if(x > y){
                            iter = vClubsPlayOff.erase(iter);
                        }
                        break;
                    }
                }

                pilaVSPlayOff1.pop();
                pilaVSPlayOff2.pop();
            }

            //se les asigna un nuevo id a los clubes que ganaron
            for(int i = 0; i < vClubsPlayOff.size(); i++){
                vClubsPlayOff[i].ID = i + 1;
            }
            removed /= 2;
            pairingPlayOffMaked = false;
        }
    }
}

//ganador
void WinnerPlayOff(){
    cout << "EL GANADOR DE LA UEFA CHAMPIONS LEAGUE ES: " << vClubsPlayOff[0].name << endl;

    vClubsGroupF.clear();
    vClubsMessy.clear();
    vClubs1Groups.clear();
    vClubs2Groups.clear();
    vClubsPlayOff.clear();
    vGroupA.clear(); vGroupB.clear(); vGroupC.clear(); vGroupD.clear(); vGroupE.clear(); vGroupF.clear(); vGroupG.clear(); vGroupH.clear();

    clubsSignedUp = false, clubsInGroups = false, matchMaked = false, phaseGroupsFinish = false;
    pairingPlayOffMaked = false;
    numPhase = 1, removed = 16;
}
