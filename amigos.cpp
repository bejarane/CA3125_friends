///////// 2020. GNU GENERAL PUBLIC LICENSE /////////////////////////
//
//  Project     : Los amigos internacionales
//  File        : amigos.cpp
//  Description :
//      Archivo principal. Codigo para cumplir con requerimientos del
//              proyecto 2.
//
//  Authors     : E. Rodriguez
//
//  Git repository: https://github.com/bejarane/CA3125_friends
////////////////////////////////////////////////////////////////////


#include <bits/stdc++.h>
using namespace std;


#define TEST(a,b) (a&(1<<b))//macro que prueba si hay un 1 en la posicion
#define CLEAR(a,b) (a & ~(1<<b)) //hace un and con el contrario, siempre va a eliminar el bit en la posicion b
#define INF 1e9

/**
 * Estructura para contener las bases de datos utilizadas en los diferentes algoritmos
 * Estaba principalmente enfocada para el algoritmo del clique, pero se extendio a otros
 * usos.
 */
struct Cliquedata {
    vector <vector <pair<int,int>>> data;
    int tamano = 0;
    unsigned long int cambios = 0;

    int *visitado;
    bool *esClique;
    vector <vector <int>> matriz;
    pair <int, unsigned long int> maximo;

    Cliquedata (int N){
        tamano = N;
        data.resize(N);
        visitado = new int[1<<N]{-1};
        esClique = new bool[1<<N]{-1};

        for (int i = 0; i < N; i++){
            visitado[1<<i] = esClique[1<<i] = 1;
        }
    }

    ~Cliquedata(){//eliminar la cache
        delete[] visitado;
        delete[] esClique;
    }

    /**
     * Actualiza la matriz de adyacencia cargando las nuevas
     * aristas que fueran agregadas en el grafo
     */
    void update(){
        vector <int> base (data.size(),0);
        matriz.resize(data.size(),base);//cuadrada

        for (unsigned int i = 0; i < data.size(); i++){
            matriz[i][i] = 1;
            for (unsigned int j = 0; j < data[i].size();j++){
                matriz[i][data[i][j].first] = 1;
            }
        }
    }
};

/**
 * Imprime los grupos que fueron creados por el algoritmo de union o dijkstra
 * @param grupos Vector de vectores de enteros que contienen los grupos
 */
void printGrupos(vector <vector<int>> grupos){
    for ( unsigned int j=0; j<grupos.size();j++){
        for (unsigned int k =0; k<grupos[j].size();k++){
            cout << grupos[j][k] << ' ';
        }
        cout << endl;
    }
    cout << endl;  
}

/**
 * Extrae el padre de un nodo y actualiza los valores almacenados. 
 * En caso de no estar definido, retorna el nodo mismo
 * @param visitado Vector de enteros que contiene la lista de padres.
 * @param x Nodo que se desea consultar.
 * @return Entero, padre del nodo consultado.
 */
int getPadre(vector<int> &visitado, int x){
    if(visitado[x]==-1) return x;
    return visitado[x]=getPadre(visitado,visitado[x]);
}

/**
 * Analiza las aristas ingresadas y genera grupos de adyacencia
 * @param data vector de vectores de pares que contiene la lista de aristas.
 * @return Vector de vector de enteros que contiene los grupos de adyacencia
 */
vector <vector <int>> gruposAmigos (vector <vector <pair<int,int>>> data){
    vector <int> visitado (data.size(),-1);
    queue <pair<int, int>> pila;

    for (unsigned int i=0; i < data.size(); i++){//se llenan los casos
        for (unsigned int j = 0; j < data[i].size();j++){
            pair <int, int> arista (i,data[i][j].first);
            pila.push(arista);
        }
    }

    while (!pila.empty()){
        pair <int,int> arista = pila.front();
        pila.pop();
        int ni = arista.first;
        int nj = arista.second;
        bool com = getPadre(visitado,ni)==getPadre(visitado,nj);
        if(!com) visitado[getPadre(visitado,ni)]=getPadre(visitado,nj);
    }

    //imprime el resultado en lineas separadas por grupo
    vector <vector<int>> buffer(data.size()); //se separan en grupos para clique e impresion

    for(unsigned int i =0; i<visitado.size();i++){
        if (getPadre(visitado,i)==-1){
            buffer[i].push_back(i);
        }else{
            buffer[getPadre(visitado,i)].push_back(i);
        }
    }

    vector <vector<int>> grupos;

    for(unsigned int i =0; i<buffer.size();i++){
        if (buffer[i].size()>0){
            grupos.push_back(buffer[i]);
        }
    }
    return grupos;
}

/**
 * Convierte un grupo de adyacencia a una mascara de bits para agilizar
 * operaciones.
 * @param grupo vector de enteros que contiene la lista de adyacencia.
 * @return Entero, mascara de bits que representa el vector.
 */
