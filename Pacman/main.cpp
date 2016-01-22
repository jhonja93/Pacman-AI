#include <allegro.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#define ROWS  31     // y
#define COLS  29 //x
#define WIDTH_SCREEN  640
#define HEIGHT_SCREEN 500

BITMAP *BUFFER;
BITMAP *WALL_STRAIGHT, *WALL_HORIZONTAL, *WALL_CORNER_UR, *WALL_CORNER_UL, *WALL_CORNER_LL, *WALL_CORNER_LR, *GHOST_DOOR;
BITMAP *FOOD, *POWER_FOOD, *FRUIT;
BITMAP *MENU_1,*MENU_2,*MOUSE;

SAMPLE *EAT, *WALK,*DEAD,*INTRO, *EAT_FRUIT, *GAME_OVER;
//int posx,posy;
int sound, score;
clock_t tiempo;
float tiempoSuper;

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

int num_tiles(char id)
{
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
    pos_x = x;
    pos_y = y;
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
            if (isSuper) pos_x -= 4;
            else pos_x -= 2;
        else if (pos_x < 0) pos_x = 27*16;
        else dir = 4;
    }
    if (dir == 1) {
        pos_actual = table[obty(pos_y, 16)][obtx(pos_x+16, 16)];
        if (pos_actual == 'o' or pos_actual == 'O' or pos_actual == ' '  or pos_actual == 'F' or pos_actual == 'x' )
            if (isSuper) pos_x += 4;
            else pos_x += 2;
        else if (pos_x >= 27*16) pos_x = 0;
        else dir = 4;
    }
    if (dir == 2) {
        pos_actual = table[obty(pos_y-1, 16)][obtx(pos_x, 16)];
        if (pos_actual == 'o' or pos_actual == 'O' or pos_actual == ' ' or pos_actual == 'F' or pos_actual == 'x' )
            if (isSuper) pos_y -= 4;
            else pos_y -= 2;
        else dir = 4;
    }
    if (dir == 3) {
        pos_actual = table[obty(pos_y+16, 16)][obtx(pos_x, 16)];
        if (pos_actual == 'o' or pos_actual == 'O' or pos_actual == ' ' or pos_actual == 'F' or pos_actual == 'x' )
            if (isSuper) pos_y += 4;
            else pos_y += 2;
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
    pos_x = 14*16;
    pos_y = 23*16;
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
    pos_x = x;
}

void Pacman::setPosYPacman(int y){
    pos_y = y;
}

void Pacman::setDirectionPacman(int direc){
    dir = direc;
}

int initialize()
{
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
        f1.moveGhost();
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
END_OF_MAIN();
