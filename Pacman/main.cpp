//#include "mapeartabla.cpp"
#include <allegro.h>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

#include <vector>
#include <queue>

#include <list>
using std::list;

using namespace std; 

#define ROWS  31     // y
#define COLS  29 //x
#define WIDTH_SCREEN  640
#define HEIGHT_SCREEN 500


#define MAXIMO 10000 //maximo numero de vértices
#define Node pair< int , int > //definimos el nodo como un par( first , second ) donde first es el vertice adyacente y second el peso de la arista
#define INF 1<<30 //definimos un valor grande que represente la distancia infinita inicial, basta conque sea superior al maximo valor del peso en alguna de las aristas


BITMAP *BUFFER;
BITMAP *WALL_STRAIGHT, *WALL_HORIZONTAL, *WALL_CORNER_UR, *WALL_CORNER_UL, *WALL_CORNER_LL, *WALL_CORNER_LR, *GHOST_DOOR;
BITMAP *FOOD, *POWER_FOOD, *FRUIT;
BITMAP *MENU_1,*MENU_2,*MOUSE;

SAMPLE *EAT, *WALK,*DEAD,*INTRO, *EAT_FRUIT, *GAME_OVER;
//int posx,posy;
int sound, score;
clock_t tiempo;
float tiempoSuper;
vector <int> vectorPuntos;
int pos_xP, pos_yP;

struct Point
 {
    int x;
    int y;
 }; 
    
int track_pos(int x, int y);
Point track_inverse(int n);
//dijkstra( int inicial );
char table[ROWS][COLS] = { //board representation
    "<------------><------------>",
    "IoooooxooooooIIooooooxoooooI",
    "Io<-->o<--->oIIo<--->o<-->oI",
    "IOI  IoI   IoIIoI   IoI  IOI",
    "IoC--VoC---VoCVoC---VoC--VoI",
    "IxooooxooxooxoFxooxooxooooxI",
    "Io<-->o<>o<------>o<>o<-->oI",
    "IoC--VoIIoC--><--VoIIoC--VoI",
    "IoooooxIIooooIIooooIIxoooooI",
    "C---->oIC--> II <--VIo<----V",
    "     IoI<--V CV C-->IoI     ",
    "     IoII   x  x   IIoI     ",
    "     IoII <--__--> IIoI     ",
    "-----VoCV I      I CVoC-----",
    "      x  xI      Ix  x      ",
    "----->o<> I      I <>o<-----",
    "     IoII C------V IIoI     ",
    "     IoIIx        xIIoI     ",
    "     IoII <------> IIoI     ",
    "<----VoCV C--><--V CVoC---->",
    "IoooooxooxoooIIoooxooxoooooI",
    "Io<-->o<--->oIIo<--->o<-->oI",
    "IoC->IoC---VoCVoC---VoI<-VoI",
    "IOooIIxooxoox  xooxooxIIooOI",
    "C->oIIo<>o<------>o<>oIIo<-V",
    "<-VoCVoIIoC--><--VoIIoCVoC->",
    "IooxoooIIooooIIooooIIoooxooI",
    "Io<----VC-->oIIo<--VC---->oI",
    "IoC--------VoCVoC--------VoI",
    "IoooooooooooxooxoooooooooooI",
    "C--------------------------V",
};

class Punto {
    int x, y;
public:
    void setX(int m) {x = m;}
    void setY(int n) {y = n;}
    int getX(){return x;}
    int getY(){return y;}
};

bool wasHere[ROWS][COLS];
bool correctPath[ROWS][COLS];
int gposx = 18, gposy = 1;
int posx = 1, posy = 1;

Punto puntos[ROWS][COLS];

struct cmp {
    bool operator() ( const Node &a , const Node &b ) {
        return a.second > b.second;
    }
};


vector< Node > ady[ MAXIMO ]; //lista de adyacencia
int distancia[ MAXIMO ];      //distancia[ u ] distancia de vértice inicial a vértice con ID = u
bool visitado[ MAXIMO ];      //para vértices visitados
priority_queue< Node , vector<Node> , cmp > Q; //priority queue propia del c++, usamos el comparador definido para que el de menor valor este en el tope
int V;                     //numero de vertices
int previo[ MAXIMO ];         //para la impresion de caminos

bool recursiveSolve(int x, int y);

//función de inicialización
void init(){
    for( int i = 0 ; i <= V ; ++i ){
        distancia[ i ] = INF;  //inicializamos todas las distancias con valor infinito
        visitado[ i ] = false; //inicializamos todos los vértices como no visitados
        previo[ i ] = -1;      //inicializamos el previo del vertice i con -1
    }
}

//Paso de relajacion
void relajacion( int actual , int adyacente , int peso ){
    //Si la distancia del origen al vertice actual + peso de su arista es menor a la distancia del origen al vertice adyacente
    if( distancia[ actual ] + peso < distancia[ adyacente ] ){
        distancia[ adyacente ] = distancia[ actual ] + peso;  //relajamos el vertice actualizando la distancia
        previo[ adyacente ] = actual;                         //a su vez actualizamos el vertice previo
        Q.push( Node( adyacente , distancia[ adyacente ] ) ); //agregamos adyacente a la cola de prioridad
    }
}

//Impresion del camino mas corto desde el vertice inicial y final ingresados
void print( int destino){
    Point punto;
    if( previo[ destino ] != -1 )    //si aun poseo un vertice previo
        print( previo[ destino ]);  //recursivamente sigo explorando

    vectorPuntos.push_back(destino);

    //printf("Destino; %i \n",destino );
    //punto = track_inverse(destino);
    //printf("(%d, %d) " , punto.x, punto.y);        //terminada la recursion imprimo los vertices recorridos
    //printf(" %d " , destino);        //terminada la recursion imprimo los vertices recorridos
}

void dijkstra( int inicial, int destino){
    init(); //inicializamos nuestros arreglos
    Q.push( Node( inicial , 0 ) ); //Insertamos el vértice inicial en la Cola de Prioridad
    distancia[ inicial ] = 0;      //Este paso es importante, inicializamos la distancia del inicial como 0
    int actual , adyacente , peso;
    while( !Q.empty() ){                   //Mientras cola no este vacia
        actual = Q.top().first;            //Obtengo de la cola el nodo con menor peso, en un comienzo será el inicial
        Q.pop();                           //Sacamos el elemento de la cola
        if( visitado[ actual ] ) continue; //Si el vértice actual ya fue visitado entonces sigo sacando elementos de la cola
        visitado[ actual ] = true;         //Marco como visitado el vértice actual

        for( int i = 0 ; i < ady[ actual ].size() ; ++i ){ //reviso sus adyacentes del vertice actual
            adyacente = ady[ actual ][ i ].first;   //id del vertice adyacente
            peso = ady[ actual ][ i ].second;        //peso de la arista que une actual con adyacente ( actual , adyacente )
            if( !visitado[ adyacente ] ){        //si el vertice adyacente no fue visitado
                relajacion( actual , adyacente , peso ); //realizamos el paso de relajacion
            }
        }
    }


    //printf( "Distancias mas cortas iniciando en vertice %d\n" , inicial );
    /*for( int i = 1 ; i <= V ; ++i ){
        printf("Vertice %d , distancia mas corta = %d\n" , i , distancia[ i ] );
    }*/

    //puts("\n**************Impresion de camino mas corto**************");
    //printf("Ingrese vertice destino: ");

    //int x, y;
    //scanf("%d %d", &x, &y);

    //int destino;
    //destino = track_pos(x, y);
    //scanf("%d" , &destino );
    //printf("El vertice destino (%d, %d) : %d\n", x, y, destino );
    print( destino );
    //printf("\n");
}

void transform(){
    for (int row = 0; row < ROWS; row++){
        for (int col = 0; col < COLS; col++){
            if(table[row][col] != 'o' and table[row][col] != ' ' and 
                table[row][col] != 'O' and table[row][col] != '_' and 
                table[row][col] != 'F' and table[row][col] != 'x')
                wasHere[row][col] = false;
            else{
                wasHere[row][col] = true;
               // puntos[row][col].setX(col);
               // puntos[row][col].setY(row);
                V++;
            }
            //correctPath[row][col] = false;
        }
    }
//bool b = recursiveSolve(gposy,gposx);
}

void transform2(){
    for (int i = 0; i < (sizeof(wasHere)/sizeof(*wasHere)); i++)
    {
        for (int j = 0; j < (sizeof(wasHere[i])/sizeof(*wasHere[i])-1); j++)
        {
            if(wasHere[i][j] == 1){
                puntos[i][j].setX(i);
                puntos[i][j].setY(j);
            }
            // else{
            //     puntos[i][j].setX(i);
            //     puntos[i][j].setY(j);
            // }
        }
    //cout<<endl;
    }
//bool b = recursiveSolve(gposy,gposx);
}

void updateScreen(){
    blit(BUFFER, screen, 0,0,0,0,WIDTH_SCREEN,HEIGHT_SCREEN);
}

int obtx(int _x, int _bs)
{
    return _x / _bs;
}

int obty(int _y, int _bs)
{
    return _y / _bs;
}

int num_tiles(char id){
    int row, col;
    int _n = 0;

    for (row=0; row<15; row++) {
        for (col=0; col<15; col++) {
            if (table[row][col] == id) ++_n;
        }
    }

    return _n;
}

class Pacman{
    BITMAP *PACMAN_DEAD;
    BITMAP *PACMAN_ICON;
    int dir;//4
    char pos_next, pos_actual; //
    int last_position_x, last_position_y;
    bool isdead;
    int points;
    bool power;

public:
    BITMAP *PACMAN;
        int lifes = 3;
        int pos_x, pos_y;
        bool isSuper;