unsigned long int grupoAmascara (vector <int> grupo){
    unsigned long int buffer =0;
    for (unsigned int j = 0; j < grupo.size();j++){
        buffer |= 1<<grupo[j]; //corre el 1 id veces, siendo id el identificador del nodo, maximo 20
    }
    return buffer;
}

/**
 * Convierte un entero de mascara de bits a su representacion vectorial
 * @param mascara entero que contiene la mascara de bits
 * @return Vector de enteros que representa el grupo contenido en la mascara
 */
vector <int> mascaraAgrupo(unsigned long int mascara){
    vector <int> buffer;
    for (int j=0; mascara; j++){
        if (mascara & 1){
            buffer.push_back(j);
        }
        mascara = mascara>>1;
    }
    return buffer;
}

/**
 * Imprime una matriz de adyacencia en la consola
 * @param data vector de vectores de enteros que contiene la matriz
 */
void printMatriz (vector<vector <int>> data){
    for (unsigned int i = 0; i < data.size(); i++){
        for (unsigned int j = 0; j < data[i].size();j++){
            cout << data[i][j] << ' ';       
        }
        cout << '\n';
    }
}

//contador de bits http://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetNaive
/**
 * Cuenta la cantidad de bits encendidos dentro de una máscara de bits
 * @param mascara entero que contiene la máscara de bits
 * @return entero, la cantidad de bits encendidos dentro de la máscara
 */
int maxCount(unsigned long int mascara){
    int contador; //maximo 20
    for (contador = 0; mascara; contador++){
        mascara &= mascara - 1; // clear the least significant bit set
    }
    return contador; 
}

/**
 * Evalua si la mascara de bits indicada contiene un clique.
 * Además, almacena en memoria los resultados para futuras evaluaciones.
 * @param mascara Mascara de bits a evaluar
 * @param mainData Arreglo que contiene los datos relacionados al grafo
 * y que debe continuar existiendo una vez que finalice el algoritmo
 * para reducir calculos en proximas iteraciones.
 * @return Booleano, retorna 1 si es clique y 0 en caso contrario.
 */
bool clique(unsigned long int mascara, Cliquedata &mainData){
    if (mascara == 0)return 0;
    if(mainData.visitado[mascara]==1){//ssi ya lo visitaron
        //si el numero buscado está entre los cambiados, solo puede ser saltado si ya es un clique
        //si no existe clique, este pudo haber cambiado, por tanto, se recalcula
        //cuando hay un cambio, solo puede generar un clique, nunca quitar uno
        if(!((mascara&mainData.cambios)>0) || mainData.esClique[mascara]==1){
            if(mainData.esClique[mascara]){
                int contador = maxCount(mascara);
                if (contador>mainData.maximo.first){ //se calcula un maximo en sitio
                    mainData.maximo.second= mascara;
                    mainData.maximo.first = contador;
                }
            }
            return mainData.esClique[mascara];
        }
    }

    mainData.visitado[mascara]=1;

    int i,j;
    for (i=0;!TEST(mascara,i); i++);
    for (j=i+1; !TEST(mascara,j); j++);
    
    int a = clique(CLEAR(mascara,i),mainData);
    
    int b = clique(CLEAR(mascara,j),mainData);
    bool unClique = a && b && mainData.matriz[i][j];

    mainData.esClique[mascara] = unClique;
    
    if (unClique){
        int contador = maxCount(mascara);
        if (contador>mainData.maximo.first){
            mainData.maximo.second= mascara;
            mainData.maximo.first = contador;
        }
        return 1;
    }
    return 0;
}

/**
 * Genera posibles cliques a partir de un grupo de nodos adyacentes
 * Se requiere esta aproximación porque el algoritmo de clique no evalúa
 * todas las posibilidades, ocasionando que descarte grupos de gran tamaño 
 * solo por omisión
 * @param mascara entero que contiene la mascara del grupo original
 * @param restriccion par de enteros que indica el minimo y el maximo
 * del subconjunto al que se deben extraer todas las posibilidades.
 * @param mainData estructura de datos con la informacion requerida por el algoritmo
 * @return Entero, siempre retorna 1, el retorno se usa como control de ejecución
 */
int generadorClique(int mascara, pair<int,int> restriccion, Cliquedata &mainData){
    if (mascara==0)return 1;
    if (restriccion.first > restriccion.second)return 1;
    clique(mascara,mainData); //clique con el bit
    int buffer = CLEAR(mascara,restriccion.first);
    clique(buffer,mainData); //clique sin el bit
    restriccion.first ++;
    generadorClique(mascara,restriccion,mainData);//universo con el bit
    generadorClique(buffer,restriccion,mainData);//universo sin el bit

    return 1;
}

/**
 * Encuentra el clique maximo para cada grupo de nodos adyances encontrados anteriormente
 * con el algoritmo de union find, aquel clique maximal que además contiene la mayor
 * cantidad de nodos posibles.
 * @param mainData estructura de datos con la informacion requerida por el algoritmo
 */
