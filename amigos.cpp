///////// 2020. GNU GENERAL PUBLIC LICENSE /////////////////////////
//
//  Project     : Prueba de diferentes algoritmos de ordenamiento
//  File        : amigos.cpp
//  Description :
//      Archivo principal. Codigo para cumplir con requerimientos de
//              proyecto.
//
//  Authors     : E. Rodriguez
//
//  Git repository: https://github.com/bejarane/CA3125_friends
////////////////////////////////////////////////////////////////////


#include <bits/stdc++.h>
using namespace std;


void gruposAmigos (vector <vector <pair<int,int>>> data){
    vector <int> visitado (data.size(),-1);
    stack <pair<int, int>> pila;
    for (int i=data.size()-1; i >= 0; i--){
        pair <int, int> arista (i,i);
        pila.push(arista);
    }


    while (!pila.empty()){
        pair <int,int> arista = pila.top();
        pila.pop();
        int nodo = arista.first;
        if (visitado[nodo] > -1) continue; //skip, porque ya esta asignado
        int padre = arista.second;
        visitado[nodo] = padre;

        for (int j =0; j < data[nodo].size(); j++){
            pair <int,int> nArista (data[nodo][j].first,nodo);
            pila.push(nArista);
        }

    }

    //imprime el resultado en lineas separadas por grupo
    int base, siguiente = 0;

    do{
        base = siguiente;
        siguiente = data.size()-1;
        for (int i=0; i < visitado.size(); i++){
            int padre = visitado[i];
            if (padre == base){
                cout << i << ' ';
            }else{
                padre < siguiente && padre >= base ? siguiente = padre:base;
            }
        }
        cout << endl;
    } while (base != siguiente);
    cout << endl;
}






int main(){
    int n,l; //n es el numero de nodos y l la cantidad de lineas

    cin >> n >> l;

    vector <vector <pair<int,int>>> data (n);


    while (l > 0){
        char type = 0;

        cin >> type;

        if (type == 'A'){
            //un input
            int a, b, c;
            cin >> a >> b >> c;
            pair <int,int> nodo (b,c);
            data[a].push_back(nodo);
            cout << "Recibido ingresar" << a << nodo.first << nodo.second << endl;
        }else{
            //un query
            char query [3] = {'\0'}; //array lleno de null para string
            cin >> query;

            //cout << "El query es " << query << endl;

            if (strcmp(query, "GA")==0) { //http://www.cplusplus.com/reference/cstring/strcmp/
                cout << "Query es GA" << endl;
                gruposAmigos(data);
            } 
        }
                  
        l--;
    }

    return 0;
}