        Pacman(int x, int y);
        void drawPacman();
        void eatPacman();
        void movePacman();
        void clearPacman();
        int getPosXPacman();
        int getPosYPacman();
        int getLastXPacman();
        int getLastYPacman();
        void drawDeadPacman();
        bool colisionObjeto();
        void isDead(bool flag);
        bool getDead();
        int colisionEnemigo(int b2_x, int b2_y, int b2_w, int b2_h,bool super);
        void setPosXPacman(int x);
        void setPosYPacman(int y);
        void setDirectionPacman(int direc);
};

Pacman::Pacman(int x,int y){
    pos_xP = pos_x = x;
    pos_yP = pos_y = y;
    dir = 4;
    isdead = false;
    lifes = 3;
    power = false;
    points = 0;
    PACMAN_ICON = load_bitmap("pacman-l_2.bmp", NULL);
    PACMAN = create_bitmap(16,16);
}

bool Pacman::colisionObjeto(){

    if (pos_x % 16 == 0 && pos_y % 16 == 0) {

        if (table[obty(pos_y, 16)][obtx(pos_x, 16)] == 'o' or table[obty(pos_y, 16)][obtx(pos_x, 16)] == 'x') {
            score += 10;
            table[pos_y/16][pos_x/16] = ' ';
            play_sample(EAT,255,0,1000,0);
            if (num_tiles('o') == 0) {
                alert("Has conseguido todos los puntos!", NULL, NULL, "&Salir", NULL, 's', 0);
                exit(0);
            }
            return false;
        }
        else if (table[obty(pos_y, 16)][obtx(pos_x, 16)] == 'O'){
            score += 100;
            sound =  play_sample(EAT,255,0,1000,0);
            table[pos_y/16][pos_x/16] = ' ';
            return true;
        }
    }
    else return false;
}

void Pacman::drawPacman(){

    if (dir == 0) PACMAN_ICON = load_bitmap("pacman-l_2.bmp",NULL);
    if (dir == 1 || dir == 4) PACMAN_ICON = load_bitmap("pacman-r_2.bmp",NULL);
    if (dir == 2) PACMAN_ICON = load_bitmap("pacman-u_2.bmp",NULL);
    if (dir == 3) PACMAN_ICON = load_bitmap("pacman-d_2.bmp",NULL);
    blit(PACMAN_ICON, PACMAN,0,0,0,0,16,16);
    draw_sprite(BUFFER, PACMAN, pos_x, pos_y);
}

void Pacman::eatPacman(){
    if (dir == 0) PACMAN_ICON = load_bitmap("pacman-l_4.bmp",NULL);
    if (dir == 1) PACMAN_ICON = load_bitmap("pacman-r_4.bmp",NULL);
    if (dir == 2) PACMAN_ICON = load_bitmap("pacman-u_4.bmp",NULL);
    if (dir == 3) PACMAN_ICON = load_bitmap("pacman-d_4.bmp",NULL);
    blit(PACMAN_ICON, PACMAN,0,0,0,0,16,16);
    draw_sprite(BUFFER, PACMAN, pos_x, pos_y);
    //rest(50);
}

void Pacman::isDead(bool flag){
    isdead = flag;
}

bool Pacman::getDead(){
    return isdead;
}

void Pacman::movePacman(){
    if (pos_x % 16 == 0 && pos_y % 16 == 0) {
            if (key[KEY_LEFT]) {
            pos_actual = table[obty(pos_y, 16)][obtx(pos_x-1, 16)];
                if (pos_actual == 'o' or
                    pos_actual == 'O' or
                    pos_actual == ' ' or
                    pos_actual == 'F' or
                    pos_actual == 'x') dir = 0;
            }
            else if (key[KEY_RIGHT]) {
            pos_actual = table[obty(pos_y, 16)][obtx(pos_x+16, 16)];
                if (pos_actual == 'o' or
                    pos_actual == 'O' or
                    pos_actual == ' ' or
                    pos_actual == 'F' or
                    pos_actual == 'x') dir = 1;
            }
            else if (key[KEY_UP]) {
            pos_actual = table[obty(pos_y-1, 16)][obtx(pos_x, 16)];
                if (pos_actual == 'o' or
                    pos_actual == 'O' or
                    pos_actual == ' ' or
                    pos_actual == 'F' or
                    pos_actual == 'x') dir = 2;
            }
            else if (key[KEY_DOWN]) {
            pos_actual = table[obty(pos_y+16, 16)][obtx(pos_x, 16)];
                if (pos_actual == 'o' or
                    pos_actual == 'O' or
                    pos_actual == ' ' or
                    pos_actual == 'F' or
                    pos_actual == 'x') dir = 3;
            }
        }


    if (dir == 0) {
        pos_actual = table[obty(pos_y, 16)][obtx(pos_x-1, 16)];
        if (pos_actual == 'o' or pos_actual == 'O' or pos_actual == ' ' or pos_actual == 'F' or pos_actual == 'x' )
            if (isSuper) pos_xP = pos_x -= 4;
            else pos_xP = pos_x -= 2;
        else if (pos_x < 0) pos_xP = pos_x = 27*16;
        else dir = 4;
    }
    if (dir == 1) {
        pos_actual = table[obty(pos_y, 16)][obtx(pos_x+16, 16)];
        if (pos_actual == 'o' or pos_actual == 'O' or pos_actual == ' '  or pos_actual == 'F' or pos_actual == 'x' )
            if (isSuper) pos_xP = pos_x += 4;
            else pos_xP = pos_x += 2;
        else if (pos_x >= 27*16) pos_xP = pos_x = 0;
        else dir = 4;
    }
    if (dir == 2) {
        pos_actual = table[obty(pos_y-1, 16)][obtx(pos_x, 16)];
        if (pos_actual == 'o' or pos_actual == 'O' or pos_actual == ' ' or pos_actual == 'F' or pos_actual == 'x' )
            if (isSuper) pos_yP = pos_y -= 4;
            else pos_yP = pos_y -= 2;
        else dir = 4;
    }
    if (dir == 3) {
        pos_actual = table[obty(pos_y+16, 16)][obtx(pos_x, 16)];
        if (pos_actual == 'o' or pos_actual == 'O' or pos_actual == ' ' or pos_actual == 'F' or pos_actual == 'x' )
            if (isSuper) pos_yP = pos_y += 4;
            else pos_yP = pos_y += 2;
        else dir = 4;
    }

}

int Pacman::colisionEnemigo(int b2x, int b2y, int b2w, int b2h, bool super){

    if (( pos_x > b2x + b2w  )  ||  // is pacman on the right side of b2 (enemy)?
        ( pos_y > b2y + b2h  )  ||  // is pacman under b2 (enemy)?
        ( b2x > pos_x + PACMAN->w  )  ||  // is b2 (enemy) on the right side of pacman?
        ( b2y > pos_y + PACMAN->h  ))     // is b2 (enemy) under pacman?
    {
        // no collision
        return 0;
    }

    // collision
    if (super){
        sound = play_sample(EAT_FRUIT,300,150,1000,0);
        score += 200;

        return 2;

    }else{
        rest(1000);
        play_sample(DEAD,300,150,1000,0);
        --lifes;

    if (lifes < 1) {
        sound =  play_sample(GAME_OVER,255,0,1000,0);
        alert("GAME OVER!", NULL, NULL, "&Aceptar", NULL, 'a', 0);
        exit(0);
    }

    drawDeadPacman();
    pos_xP = pos_x = 14*16;
    pos_yP = pos_y = 23*16;
    if (score > 0) score /= 2;

    dir = 4;
    return 1;
    }
}

void Pacman::clearPacman(){
    clear(PACMAN);
}

int Pacman::getPosXPacman(){
    return pos_x;
}

int Pacman::getPosYPacman(){
    return pos_y;
}

int Pacman::getLastXPacman(){
    return last_position_x;
}

int Pacman::getLastYPacman(){
    return last_position_y;
}

void Pacman::drawDeadPacman(){ //ya funciona :D

    for(int i = 0; i <= 9 ; i++){
        //drawBoard();
        if(i == 0) {
            PACMAN_DEAD = load_bitmap("pacman.bmp",NULL);
            blit(PACMAN_DEAD, PACMAN, 0,0,0,0,16,16);
            draw_sprite(BUFFER,PACMAN,pos_x,pos_y);
            updateScreen();
            rest(40);
            }
        if(i == 1) {
            PACMAN_DEAD = load_bitmap("pacman-u_1.bmp",NULL);
            blit(PACMAN_DEAD, PACMAN, 0,0,0,0,16,16);
            draw_sprite(BUFFER,PACMAN,pos_x,pos_y);
            updateScreen();
            rest(40);
            }
        if(i == 2) {
            PACMAN_DEAD = load_bitmap("pacman-u_2.bmp",NULL);
            blit(PACMAN_DEAD, PACMAN, 0,0,0,0,16,16);
            draw_sprite(BUFFER,PACMAN,pos_x,pos_y);
            updateScreen();
            rest(40);
            }
        if(i == 3) {
            PACMAN_DEAD = load_bitmap("pacman-u_3.bmp",NULL);
            blit(PACMAN_DEAD, PACMAN, 0,0,0,0,16,16);
            draw_sprite(BUFFER,PACMAN,pos_x,pos_y);
            updateScreen();
            rest(40);
            }
        if(i == 4) {
            PACMAN_DEAD = load_bitmap("pacman-u_4.bmp",NULL);
            blit(PACMAN_DEAD, PACMAN, 0,0,0,0,16,16);
            draw_sprite(BUFFER,PACMAN,pos_x,pos_y);
            updateScreen();
            rest(40);
            }
        if(i == 5) {
            PACMAN_DEAD = load_bitmap("pacman-u_5.bmp",NULL);
            blit(PACMAN_DEAD, PACMAN, 0,0,0,0,16,16);
            draw_sprite(BUFFER,PACMAN,pos_x,pos_y);
            updateScreen();
            rest(40);
            }
        if(i == 6) {
            PACMAN_DEAD = load_bitmap("pacman-r_5.bmp",NULL);
            blit(PACMAN_DEAD, PACMAN, 0,0,0,0,16,16);
            draw_sprite(BUFFER,PACMAN,pos_x,pos_y);
            updateScreen();
            rest(40);
            }
        if(i == 7) {
            PACMAN_DEAD = load_bitmap("pacman-d_5.bmp",NULL);
            blit(PACMAN_DEAD, PACMAN, 0,0,0,0,16,16);
            draw_sprite(BUFFER,PACMAN,pos_x,pos_y);
            updateScreen();
            rest(40);
            }
        if(i == 8) {
            PACMAN_DEAD = load_bitmap("pacman-l_5.bmp",NULL);
            blit(PACMAN_DEAD, PACMAN, 0,0,0,0,16,16);
            draw_sprite(BUFFER,PACMAN,pos_x,pos_y);
            updateScreen();
            rest(40);
            }
        if(i == 9) {
            PACMAN_DEAD = load_bitmap("blank.bmp",NULL);
            blit(PACMAN_DEAD, PACMAN, 0,0,0,0,16,16);
            draw_sprite(BUFFER,PACMAN,pos_x,pos_y);
            updateScreen();
            rest(40);
            }

        }
}