void cliqueMax (Cliquedata &mainData){
    vector<vector<int>> resultados;

    mainData.update(); //actualiza la matriz

    vector <vector<int>> grupos = gruposAmigos(mainData.data); 

    for (unsigned int j =0; j< grupos.size();j++){//para cada grupo
        mainData.maximo.first = 1;//si no hay clique maximo, como minimo,
    //el clique es cualquier nodo del grupo. Se toma el primer nodo siempre.
        mainData.maximo.second = (1<<(grupos[j][0]));
        
        int buffer = grupoAmascara(grupos[j]);
        int k,l;
        for (k=0;(buffer>>k)>1;k++);//encuentra el bit más alto
        for (l=0;!TEST(buffer,l);l++);
        pair<int,int> restriccion (l,k);

        generadorClique(buffer,restriccion,mainData);
        
        resultados.push_back(mascaraAgrupo(mainData.maximo.second));
    }
    printGrupos(resultados);
    mainData.cambios=0;
}

/**
 * Encuentra el camino optimo desde un punto de inicio hasta un destino
 * Para esto se utiliza el algoritmo de dijkstra
 * @param f Punto de inicio
 * @param d Pundo de destino
 * @param m cantidad de minutos que dura el mensaje
 * @param data vector de vectores de pares con las aristas del grafo
 * @return Entero, costo optimo de envio del mensaje
 */
int dijkstra(int f, int d, int m, vector<vector<pair<int,int>>> data){
    vector <int> costo (data.size(),INF);
    stack <pair<int,int>> pila;

    pila.push(pair<int,int>(f,0));
    costo[f]=0;

    while(!pila.empty()){
        pair <int,int> caso = pila.top(); pila.pop();
        //if(caso.first == d) break; //dudoso, ocasiona que no se evaluén todas las posibilidades
        for(unsigned int j=0; j<data[caso.first].size();j++){
            int a = data[caso.first][j].first;
            int b = data[caso.first][j].second;
            if(costo[caso.first]+b < costo[a]){
                costo[a] = costo[caso.first]+b;
                pila.push(pair <int,int>(a,costo[b]));
            }
        }
    }
    return costo[d]*m;
}

/**
 * Calcula el costo de un mensaje filtrando primero aquellos mensajes que no
 * pueden ser enviados y utilizando dijkstra para aquellos que pueden ser
 * calculado
 * @param f Punto de inicio
 * @param d Pundo de destino
 * @param m cantidad de minutos que dura el mensaje
 * @param data vector de vectores de pares con las aristas del grafo
 * @return Devuelve 1 o 0, se usa como método de control del proceso
 */
int costoMensaje(int f, int d, int m, Cliquedata &mainData){
    vector <vector <int>> grupos = gruposAmigos(mainData.data);

    for (unsigned int j = 0; j< grupos.size();j++){
        bool a = false, b = false;
        for (unsigned int k = 0; k< grupos[j].size();k++){
            grupos[j][k]==f?a=true:0;
            grupos[j][k]==d?b=true:0;
        }
        if(a && b){
            cout << dijkstra(f,d,m,mainData.data)<<"\n\n";
            return 1;
        }
    }
    cout << "INF\n\n";
    return 0;
}

/**
 * Función principal del programa, se encarga de leer la entrada de datos,
 * actualizar las estructuras de almacenamiento y llamar a los respectivos
 * método para resolver las consultas del usuario.
 */
int main(){
    int n,l; //n es el numero de nodos y l la cantidad de lineas

    cin >> n >> l;

    Cliquedata mainData(n);

    while (l > 0){
        char type = 0;

        cin >> type;

        if (type == 'A'){
            //un input
            int a, b, c;
            cin >> a >> b >> c;

            //siempre hacer dobles relaciones para dijkstra
            pair <int,int> nodo (a,c);
            mainData.data[b].push_back(nodo);
            pair <int,int> nodo2 (b,c);
            mainData.data[a].push_back(nodo2);

            //la mascara de cambios permite reciclar cache en algoritmos
            //de clique maximo
            mainData.cambios |= 1<<a; //registra un cambio en a
            mainData.cambios |= 1<<b; //registra un cambio en b
        }else{
            char query [3] = {'\0'}; //array lleno de null para string
            cin >> query;

            if (strcmp(query, "GA")==0) { //http://www.cplusplus.com/reference/cstring/strcmp/
                printGrupos(gruposAmigos(mainData.data));
            }

            if (strcmp(query, "TA")==0) {
                cliqueMax(mainData);
            }

            if (strcmp(query, "CM")==0) {
                int f,d,m;
                cin>>f>>d>>m;
                costoMensaje(f,d,m,mainData);
            } 
        }         
        l--;
    }
    return 0;
}