void Pacman::setPosXPacman(int x){
    pos_xP = pos_x = x;
}

void Pacman::setPosYPacman(int y){
    pos_yP = pos_y = y;
}

void Pacman::setDirectionPacman(int direc){
    dir = direc;
}

int initialize(){
    /* Inicializar la librería */
    allegro_init();
    install_keyboard();
    install_mouse();
    install_timer();
    //remove_timer();
    //install_int_ex(MSEC_TO_TIMER(1000), int speed);

    if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) != 0){
        allegro_message("Error: inicializando sistema de sonido\n%s\n", allegro_error);
        return 1; }

    set_volume(70, 70);

    /* Fijar la densidad del color y la resolución */
    set_color_depth(32);
    if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, WIDTH_SCREEN,HEIGHT_SCREEN, 0, 0) < 0) {
        set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
        allegro_message("Failed to initialize GFX");
        exit(-1);
    }
    return 0;
}

void loadResources(){
    WALL_STRAIGHT = load_bitmap("wall_straight.bmp", NULL) ;
    WALL_HORIZONTAL = load_bitmap("wall_horiz.bmp", NULL);
    WALL_CORNER_UR = load_bitmap("wall_corner_ur.bmp", NULL);
    WALL_CORNER_LR = load_bitmap("wall_corner_lr.bmp", NULL);
    WALL_CORNER_UL = load_bitmap("wall_corner_ul.bmp", NULL);
    WALL_CORNER_LL = load_bitmap("wall_corner_ll.bmp", NULL);
    GHOST_DOOR = load_bitmap("ghost_door.bmp",NULL);
    FOOD = load_bitmap("pellet.bmp",NULL);
    POWER_FOOD = load_bitmap("pellet-power.bmp",NULL);
    MENU_1 = load_bitmap("menu_1.bmp",NULL);
    MENU_2 = load_bitmap("menu_2.bmp",NULL);
    MOUSE = load_bitmap("mouse.bmp",NULL);
    INTRO = load_wav("pacman_beginning.wav");
    EAT = load_wav("pacman_chomp.wav");
    DEAD = load_wav("pacman_death.wav");
    WALK = load_wav("pacman_siren.wav");
    EAT_FRUIT = load_wav("pacman_eatfruit.wav");
    GAME_OVER = load_wav("gameover.wav");
    FRUIT = load_bitmap("fruit.bmp",NULL);
}

void drawBoard(int score, Pacman p){
    int i,j; //i and j * pixels
    for(i = 0; i < ROWS ; i++){
        for(j = 0 ; j < COLS ; j++){
            if(table[i][j] == 'I') draw_sprite(BUFFER, WALL_STRAIGHT, j*16, i*16);
            else if(table[i][j] == '-') draw_sprite(BUFFER, WALL_HORIZONTAL, j*16, i*16);
            else if(table[i][j] == '<') draw_sprite(BUFFER, WALL_CORNER_UL, j*16, i*16);
            else if(table[i][j] == '>') draw_sprite(BUFFER, WALL_CORNER_UR, j*16, i*16);
            else if(table[i][j] == 'C') draw_sprite(BUFFER, WALL_CORNER_LL, j*16, i*16);
            else if(table[i][j] == 'V') draw_sprite(BUFFER, WALL_CORNER_LR, j*16, i*16);
            else if(table[i][j] == '_') draw_sprite(BUFFER, GHOST_DOOR, j*16, i*16);
            else if(table[i][j] == 'o' or table[i][j] == 'x') draw_sprite(BUFFER, FOOD, j*16, i*16);
            else if(table[i][j] == 'O') draw_sprite(BUFFER, POWER_FOOD, j*16, i*16);
            else if(table[i][j] == 'F'){
                if(score >= 1000){
                    draw_sprite(BUFFER,FRUIT,j*16,i*16);
                    if(p.pos_y/16 == i && p.pos_x/16 == j) {
                        table[i][j] = ' ';
                        sound =  play_sample(EAT_FRUIT,300,150,1000,0);
                    }
                }
                else if(score < 1000){
                    draw_sprite(BUFFER,FOOD, j*16, i*16);
                    if(p.pos_y/16 == i && p.pos_x/16 == j) {
                        table[i][j] = ' ';
                        sound = play_sample(EAT,300,150,1000,0);}
                }
            }
        }
    }
}

class Fantasma{

    int gdir = 2; //ghost direction = 0

    char gpos_next;
    char gpos_now;
    char gpos_next_u;
    char gpos_next_d;
    char gpos_next_l;
    char gpos_next_r;
public:
    BITMAP *GHOST;
    BITMAP *GHOST_SCARED;
    BITMAP *GHOST_ICON;
    bool isScared, *isDead;
        int gpos_x, gpos_y, inicial_x, inicial_y; //11 17original position of blinky 11*16 y 14*16
        Fantasma(int x, int y, int type);
        void drawGhost() const;
        void drawGhostScared();
        void deadPacman(Pacman p); //ghosts kills pacman
        void moveGhost();
        void moveGhostMiniMax(int pos_x, int pos_y);
        int getPosXGhost();
        int getPosYGhost();
};

Fantasma::Fantasma(int x, int y, int type){
    gpos_x = x;
    gpos_y = y;
    inicial_x = x;
    inicial_y = y;
    gdir = rand()%4;

    GHOST_SCARED = load_bitmap("ghost.bmp",NULL);
    isScared = false;

     if (type == 1){
    GHOST_ICON = load_bitmap("ghost-blinky.bmp",NULL);
    GHOST = create_bitmap(16,16);}
    else if(type == 2){
    GHOST_ICON = load_bitmap("ghost-inky.bmp",NULL);
    GHOST = create_bitmap(16,16);}
    else if(type == 3){
    GHOST_ICON = load_bitmap("ghost-pinky.bmp",NULL);
    GHOST = create_bitmap(16,16);}
    else if(type == 4){
    GHOST_ICON = load_bitmap("ghost-sue.bmp",NULL);
    GHOST = create_bitmap(16,16);}
}

int Fantasma::getPosXGhost(){
    return gpos_x;
}

int Fantasma::getPosYGhost(){
    return gpos_y;
}

void Fantasma::drawGhost() const{
    blit(GHOST_ICON, GHOST,0,0,0,0,16,16);
    draw_sprite(BUFFER, GHOST, gpos_x, gpos_y);
}

void Fantasma::drawGhostScared(){
    //blit(GHOST_SCARED, GHOST,0,0,0,0,16,16);
    draw_sprite(BUFFER, GHOST_SCARED, gpos_x, gpos_y);
}

void Fantasma::deadPacman(Pacman p){ //verificar funcion muchos a uno
    if(p.getPosXPacman() == gpos_x and p.getPosYPacman() == gpos_y or gpos_x == p.getLastXPacman() and gpos_y == p.getLastYPacman()){
        p.isDead(true);
        p.drawDeadPacman();
        p.lifes --;
    }
}

void Fantasma::moveGhostMiniMax(int pos_x, int pos_y){

    int coord_Ghost,coord_Pacman;
    //gpos_now = table[gpos_y/16][gpos_x/16];
    //gpos_now_pacman = table[pos_y/16][pos_x/16];
    coord_Ghost = track_pos((gpos_y/16),(gpos_x/16));
    coord_Pacman = track_pos((pos_y/16), (pos_x/16));
    dijkstra( coord_Ghost, coord_Pacman );
    printf("%d %d \n", coord_Ghost, coord_Pacman);
    

    int tamLista = vectorPuntos.size();
    int coord_next = vectorPuntos[1];
    vectorPuntos.clear();

    Point punto_next;
    punto_next = track_inverse(coord_next);
    printf("COORDENADAS (%d,%d) :       %d\n",punto_next.x, punto_next.y ,coord_next );
    printf("GHOST %d %d   \n", (gpos_y/16), (gpos_x/16));
    printf("PACMAN %d %d   \n", (pos_y/16), (pos_x/16));

    
    if ((gpos_y/16) == punto_next.x)
    {
        if (gpos_x == 0) gpos_x = 27*16;
        if (gpos_x == 27*16) gpos_x = 0;

        if ((gpos_x/16) < punto_next.y) { gpos_x +=2; }
        else  if((gpos_x/16) > punto_next.y) { gpos_x -=2;}
    }

    if ((gpos_x/16) == punto_next.y)
    {
        if ((gpos_y/16) < punto_next.x) { gpos_y +=2; }
        else  if ((gpos_y/16) > punto_next.x) { gpos_y -=2;}
    }
}

void Fantasma::moveGhost(){

    if (gdir == 0){
        if (gpos_x % 16 == 0 && gpos_y % 16 == 0) {
            gpos_next = table[gpos_y/16][(gpos_x-16)/16];
            gpos_next_u = table[(gpos_y-16)/16][gpos_x/16];
            gpos_next_d = table[(gpos_y+16)/16][gpos_x/16];
            gpos_now = table[gpos_y/16][gpos_x/16];
            
            if(gpos_next== 'o' or gpos_next == ' ' or gpos_next == 'O' or gpos_next == '_' or gpos_next == 'F' or gpos_next== 'x') {
                
                if (gpos_now == 'x'){
                    srand((int)time(0));
                    int val = rand()%3;     //0,1,2 ==> 0,2,3

                    if (val == 0) {
                        gpos_x -=2;
                        gdir = 0;
                    }
                    else if (val == 1){
                        if (gpos_next_u == '-' or gpos_next_u == 'I' or gpos_next_u == '<' or gpos_next_u == '>' or gpos_next_u == 'C' or gpos_next_u == 'V') {
                            
                            srand((int)time(0));
                            val = rand()%2; //0,1 ==> 0,3

                            if (val == 0) {
                                gpos_x -=2;
                                gdir = 0;
                            }
                            else if (val == 1) 
                                if (gpos_next_d == '-' or gpos_next_d == 'I' or gpos_next_d == '<' or gpos_next_d == '>' or gpos_next_d == 'C' or gpos_next_d == 'V') {
                                    gpos_x -=2; 
                                    gdir = 0;
                                }
                                else {
                                    gpos_y +=2;
                                    gdir = 3;
                                } 
                        }else {
                            gpos_y -=2;
                            gdir = 2;
                        }
                    }
                    else if (val == 2) {
                        if (gpos_next_d == '-' or gpos_next_d == 'I' or gpos_next_d == '<' or gpos_next_d == '>' or gpos_next_d == 'C' or gpos_next_d == 'V') {

                            srand((int)time(0));
                            val = rand()%2; //0,1 ==> 0,2

                            if (val == 0) {
                                gpos_x -=2;
                                gdir = 0;
                            }
                            else if (val == 1) 
                                if (gpos_next_u == '-' or gpos_next_u == 'I' or gpos_next_u == '<' or gpos_next_u == '>' or gpos_next_u == 'C' or gpos_next_u == 'V') {
                                    gpos_x -=2; 
                                    gdir = 0;
                                }
                                else {
                                    gpos_y -=2;
                                    gdir = 2;
                                } 
                        }else{
                            gpos_y -=2;
                            gdir = 3;
                        }
                    }
                }else gpos_x -=2;
                
            } 
            else if (gpos_x == 0) gpos_x = 27*16;//atajo
            else {
                srand((int)time(0));
                gdir = (rand()%2) + 2; //  2,3

                if (gdir == 2)
                    if (gpos_next_u == '-' or gpos_next_u == 'I' or gpos_next_u == '<' or gpos_next_u == '>' or gpos_next_u == 'C' or gpos_next_u == 'V') {
                        gdir=3;
                        gpos_y +=2;
                    }else {
                        gdir =2;
                        gpos_y -=2;
                    }
                else if (gdir == 3)
                    if (gpos_next_d == '-' or gpos_next_d == 'I' or gpos_next_d == '<' or gpos_next_d == '>' or gpos_next_d == 'C' or gpos_next_d == 'V') {
                        gdir=2;
                        gpos_y -=2;
                    }else{
                        gdir =3;
                        gpos_y +=2;
                    }
            }

        }else gpos_x -=2;
    }
    if(gdir == 1) {
        if (gpos_x % 16 == 0 && gpos_y % 16 == 0) {
            gpos_next = table[gpos_y/16][(gpos_x+16)/16];

            gpos_next_u = table[(gpos_y-16)/16][gpos_x/16];
            gpos_next_d = table[(gpos_y+16)/16][gpos_x/16];
            gpos_now = table[gpos_y/16][gpos_x/16];

            if(gpos_next == 'o' or gpos_next == ' ' or gpos_next == '_' or gpos_next == 'O' or gpos_next == 'F' or gpos_next== 'x'){
                
                if (gpos_now == 'x'){
                    srand((int)time(0));
                    gdir = (rand()%3) +1; //1,2,3
                    if (gdir == 1) gpos_x +=2;
                    else if (gdir == 2){
                        if (gpos_next_u == '-' or gpos_next_u == 'I' or gpos_next_u == '<' or gpos_next_u == '>' or gpos_next_u == 'C' or gpos_next_u == 'V') {
                            srand((int)time(0));
                            int val = rand()%2;     //0,1 ==> 1,3

                            if (val == 0){
                                gpos_x +=2;
                                gdir =1;
                            } 
                            else if (val == 1) {
                                if (gpos_next_d == '-' or gpos_next_d == 'I' or gpos_next_d == '<' or gpos_next_d == '>' or gpos_next_d == 'C' or gpos_next_d == 'V'){
                                  gpos_x +=2;  
                                  gdir = 1; 
                                } 
                                else {
                                    gpos_y -=2;
                                    gdir = 3;
                                }
                            }
                        }else gpos_y +=2;
                    }else if (gdir == 3)  gpos_y -=2;
                }else  gpos_x +=2;
            }
            else if (gpos_x == 27*16) gpos_x = 0;//atajo
            else {
                srand((int)time(0));
                gdir = (rand()%2) + 2; //  2,3

                if (gdir == 2)
                    if (gpos_next_u == '-' or gpos_next_u == 'I' or gpos_next_u == '<' or gpos_next_u == '>' or gpos_next_u == 'C' or gpos_next_u == 'V') {
                        gdir=3;
                        gpos_y +=2;
                    }else {
                        gdir =2;
                        gpos_y -=2;
                    }
                else if (gdir == 3)
                    if (gpos_next_d == '-' or gpos_next_d == 'I' or gpos_next_d == '<' or gpos_next_d == '>' or gpos_next_d == 'C' or gpos_next_d == 'V') {
                        gdir=2;
                        gpos_y -=2;
                    }else{
                        gdir =3;
                        gpos_y +=2;
                    }
            }
        }else gpos_x +=2;
    }
    if(gdir == 2) {
        if (gpos_x % 16 == 0 && gpos_y % 16 == 0) {
            gpos_next = table[(gpos_y-16)/16][gpos_x/16];

            gpos_next_l = table[gpos_y/16][(gpos_x-16)/16];
            gpos_next_r = table[gpos_y/16][(gpos_x+16)/16];
            gpos_now = table[gpos_y/16][gpos_x/16];

            if(gpos_next == 'o' or gpos_next == ' ' or gpos_next == 'O' or gpos_next == '_' or gpos_next == 'F' or gpos_next== 'x') {
                
                if (gpos_now == 'x'){
                    srand((int)time(0));
                    gdir = rand()%3; //0,1,2

                    if (gdir == 0){
                        if (gpos_next_l == '-' or gpos_next_l == 'I' or gpos_next_l == '<' or gpos_next_l == '>' or gpos_next_l == 'C' or gpos_next_l == 'V') {
                            srand((int)time(0));
                            gdir = (rand()%2)+1;//1,2
                            if (gdir == 1)
                                if (gpos_next_r == '-' or gpos_next_r == 'I' or gpos_next_r == '<' or gpos_next_r == '>' or gpos_next_r == 'C' or gpos_next_r == 'V') gpos_y -=2; 
                                else gpos_x +=2;
                            else if (gdir == 2) gpos_y -=2;
                        }else gpos_x -=2;
                    }
                    else if (gdir == 1) {
                        if (gpos_next_r == '-' or gpos_next_r == 'I' or gpos_next_r == '<' or gpos_next_r == '>' or gpos_next_r == 'C' or gpos_next_r == 'V') {
                            
                            srand((int)time(0));
                            int val = rand()%2;     //0,1 ==> 0,2

                            if (val == 0){
                                if (gpos_next_l == '-' or gpos_next_l == 'I' or gpos_next_l == '<' or gpos_next_l == '>' or gpos_next_l == 'C' or gpos_next_l == 'V'){
                                    gpos_y -=2;
                                    gdir = 2;
                                }
                                else {
                                    gpos_x -=2; 
                                    gdir = 0;
                                }   
                            }
                            else if (val == 1){
                                gpos_y -=2;
                                gdir =2;                                
                            } 
                        }else gpos_x +=2;
                    }
                    else if (gdir == 2)  gpos_y -=2;
                }else gpos_y -=2; 
            }
            else {
                srand((int)time(0));
                gdir = rand()%2; //0,1

                 if (gdir == 0)
                    if (gpos_next_l == '-' or gpos_next_l == 'I' or gpos_next_l == '<' or gpos_next_l == '>' or gpos_next_l == 'C' or gpos_next_l == 'V') {
                        gdir=1;
                        gpos_x +=2;
                    }else {
                        gdir =0;
                        gpos_x -=2;
                    }
                else if (gdir == 1)
                    if (gpos_next_r == '-' or gpos_next_r == 'I' or gpos_next_r == '<' or gpos_next_r == '>' or gpos_next_r == 'C' or gpos_next_r == 'V') {
                        gdir=0;
                        gpos_x -=2;
                    }else{
                        gdir =1;
                        gpos_x +=2;
                    }
            }
        }else gpos_y -=2;
    }

    if(gdir == 3) {
        if (gpos_x % 16 == 0 && gpos_y % 16 == 0) {
            gpos_next = table[(gpos_y+16)/16][gpos_x/16];

            gpos_now = table[gpos_y/16][gpos_x/16];
            gpos_next_r = table[gpos_y/16][(gpos_x+16)/16];
            gpos_next_l = table[gpos_y/16][(gpos_x-16)/16];

            if(gpos_next == 'o' or gpos_next == ' ' or gpos_next == 'O' or gpos_next == '_' or gpos_next == 'F' or gpos_next== 'x'){
           
                if (gpos_now == 'x'){
                    srand((int)time(0));
                    int val = rand()%3; //0,1,2 ==> 0,1,3

                    if (val == 0){
                        if (gpos_next_l == '-' or gpos_next_l == 'I' or gpos_next_l == '<' or gpos_next_l == '>' or gpos_next_l == 'C' or gpos_next_l == 'V') {
                            srand((int)time(0));
                            val = rand()%2; //0,1 ==> 1,3

                            if (val == 0)
                                if (gpos_next_r == '-' or gpos_next_r == 'I' or gpos_next_r == '<' or gpos_next_r == '>' or gpos_next_r == 'C' or gpos_next_r == 'V'){
                                    gpos_y +=2; 
                                    gdir = 3;
                                } 
                                else {
                                    gpos_x +=2;
                                    gdir = 1;
                                }
                            else if (val == 1) {
                                gpos_y +=2;
                                gdir = 3;
                            }
                        }else {
                            gpos_x -=2;
                            gdir = 0;
                        }
                    }
                    else if (val == 1) {
                        if (gpos_next_r == '-' or gpos_next_r == 'I' or gpos_next_r == '<' or gpos_next_r == '>' or gpos_next_r == 'C' or gpos_next_r == 'V') {

                            srand((int)time(0));
                            val = rand()%2; //0,1 ==> 0,3

                            if (val == 0)
                                if (gpos_next_l == '-' or gpos_next_l == 'I' or gpos_next_l == '<' or gpos_next_l == '>' or gpos_next_l == 'C' or gpos_next_l == 'V') {
                                    gpos_y +=2; 
                                    gdir = 3;
                                }
                                else {
                                    gpos_x -=2;
                                    gdir = 0;
                                }
                            else if (val == 1) {
                                gpos_y +=2;
                                gdir = 3;
                            }
                        }else{
                            gpos_x +=2;
                            gdir =1;
                        } 
                    }
                    else if (val == 2) {
                        gpos_y +=2;
                        gdir = 3;
                    } 
                }else gpos_y +=2;
                
            } 
            else{
                srand((int)time(0));
                gdir = rand()%2; //0,1

                if (gdir == 0)
                    if (gpos_next_l == '-' or gpos_next_l == 'I' or gpos_next_l == '<' or gpos_next_l == '>' or gpos_next_l == 'C' or gpos_next_l == 'V') {
                        gdir=1;
                        gpos_x +=2;
                    }else {
                        gdir =0;
                        gpos_x -=2;
                    }
                else if (gdir == 1)
                    if (gpos_next_r == '-' or gpos_next_r == 'I' or gpos_next_r == '<' or gpos_next_r == '>' or gpos_next_r == 'C' or gpos_next_r == 'V') {
                        gdir=0;
                        gpos_x -=2;
                    }else{
                        gdir =1;
                        gpos_x +=2;
                    }
            }
        }else gpos_y +=2;
    }
}

using namespace std;

void drawInfo(Pacman p){

        rectfill(BUFFER, 455, 0, 640, 493, makecol(101, 12, 104));
        ellipsefill(BUFFER, 545, 50, 80, 30, makecol(0, 0, 0));
        ellipsefill(BUFFER, 545, 130, 80, 30, makecol(0, 0, 0));
        ellipsefill(BUFFER, 545, 210, 80, 30, makecol(0, 0, 0));

        textprintf_ex(BUFFER, font, 490, 50, makecol(255, 255, 255), -1, "Puntos: %06d", score);
        textprintf_ex(BUFFER, font, 510, 130, makecol(255, 255, 255), -1, "Vidas: %d", p.lifes);
        textprintf_ex(BUFFER, font, 507, 205, makecol(255, 0, 0), -1, "TIME: %.2f", ((float)tiempo/CLOCKS_PER_SEC));
}

void createGraph(){
    for (int row = 0; row < ROWS; row++){
        for (int col = 0; col < COLS; col++){
            if(wasHere[row][col] == 1){
                /*cout<<endl;
                cout<<"ESTOY ANALIZANDO EL SIGUIENTE PUNTO: ";
                cout<<endl;
                cout<<"("<<row<<","<<col<<")";
                cout<<endl;
                cout<<endl;*/
                if(wasHere[row+1][col] == 1 ){

                    int origen = track_pos(row,col);
                    int destino = track_pos(row+1,col);

                    /*cout<<"("<<row+1<<","<<col<<")";
                    cout<<endl;
                    cout<<"origen: "<<origen<<"=>"<<"destino: "<<destino;
                    cout<<endl;*/

                    ady[ origen ].push_back( Node( destino, 1 ) ); 
                    //ady[ destino ].push_back( Node( origen , 1 ) );
                }
                if(wasHere[row-1][col] == 1 ){

                    int origen = track_pos(row,col);
                    int destino = track_pos(row-1,col);

                    /*cout<<"("<<row-1<<","<<col<<")";
                    cout<<endl;
                    cout<<"origen: "<<origen<<"=>"<<"destino: "<<destino;
                    cout<<endl;*/

                    ady[ origen ].push_back( Node( destino, 1 ) ); 
                    //ady[ destino ].push_back( Node( origen , 1 ) );

                }
                if(wasHere[row][col+1] == 1 ){

                    int origen = track_pos(row,col);
                    int destino = track_pos(row,col+1);

                    /*cout<<"("<<row<<","<<col+1<<")";
                    cout<<endl;
                    cout<<"origen: "<<origen<<"=>"<<"destino: "<<destino; 
                    cout<<endl;*/

                    ady[ origen ].push_back( Node( destino, 1 ) ); 
                    //ady[ destino ].push_back( Node( origen , 1 ) );

                }
                if(wasHere[row][col-1] == 1 ){

                    int origen = track_pos(row,col);
                    int destino = track_pos(row,col-1);

                    /*cout<<"("<<row<<","<<col-1<<")";
                    cout<<endl;
                    cout<<"origen: "<<origen<<"=>"<<"destino: "<<destino;
                    cout<<endl;*/

                    ady[ origen ].push_back( Node( destino, 1 ) ); 
                    //ady[ destino ].push_back( Node( origen , 1 ) );

                }
            }
        }
    }
}

void solveMaze(){
    for (int row = 0; row < (sizeof(table)/sizeof(*table)); row++){
        for (int col = 0; col < (sizeof(table[row])/sizeof(*table[row])); col++){
            wasHere[row][col] = false;
            correctPath[row][col] = false;
        }
    }
//bool b = recursiveSolve(gposy,gposx);
}

bool recursiveSolve(int gposx, int gposy){
    if(gposx == posx && gposy == posy) return true;
    //gpos_next== 'o' or gpos_next == ' ' or gpos_next == 'O' or gpos_next == '_' or gpos_next == 'F' or gpos_next== 'x'
    if((table[gposx][gposy] != 'o' and table[gposx][gposy] != ' ' and 
        table[gposx][gposy] != 'O' and table[gposx][gposy] != '_' and 
        table[gposx][gposy] != 'F' and table[gposx][gposy] != 'x') || wasHere[gposx][gposy])
    {
        /*cout<<"FALSE";
        cout<<endl;*/
        return false;   
    }
        
    wasHere[gposx][gposy] = true;

    if (gposx != 0){ // Checks if not on left edge
        //cout << "ENTRO 1";
        if (recursiveSolve(gposx-1, gposy)) { // Recalls method one to the left
            correctPath[gposx][gposy] = true; // Sets that path value to true;
            return true;
        }
    }
    if (gposx != COLS - 1){ // Checks if not on right edge
        //cout << "ENTRO 2";
        if (recursiveSolve(gposx+1, gposy)) { // Recalls method one to the right
            correctPath[gposx][gposy] = true;
            return true;
        }
    }
    if (gposy != 0){  // Checks if not on top edge
        //cout << "ENTRO 3";
        if (recursiveSolve(gposx, gposy-1)) { // Recalls method one up
            correctPath[gposx][gposy] = true;
            return true;
        }
    }
    if (gposy != ROWS- 1){ // Checks if not on bottom edge
        //cout << "ENTRO 4";
        if (recursiveSolve(gposx, gposy+1)) { // Recalls method one down
            correctPath[gposx][gposy] = true;
            return true;
        }
    }
    return false;
}

void printMatrix(){
    // for (int i = 0; i < (sizeof(table)/sizeof(*table)); i++)
    // {
    //     for (int j = 0; j < (sizeof(table[i])/sizeof(*table[i])); j++)
    //     {
    //         cout<<table[i][j];
    //     }
    // cout<<endl;
    // }

    //cout<<endl;

    for (int i = 0; i < (sizeof(wasHere)/sizeof(*wasHere)); i++)
    {
        //cout << i;
        for (int j = 0; j < (sizeof(wasHere[i])/sizeof(*wasHere[i])-1); j++)
        {
            cout<<wasHere[i][j];
        }
    cout<<endl;
    }

    for (int i = 0; i < (sizeof(puntos)/sizeof(*puntos)); i++)
    {
        cout << "(";
        for (int j = 0; j < (sizeof(puntos[i])/sizeof(*puntos[i])-1); j++)
        {
            cout<< puntos[i][j].getX()<<","<<puntos[i][j].getY()<<"|";
        }
    cout << ")";
    cout<<endl;
    }
}
int main(){

    initialize();
    loadResources();
    BUFFER = create_bitmap(WIDTH_SCREEN,HEIGHT_SCREEN);

    Pacman pac(14*16,23*16);

    Fantasma f1(9*16,14*16,1);
    Fantasma f2(18*16,14*16,2);
    Fantasma f3(2*16,8*16,3);
    Fantasma f4(12*16,19*16,4);

    score = 0;
    f1.isScared = false;
    

//MENU
    bool salida_menu = false;
    while(!salida_menu){
        if(mouse_x > 102 && mouse_x < 339 && mouse_y > 347 && mouse_y < 404 ){
            blit(MENU_2,BUFFER,0,0,0,0,WIDTH_SCREEN,HEIGHT_SCREEN);
            if(mouse_b & 1){
                salida_menu = true;
            }
        }
        else blit(MENU_1,BUFFER,0,0,0,0,WIDTH_SCREEN,HEIGHT_SCREEN);
        masked_blit(MOUSE,BUFFER,0,0,mouse_x,mouse_y,13,22);
        blit(BUFFER,screen,0,0,0,0,WIDTH_SCREEN,HEIGHT_SCREEN);
    }

//JUEGO
    play_sample(INTRO,255,0,1000,0);

    transform();
    transform2();
    createGraph();

    while(!key[KEY_ESC]){
        srand(time(NULL));
        tiempo = clock();
        if(key[KEY_SPACE]) {
            alert("PAUSE", NULL, NULL, "&Continue", NULL, 'c', 0);
            rest(100);
            }

        if((float)tiempo/CLOCKS_PER_SEC >= 20){
            alert("TIEMPO AGOTADO!", NULL, NULL, "&Salir", NULL, 's', 0);
            exit(0);
        }

        pac.movePacman();
        clear(BUFFER);
        drawBoard(score,pac);
        int xpos,ypos;

        xpos = pac.getPosXPacman();
        ypos = pac.getPosYPacman();

        f1.moveGhostMiniMax(pos_xP , pos_yP );
        f2.moveGhost();
        f3.moveGhost();
        f4.moveGhost();
        rest(5);

        if(pac.colisionObjeto() == 1){
            f1.isScared = true;
            f2.isScared = true;
            f3.isScared = true;
            f4.isScared = true;
            tiempoSuper = 1 + (float)tiempo/CLOCKS_PER_SEC;
            pac.isSuper = true;
        }

        if (pac.colisionEnemigo(f1.gpos_x+4, f1.gpos_y+4, f1.GHOST->w-4, f1.GHOST->h-4,pac.isSuper)>= 1){
            f1.gpos_x = 9*16; f1.gpos_y = 14*16;
        }
        if (pac.colisionEnemigo(f2.gpos_x+4, f2.gpos_y+4, f2.GHOST->w-4, f2.GHOST->h-4,pac.isSuper) >=1){
            f2.gpos_x = 18*16; f2.gpos_y = 14*16;
        }
        if (pac.colisionEnemigo(f3.gpos_x+4, f3.gpos_y+4, f3.GHOST->w-4, f3.GHOST->h-4,pac.isSuper) >=1){
            f3.gpos_x = 9*16; f3.gpos_y = 14*16;
        }
        if (pac.colisionEnemigo(f4.gpos_x+4, f4.gpos_y+4, f4.GHOST->w-4, f4.GHOST->h-4,pac.isSuper)>= 1){
            f4.gpos_x = 18*16; f4.gpos_y = 14*16;
        }

        drawInfo(pac);
        updateScreen();
        pac.clearPacman();

        //clear(BUFFER);
        //clear(pac.PACMAN);

        drawBoard(score,pac);
        pac.eatPacman();

        //cout << "isScared = " <<f1.isScared << endl;

        if (f1.isScared and ((float)tiempo/CLOCKS_PER_SEC <= tiempoSuper)){
            f1.drawGhostScared();
            f2.drawGhostScared();
            f3.drawGhostScared();
            f4.drawGhostScared();
        }
        else{
            pac.isSuper = false;
            f1.drawGhost();
            f2.drawGhost();
            f3.drawGhost();
            f4.drawGhost();
        }

        updateScreen();
        clear(BUFFER);
        clear(pac.PACMAN);

        rest(20);
    }
    tiempo = clock() - tiempo;

    return 0;
}


int track_pos(int x, int y){
    if(x==1){
        if(y==1) return 1;
        else if(y==2) return 2;
        else if(y==3) return 3;
        else if(y==4) return 4;
        else if(y==5) return 5;
        else if(y==6) return 6;
        else if(y==7) return 7;
        else if(y==8) return 8;
        else if(y==9) return 9;
        else if(y==10) return 10;
        else if(y==11) return 11;
        else if(y==12) return 12;
        else if(y==15) return 13;
        else if(y==16) return 14;
        else if(y==17) return 15;
        else if(y==18) return 16;
        else if(y==19) return 17;
        else if(y==20) return 18;
        else if(y==21) return 19;
        else if(y==22) return 20;
        else if(y==23) return 21;
        else if(y==24) return 22;
        else if(y==25) return 23;
        else if(y==26) return 24;
    }
    else if(x==2){
        if(y==1) return 25;
        else if(y==6) return 26;
        else if(y==12) return 27;
        else if(y==15) return 28;
        else if(y==21) return 29;
        else if(y==26) return 30;
    }
    else if(x==3){
        if(y==1) return 31;
        else if(y==6) return 32;
        else if(y==12) return 33;
        else if(y==15) return 34;
        else if(y==21) return 35;
        else if(y==26) return 36;
    }
    else if(x==4){
        if(y==1) return 37;
        else if(y==6) return 38;
        else if(y==12) return 39;
        else if(y==15) return 40;
        else if(y==21) return 41;
        else if(y==26) return 42;
    }
    else if(x==5){
        if(y==1) return 43;
        else if(y==2) return 44;
        else if(y==3) return 45;
        else if(y==4) return 46;
        else if(y==5) return 47;
        else if(y==6) return 48;
        else if(y==7) return 49;
        else if(y==8) return 50;
        else if(y==9) return 51;
        else if(y==10) return 52;
        else if(y==11) return 53;
        else if(y==12) return 54;
        else if(y==13) return 55;
        else if(y==14) return 56;
        else if(y==15) return 57;
        else if(y==16) return 58;
        else if(y==17) return 59;
        else if(y==18) return 60;
        else if(y==19) return 61;
        else if(y==20) return 62;
        else if(y==21) return 63;
        else if(y==22) return 64;
        else if(y==23) return 65;
        else if(y==24) return 66;
        else if(y==25) return 67;
        else if(y==26) return 68;
    }
    else if(x==6){
        if(y==1) return 69;
        else if(y==6) return 70;
        else if(y==9) return 71;
        else if(y==18) return 72;
        else if(y==21) return 73;
        else if(y==26) return 74;
    }
    else if(x==7){
        if(y==1) return 75;
        else if(y==6) return 76;
        else if(y==9) return 77;
        else if(y==18) return 78;
        else if(y==21) return 79;
        else if(y==26) return 80;
    }
    else if(x==8){
        if(y==1) return 81;
        else if(y==2) return 82;
        else if(y==3) return 83;
        else if(y==4) return 84;
        else if(y==5) return 85;
        else if(y==6) return 86;
        else if(y==9) return 87;
        else if(y==10) return 88;
        else if(y==11) return 89;
        else if(y==12) return 90;
        else if(y==15) return 91;
        else if(y==16) return 92;
        else if(y==17) return 93;
        else if(y==18) return 94;
        else if(y==21) return 95;
        else if(y==22) return 96;
        else if(y==23) return 97;
        else if(y==24) return 98;
        else if(y==25) return 99;
        else if(y==26) return 100;
    }
    else if(x==9){
        if(y==6) return 101;
        else if(y==12) return 102;
        else if(y==15) return 103;
        else if(y==21) return 104;
    }
    else if(x==10){
        if(y==6) return 105;
        else if(y==12) return 106;
        else if(y==15) return 107;
        else if(y==21) return 108;
    }
    else if(x==11){
        if(y==6) return 109;
        else if(y==9) return 110;
        else if(y==10) return 111;
        else if(y==11) return 112;
        else if(y==12) return 113;
        else if(y==13) return 114;
        else if(y==14) return 115;
        else if(y==15) return 116;
        else if(y==16) return 117;
        else if(y==17) return 118;
        else if(y==18) return 119;
        else if(y==21) return 120;
    }
    else if(x==12){
        if(y==6) return 121;
        else if(y==9) return 122;
        else if(y==18) return 123;
        else if(y==21) return 124;
    }
    else if(x==13){
        if(y==6) return 125;
        else if(y==9) return 126;
        else if(y==18) return 127;
        else if(y==21) return 128;
    }
    else if(x==14){
        if(y==0) return 129;
        else if(y==1) return 130;
        else if(y==2) return 131;
        else if(y==3) return 132;
        else if(y==4) return 133;
        else if(y==5) return 134;
        else if(y==6) return 135;
        else if(y==7) return 136;
        else if(y==8) return 137;
        else if(y==9) return 138;
        else if(y==18) return 139;
        else if(y==19) return 140;
        else if(y==20) return 141;
        else if(y==21) return 142;
        else if(y==22) return 143;
        else if(y==23) return 144;
        else if(y==24) return 145;
        else if(y==25) return 146;
        else if(y==26) return 147;
        else if(y==27) return 148;
    }
    else if(x==15){
        if(y==6) return 149;
        else if(y==9) return 150;
        else if(y==18) return 151;
        else if(y==21) return 152;
    }
    else if(x==16){
        if(y==6) return 153;
        else if(y==9) return 154;
        else if(y==18) return 155;
        else if(y==21) return 156;
    }
    else if(x==17){
        if(y==6) return 157;
        else if(y==9) return 158;
        else if(y==10) return 159;
        else if(y==11) return 160;
        else if(y==12) return 161;
        else if(y==13) return 162;
        else if(y==14) return 163;
        else if(y==15) return 164;
        else if(y==16) return 165;
        else if(y==17) return 166;
        else if(y==18) return 167;
        else if(y==21) return 168;
    }
    else if(x==18){
        if(y==6) return 169;
        else if(y==9) return 170;
        else if(y==18) return 171;
        else if(y==21) return 172;
    }
    else if(x==19){
        if(y==6) return 173;
        else if(y==9) return 174;
        else if(y==18) return 175;
        else if(y==21) return 176;    
    }
    else if(x==20){
        if(y==1) return 177;
        else if(y==2) return 178;
        else if(y==3) return 179;
        else if(y==4) return 180;
        else if(y==5) return 181;
        else if(y==6) return 182;
        else if(y==7) return 183;
        else if(y==8) return 184;
        else if(y==9) return 185;
        else if(y==10) return 186;
        else if(y==11) return 187;
        else if(y==12) return 188;
        else if(y==15) return 189;
        else if(y==16) return 190;
        else if(y==17) return 191;
        else if(y==18) return 192;
        else if(y==19) return 193;
        else if(y==20) return 194;
        else if(y==21) return 195;
        else if(y==22) return 196;
        else if(y==23) return 197;
        else if(y==24) return 198;
        else if(y==25) return 199;
        else if(y==26) return 200;
    }
    else if(x==21){
        if(y==1) return 201;
        else if(y==6) return 202;
        else if(y==12) return 203;
        else if(y==15) return 204;
        else if(y==21) return 205;
        else if(y==26) return 206;
    }
    else if(x==22){
        if(y==1) return 207;
        else if(y==6) return 208;
        else if(y==12) return 209;
        else if(y==15) return 210;
        else if(y==21) return 211;
        else if(y==26) return 212;
    }
    else if(x==23){
        if(y==1) return 213;
        else if(y==2) return 214;
        else if(y==3) return 215;
        else if(y==6) return 216;
        else if(y==7) return 217;
        else if(y==8) return 218;
        else if(y==9) return 219;
        else if(y==10) return 220;
        else if(y==11) return 221;
        else if(y==12) return 222;
        else if(y==13) return 223;
        else if(y==14) return 224;
        else if(y==15) return 225;
        else if(y==16) return 226;
        else if(y==17) return 227;
        else if(y==18) return 228;
        else if(y==19) return 229;
        else if(y==20) return 230;
        else if(y==21)  return 231;
        else if(y==24) return 232;
        else if(y==25) return 233;
        else if(y==26) return 234;
    }
    else if(x==24){
        if(y==3) return 235;
        else if(y==6) return 236;
        else if(y==9) return 237;
        else if(y==18) return 238;
        else if(y==21) return 239;
        else if(y==24) return 240;
    }
    else if(x==25){
        if(y==3) return 241;
        else if(y==6) return 242;
        else if(y==9) return 243;
        else if(y==18) return 244;
        else if(y==21) return 245;
        else if(y==24) return 246;
    }
    else if(x==26){
        if(y==1) return 247;
        else if(y==2) return 248;
        else if(y==3) return 249;
        else if(y==4) return 250;
        else if(y==5) return 251;
        else if(y==6) return 252;
        else if(y==9) return 253;
        else if(y==10) return 254;
        else if(y==11) return 255;
        else if(y==12)  return 256;
        else if(y==15) return 257;
        else if(y==16) return 258;
        else if(y==17) return 259;
        else if(y==18) return 260;
        else if(y==21) return 261;
        else if(y==22) return 262;
        else if(y==23) return 263;
        else if(y==24) return 264;
        else if(y==25) return 265;
        else if(y==26) return 266;
    }
    else if(x==27){
        if(y==1) return 267;
        else if(y==12) return 268;
        else if(y==15) return 269;
        else if(y==26) return 270;
    }
    else if(x==28){
        if(y==1) return 271;
        else if(y==12) return 272;
        else if(y==15) return 273;
        else if(y==26) return 274;
    }
    else if(x==29){
        if(y==1) return 275;
        else if(y==2) return 276;
        else if(y==3) return 277;
        else if(y==4) return 278;
        else if(y==5) return 279;
        else if(y==6) return 280;
        else if(y==7) return 281;
        else if(y==8) return 282;
        else if(y==9) return 283;
        else if(y==10) return 284;
        else if(y==11) return 285;
        else if(y==12) return 286;
        else if(y==13) return 287;
        else if(y==14) return 288;
        else if(y==15) return 289;
        else if(y==16) return 290;
        else if(y==17) return 291;
        else if(y==18) return 292;
        else if(y==19) return 293;
        else if(y==20) return 294;
        else if(y==21) return 295;
        else if(y==22) return 296;
        else if(y==23) return 297;
        else if(y==24) return 298;
        else if(y==25) return 299;
        else if(y==26) return 300; 
    }
}

Point track_inverse(int n){
     int x, y;

    if(n==1){ x=1; y=1; }
    else if(n==2){ x=1; y=2; }
    else if(n==3){ x=1; y=2; }
    else if(n==4){ x=1; y=4; }
    else if(n==5){ x=1; y=5; }
    else if(n==6){ x=1; y=6; }
    else if(n==7){ x=1; y=7; }
    else if(n==8){ x=1; y=8; }
    else if(n==9){ x=1; y=9; }
    else if(n==10){ x=1; y=10; }
    else if(n==11){ x=1; y=11; }
    else if(n==12){ x=1; y=12; }
    else if(n==13){ x=1; y=15; }
    else if(n==14){ x=1; y=16; }
    else if(n==15){ x=1; y=17; }
    else if(n==16){ x=1; y=18; }
    else if(n==17){ x=1; y=19; }
    else if(n==18){ x=1; y=20; }
    else if(n==19){ x=1; y=21; }
    else if(n==20){ x=1; y=22; }
    else if(n==21){ x=1; y=23; }
    else if(n==22){ x=1; y=24; }
    else if(n==23){ x=1; y=25; }
    else if(n==24){ x=1; y=26; }
    else if(n==25){ x=2; y=1; }
    else if(n==26){ x=2; y=6; }
    else if(n==27){ x=2; y=12; }
    else if(n==28){ x=2; y=15; }
    else if(n==29){ x=2; y=21; }
    else if(n==30){ x=2; y=26; }
    else if(n==31){ x=3; y=1; }
    else if(n==32){ x=3; y=6; }
    else if(n==33){ x=3; y=12; }
    else if(n==34){ x=3; y=15; }
    else if(n==35){ x=3; y=21; }
    else if(n==36){ x=3; y=26; }
    else if(n==37){ x=4; y=1; }
    else if(n==38){ x=4; y=6; }
    else if(n==39){ x=4; y=12; }
    else if(n==40){ x=4; y=15; }
    else if(n==41){ x=4; y=21; }
    else if(n==42){ x=4; y=26; }
    else if(n==43){ x=5; y=1; }
    else if(n==44){ x=5; y=2; }
    else if(n==45){ x=5; y=3; }
    else if(n==46){ x=5; y=4; }
    else if(n==47){ x=5; y=5; }
    else if(n==48){ x=5; y=6; }
    else if(n==49){ x=5; y=7; }
    else if(n==50){ x=5; y=8; }
    else if(n==51){ x=5; y=9; }
    else if(n==52){ x=5; y=10; }
    else if(n==53){ x=5; y=11; }
    else if(n==54){ x=5; y=12; }
    else if(n==55){ x=5; y=13; }
    else if(n==56){ x=5; y=14; }
    else if(n==57){ x=5; y=15; }
    else if(n==58){ x=5; y=16; }
    else if(n==59){ x=5; y=17; }
    else if(n==60){ x=5; y=18; }
    else if(n==61){ x=5; y=19; }
    else if(n==62){ x=5; y=20; }
    else if(n==63){ x=5; y=21; }
    else if(n==64){ x=5; y=22; }
    else if(n==65){ x=5; y=23; }
    else if(n==66){ x=5; y=24; }
    else if(n==67){ x=5; y=25; }
    else if(n==68){ x=5; y=26; }
    else if(n==69){ x=6; y=2; }
    else if(n==70){ x=6; y=6; }
    else if(n==71){ x=6; y=9; }
    else if(n==72){ x=6; y=18; }
    else if(n==73){ x=6; y=21; }
    else if(n==74){ x=6; y=26; }
    else if(n==75){ x=7; y=2; }
    else if(n==76){ x=7; y=6; }
    else if(n==77){ x=7; y=9; }
    else if(n==78){ x=7; y=18; }
    else if(n==79){ x=7; y=21; }
    else if(n==80){ x=7; y=26; }
    else if(n==81){ x=8; y=1; }
    else if(n==82){ x=8; y=2; }
    else if(n==83){ x=8; y=3; }
    else if(n==84){ x=8; y=4; }
    else if(n==85){ x=8; y=5; }
    else if(n==86){ x=8; y=6; }
    else if(n==87){ x=8; y=9; }
    else if(n==88){ x=8; y=10; }
    else if(n==89){ x=8; y=11; }
    else if(n==90){ x=8; y=12; }
    else if(n==91){ x=8; y=15; }
    else if(n==92){ x=8; y=16; }
    else if(n==93){ x=8; y=17; }
    else if(n==94){ x=8; y=18; }
    else if(n==95){ x=8; y=21; }
    else if(n==96){ x=8; y=22; }
    else if(n==97){ x=8; y=23; }
    else if(n==98){ x=8; y=24; }
    else if(n==99){ x=8; y=25; }
    else if(n==100){ x=8; y=26; }
    else if(n==101){ x=9; y=6; }
    else if(n==102){ x=9; y=12; }
    else if(n==103){ x=9; y=15; }
    else if(n==104){ x=9; y=21; }
    else if(n==105){ x=10; y=6; }
    else if(n==106){ x=10; y=12; }
    else if(n==107){ x=10; y=15; }
    else if(n==108){ x=10; y=21; }
    else if(n==109){ x=11; y=6; }
    else if(n==110){ x=11; y=9; }
    else if(n==111){ x=11; y=10; }
    else if(n==112){ x=11; y=11; }
    else if(n==113){ x=11; y=12; }
    else if(n==114){ x=11; y=13; }
    else if(n==115){ x=11; y=14; }
    else if(n==116){ x=11; y=15; }
    else if(n==117){ x=11; y=16; }
    else if(n==118){ x=11; y=17; }
    else if(n==119){ x=11; y=18; }
    else if(n==120){ x=11; y=21; }
    else if(n==121){ x=12; y=6; }
    else if(n==122){ x=12; y=9; }
    else if(n==123){ x=12; y=18; }
    else if(n==124){ x=12; y=21; }
    else if(n==125){ x=13; y=6; }
    else if(n==126){ x=13; y=9; }
    else if(n==127){ x=13; y=18; }
    else if(n==128){ x=13; y=21; }
    else if(n==129){ x=14; y=0; }
    else if(n==130){ x=14; y=1; }
    else if(n==131){ x=14; y=2; }
    else if(n==132){ x=14; y=3; }
    else if(n==133){ x=14; y=4; }
    else if(n==134){ x=14; y=5; }
    else if(n==135){ x=14; y=6; }
    else if(n==136){ x=14; y=7; }
    else if(n==137){ x=14; y=8; }
    else if(n==138){ x=14; y=9; }
    else if(n==139){ x=14; y=18; }
    else if(n==140){ x=14; y=19; }
    else if(n==141){ x=14; y=20; }
    else if(n==142){ x=14; y=21; }
    else if(n==143){ x=14; y=22; }
    else if(n==144){ x=14; y=23; }
    else if(n==145){ x=14; y=24; }
    else if(n==146){ x=14; y=25; }
    else if(n==147){ x=14; y=26; }
    else if(n==148){ x=14; y=27; }
    else if(n==149){ x=15; y=6; }
    else if(n==150){ x=15; y=9; }
    else if(n==151){ x=15; y=18; }
    else if(n==152){ x=15; y=21; }
    else if(n==153){ x=16; y=6; }
    else if(n==154){ x=16; y=9; }
    else if(n==155){ x=16; y=18; }
    else if(n==156){ x=16; y=21; }
    else if(n==157){ x=17; y=6; }
    else if(n==158){ x=17; y=9; }
    else if(n==159){ x=17; y=10; }
    else if(n==160){ x=17; y=11; }
    else if(n==161){ x=17; y=12; }
    else if(n==162){ x=17; y=13; }
    else if(n==163){ x=17; y=14; }
    else if(n==164){ x=17; y=15; }
    else if(n==165){ x=17; y=16; }
    else if(n==166){ x=17; y=17; }
    else if(n==167){ x=17; y=18; }
    else if(n==168){ x=17; y=21; }
    else if(n==169){ x=18; y=6; }
    else if(n==170){ x=18; y=9; }
    else if(n==171){ x=18; y=18; }
    else if(n==172){ x=18; y=21; }
    else if(n==173){ x=19; y=6; }
    else if(n==174){ x=19; y=9; }
    else if(n==175){ x=19; y=18; }
    else if(n==176){ x=19; y=21; }
    else if(n==177){ x=20; y=1; }
    else if(n==178){ x=20; y=2; }
    else if(n==179){ x=20; y=3; }
    else if(n==180){ x=20; y=4; }
    else if(n==181){ x=20; y=5; }
    else if(n==182){ x=20; y=6; }
    else if(n==183){ x=20; y=7; }
    else if(n==184){ x=20; y=8; }
    else if(n==185){ x=20; y=9; }
    else if(n==186){ x=20; y=11; }
    else if(n==187){ x=20; y=11; }
    else if(n==188){ x=20; y=12;}
    else if(n==189){ x=20; y=15;}
    else if(n==190){ x=20; y=16;}
    else if(n==191){ x=20; y=17;}
    else if(n==192){ x=20; y=18;}
    else if(n==193){ x=20; y=19;}
    else if(n==194){ x=20; y=20;}
    else if(n==195){ x=20; y=21;}
    else if(n==196){ x=20; y=22;}
    else if(n==197){ x=20; y=23;}
    else if(n==198){ x=20; y=24; }
    else if(n==199){ x=20; y=25; }
    else if(n==200){ x=20; y=26; }
    else if(n==201){ x=21; y=1; }
    else if(n==202){ x=21; y=6; }
    else if(n==203){ x=21; y=12; }
    else if(n==204){ x=21; y=15; }
    else if(n==205){ x=21; y=21; }
    else if(n==206){ x=21; y=26; }
    else if(n==207){ x=22; y=1; }
    else if(n==208){ x=22; y=6; }
    else if(n==209){ x=22; y=12; }
    else if(n==210){ x=22; y=15; }
    else if(n==211){ x=22; y=21; }
    else if(n==212){ x=22; y=26; }
    else if(n==213){ x=23; y=1; }
    else if(n==214){ x=23; y=2; }
    else if(n==215){ x=23; y=3; }
    else if(n==216){ x=23; y=6; }
    else if(n==217){ x=23; y=7; }
    else if(n==218){ x=23; y=8; }
    else if(n==219){ x=23; y=9; }
    else if(n==220){ x=23; y=10; }
    else if(n==221){ x=23; y=11; }
    else if(n==222){ x=23; y=12; }
    else if(n==223){ x=23; y=13; }
    else if(n==224){ x=23; y=14; }
    else if(n==225){ x=23; y=15; }
    else if(n==226){ x=23; y=16; }
    else if(n==227){ x=23; y=17; }
    else if(n==228){ x=23; y=18; }
    else if(n==229){ x=23; y=19; }
    else if(n==230){ x=23; y=20; }
    else if(n==231){ x=23; y=21; }
    else if(n==232){ x=23; y=24; }
    else if(n==233){ x=23; y=25; }
    else if(n==234){ x=23; y=26; }
    else if(n==235){ x=24; y=3; }
    else if(n==236){ x=24; y=6; }
    else if(n==237){ x=24; y=9; }
    else if(n==238){ x=24; y=18; }
    else if(n==239){ x=24; y=21; }
    else if(n==240){ x=24; y=24; }
    else if(n==241){ x=25; y=3; }
    else if(n==242){ x=25; y=6; }
    else if(n==243){ x=25; y=9; }
    else if(n==244){ x=25; y=18; }
    else if(n==245){ x=25; y=21; }
    else if(n==246){ x=25; y=24; }
    else if(n==247){ x=26; y=1; }
    else if(n==248){ x=26; y=2; }
    else if(n==249){ x=26; y=3; }
    else if(n==250){ x=26; y=4; }
    else if(n==251){ x=26; y=5; }
    else if(n==252){ x=26; y=6; }
    else if(n==253){ x=26; y=9; }
    else if(n==254){ x=26; y=10;} 
    else if(n==255){ x=26; y=11; }
    else if(n==256){ x=26; y=12; }
    else if(n==257){ x=26; y=15; }
    else if(n==258){ x=26; y=16; }
    else if(n==259){ x=26; y=17; }
    else if(n==260){ x=26; y=18; }
    else if(n==261){ x=26; y=21; }
    else if(n==262){ x=26; y=22; }
    else if(n==263){ x=26; y=23; }
    else if(n==264){ x=26; y=24; }
    else if(n==265){ x=26; y=25; }
    else if(n==266){ x=26; y=26; }
    else if(n==267){ x=27; y=1; }
    else if(n==268){ x=27; y=12; }
    else if(n==269){ x=27; y=15; }
    else if(n==270){ x=27; y=26; }
    else if(n==271){ x=28; y=1; }
    else if(n==272){ x=28; y=12; }
    else if(n==273){ x=28; y=15; }
    else if(n==274){ x=28; y=26; }
    else if(n==275){ x=29; y=1; }
    else if(n==276){ x=29; y=2; }
    else if(n==277){ x=29; y=3; }
    else if(n==278){ x=29; y=4; }
    else if(n==279){ x=29; y=5; }
    else if(n==280){ x=29; y=6; }
    else if(n==281){ x=29; y=7; }
    else if(n==282){ x=29; y=8; }
    else if(n==283){ x=29; y=9; }
    else if(n==284){ x=29; y=10; }
    else if(n==285){ x=29; y=11; }
    else if(n==286){ x=29; y=12; }
    else if(n==287){ x=29; y=13; }
    else if(n==288){ x=29; y=14; }
    else if(n==289){ x=29; y=15; }
    else if(n==290){ x=29; y=16; }
    else if(n==291){ x=29; y=17; }
    else if(n==292){ x=29; y=18; }
    else if(n==293){ x=29; y=19; }
    else if(n==294){ x=29; y=20; }
    else if(n==295){ x=29; y=21; }
    else if(n==296){ x=29; y=22; }
    else if(n==297){ x=29; y=23; }
    else if(n==298){ x=29; y=24; }
    else if(n==299){ x=29; y=25; }
    else if(n==300){ x=29; y=26; }

    Point p;
    p.x = x;
    p.y = y;

    return p;
}

END_OF_MAIN();
