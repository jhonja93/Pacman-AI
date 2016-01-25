#include "mapeartabla.h"




int track_pos(int x, int y){
    y = y+1;
    if(x==1){
        if(y==2) return 1;
        else if(y==3) return 2;
        else if(y==4) return 3;
        else if(y==5) return 4;
        else if(y==6) return 5;
        else if(y==7) return 6;
        else if(y==8) return 7;
        else if(y==9) return 8;
        else if(y==10) return 9;
        else if(y==11) return 10;
        else if(y==12) return 11;
        else if(y==13) return 12;
        else if(y==16) return 13;
        else if(y==17) return 14;
        else if(y==18) return 15;
        else if(y==19) return 16;
        else if(y==20) return 17;
        else if(y==21) return 18;
        else if(y==22) return 19;
        else if(y==23) return 20;
        else if(y==24) return 21;
        else if(y==25) return 22;
        else if(y==26) return 23;
        else if(y==27) return 24;
    }
    else if(x==2){
        if(y==2) return 25;
        else if(y==7) return 26;
        else if(y==13) return 27;
        else if(y==16) return 28;
        else if(y==22) return 29;
        else if(y==27) return 30;
    }
    else if(x==3){
        if(y==2) return 31;
        else if(y==7) return 32;
        else if(y==13) return 33;
        else if(y==16) return 34;
        else if(y==22) return 35;
        else if(y==27) return 36;
    }
    else if(x==4){
        if(y==2) return 37;
        else if(y==7) return 38;
        else if(y==13) return 39;
        else if(y==16) return 40;
        else if(y==22) return 41;
        else if(y==27) return 42;
    }
    else if(x==5){
        if(y==2) return 43;
        else if(y==3) return 44;
        else if(y==4) return 45;
        else if(y==5) return 46;
        else if(y==6) return 47;
        else if(y==7) return 48;
        else if(y==8) return 49;
        else if(y==9) return 50;
        else if(y==10) return 51;
        else if(y==11) return 52;
        else if(y==12) return 53;
        else if(y==13) return 54;
        else if(y==14) return 55;
        else if(y==15) return 56;
        else if(y==16) return 57;
        else if(y==17) return 58;
        else if(y==18) return 59;
        else if(y==19) return 60;
        else if(y==20) return 61;
        else if(y==21) return 62;
        else if(y==22) return 63;
        else if(y==23) return 64;
        else if(y==24) return 65;
        else if(y==25) return 66;
        else if(y==26) return 67;
        else if(y==27) return 68;
    }
    else if(x==6){
        if(y==2) return 69;
        else if(y==7) return 70;
        else if(y==10) return 71;
        else if(y==19) return 72;
        else if(y==22) return 73;
        else if(y==27) return 74;
    }
    else if(x==7){
        if(y==2) return 75;
        else if(y==7) return 76;
        else if(y==10) return 77;
        else if(y==19) return 78;
        else if(y==22) return 79;
        else if(y==27) return 80;
    }
    else if(x==8){
        if(y==2) return 81;
        else if(y==3) return 82;
        else if(y==4) return 83;
        else if(y==5) return 84;
        else if(y==6) return 85;
        else if(y==7) return 86;
        else if(y==10) return 87;
        else if(y==11) return 88;
        else if(y==12) return 89;
        else if(y==13) return 90;
        else if(y==16) return 91;
        else if(y==17) return 92;
        else if(y==18) return 93;
        else if(y==19) return 94;
        else if(y==22) return 95;
        else if(y==23) return 96;
        else if(y==24) return 97;
        else if(y==25) return 98;
        else if(y==26) return 99;
        else if(y==27) return 100;
    }
    else if(x==9){
        if(y==7) return 101;
        else if(y==13) return 102;
        else if(y==16) return 103;
        else if(y==22) return 104;
    }
    else if(x==10){
        if(y==7) return 105;
        else if(y==13) return 106;
        else if(y==16) return 107;
        else if(y==22) return 108;
    }
    else if(x==11){
        if(y==7) return 109;
        else if(y==10) return 110;
        else if(y==11) return 111;
        else if(y==12) return 112;
        else if(y==13) return 113;
        else if(y==14) return 114;
        else if(y==15) return 115;
        else if(y==16) return 116;
        else if(y==17) return 117;
        else if(y==18) return 118;
        else if(y==19) return 119;
        else if(y==22) return 120;
    }
    else if(x==12){
        if(y==7) return 121;
        else if(y==10) return 122;
        else if(y==19) return 123;
        else if(y==22) return 124;
    }
    else if(x==13){
        if(y==7) return 125;
        else if(y==10) return 126;
        else if(y==19) return 127;
        else if(y==22) return 128;
    }
    else if(x==14){
        if(y==1) return 129;
        else if(y==2) return 130;
        else if(y==3) return 131;
        else if(y==4) return 132;
        else if(y==5) return 133;
        else if(y==6) return 134;
        else if(y==7) return 135;
        else if(y==8) return 136;
        else if(y==9) return 137;
        else if(y==10) return 138;
        else if(y==19) return 139;
        else if(y==20) return 140;
        else if(y==21) return 141;
        else if(y==22) return 142;
        else if(y==23) return 143;
        else if(y==24) return 144;
        else if(y==25) return 145;
        else if(y==26) return 146;
        else if(y==27) return 147;
        else if(y==28) return 148;
    }
    else if(x==15){
        if(y==7) return 149;
        else if(y==10) return 150;
        else if(y==19) return 151;
        else if(y==22) return 152;
    }
    else if(x==16){
        if(y==7) return 153;
        else if(y==10) return 154;
        else if(y==19) return 155;
        else if(y==22) return 156;
    }
    else if(x==17){
        if(y==7) return 157;
        else if(y==10) return 158;
        else if(y==11) return 159;
        else if(y==12) return 160;
        else if(y==13) return 161;
        else if(y==14) return 162;
        else if(y==15) return 163;
        else if(y==16) return 164;
        else if(y==17) return 165;
        else if(y==18) return 166;
        else if(y==19) return 167;
        else if(y==22) return 168;
    }
    else if(x==18){
        if(y==7) return 169;
        else if(y==10) return 170;
        else if(y==19) return 171;
        else if(y==22) return 172;
    }
    else if(x==19){
        if(y==7) return 173;
        else if(y==10) return 174;
        else if(y==19) return 175;
        else if(y==22) return 176;    
    }
    else if(x==20){
        if(y==2) return 177;
        else if(y==3) return 178;
        else if(y==4) return 179;
        else if(y==5) return 180;
        else if(y==6) return 181;
        else if(y==7) return 182;
        else if(y==8) return 183;
        else if(y==9) return 184;
        else if(y==10) return 185;
        else if(y==11) return 186;
        else if(y==12) return 187;
        else if(y==13) return 188;
        else if(y==16) return 189;
        else if(y==17) return 190;
        else if(y==18) return 191;
        else if(y==19) return 192;
        else if(y==20) return 193;
        else if(y==21) return 194;
        else if(y==22) return 195;
        else if(y==23) return 196;
        else if(y==24) return 197;
        else if(y==25) return 198;
        else if(y==26) return 199;
        else if(y==27) return 200;
    }
    else if(x==21){
        if(y==2) return 201;
        else if(y==7) return 202;
        else if(y==13) return 203;
        else if(y==16) return 204;
        else if(y==22) return 205;
        else if(y==27) return 206;
    }
    else if(x==22){
        if(y==2) return 207;
        else if(y==7) return 208;
        else if(y==13) return 209;
        else if(y==16) return 210;
        else if(y==22) return 211;
        else if(y==27) return 212;
    }
    else if(x==23){
        if(y==2) return 213;
        else if(y==3) return 214;
        else if(y==4) return 215;
        else if(y==7) return 216;
        else if(y==8) return 217;
        else if(y==9) return 218;
        else if(y==10) return 219;
        else if(y==11) return 220;
        else if(y==12) return 221;
        else if(y==13) return 222;
        else if(y==14) return 223;
        else if(y==15) return 224;
        else if(y==16) return 225;
        else if(y==17) return 226;
        else if(y==18) return 227;
        else if(y==19) return 228;
        else if(y==20) return 229;
        else if(y==21) return 230;
        else if(y==22)  return 231;
        else if(y==25) return 232;
        else if(y==26) return 233;
        else if(y==27) return 234;
    }
    else if(x==24){
        if(y==4) return 235;
        else if(y==7) return 236;
        else if(y==10) return 237;
        else if(y==19) return 238;
        else if(y==22) return 239;
        else if(y==25) return 240;
    }
    else if(x==25){
        if(y==4) return 241;
        else if(y==7) return 242;
        else if(y==10) return 243;
        else if(y==19) return 244;
        else if(y==22) return 245;
        else if(y==25) return 246;
    }
    else if(x==26){
        if(y==2) return 247;
        else if(y==3) return 248;
        else if(y==4) return 249;
        else if(y==5) return 250;
        else if(y==6) return 251;
        else if(y==7) return 252;
        else if(y==10) return 253;
        else if(y==11) return 254;
        else if(y==12) return 255;
        else if(y==13)  return 256;
        else if(y==16) return 257;
        else if(y==17) return 258;
        else if(y==18) return 259;
        else if(y==19) return 260;
        else if(y==22) return 261;
        else if(y==23) return 262;
        else if(y==24) return 263;
        else if(y==25) return 264;
        else if(y==26) return 265;
        else if(y==27) return 266;
    }
    else if(x==27){
        if(y==2) return 267;
        else if(y==13) return 268;
        else if(y==16) return 269;
        else if(y==27) return 270;
    }
    else if(x==28){
        if(y==2) return 271;
        else if(y==13) return 272;
        else if(y==16) return 273;
        else if(y==27) return 274;
    }
    else if(x==29){
        if(y==2) return 275;
        else if(y==3) return 276;
        else if(y==4) return 277;
        else if(y==5) return 278;
        else if(y==6) return 279;
        else if(y==7) return 280;
        else if(y==8) return 281;
        else if(y==9) return 282;
        else if(y==10) return 283;
        else if(y==11) return 284;
        else if(y==12) return 285;
        else if(y==13) return 286;
        else if(y==14) return 287;
        else if(y==15) return 288;
        else if(y==16) return 289;
        else if(y==17) return 290;
        else if(y==18) return 291;
        else if(y==19) return 292;
        else if(y==20) return 293;
        else if(y==21) return 294;
        else if(y==22) return 295;
        else if(y==23) return 296;
        else if(y==24) return 297;
        else if(y==25) return 298;
        else if(y==26) return 299;
        else if(y==27) return 300; 
    }
}

Point track_inverse(int n){
     int x, y;

    if(n==1){ x=1; y=2; }
    else if(n==2){ x=1; y=3; }
    else if(n==3){ x=1; y=4; }
    else if(n==4){ x=1; y=5; }
    else if(n==5){ x=1; y=6; }
    else if(n==6){ x=1; y=7; }
    else if(n==7){ x=1; y=8; }
    else if(n==8){ x=1; y=9; }
    else if(n==9){ x=1; y=10; }
    else if(n==10){ x=1; y=11; }
    else if(n==11){ x=1; y=12; }
    else if(n==12){ x=1; y=13; }
    else if(n==13){ x=1; y=16; }
    else if(n==14){ x=1; y=17; }
    else if(n==15){ x=1; y=18; }
    else if(n==16){ x=1; y=19; }
    else if(n==17){ x=1; y=20; }
    else if(n==18){ x=1; y=21; }
    else if(n==19){ x=1; y=22; }
    else if(n==20){ x=1; y=23; }
    else if(n==21){ x=1; y=24; }
    else if(n==22){ x=1; y=25; }
    else if(n==23){ x=1; y=26; }
    else if(n==24){ x=1; y=27; }
    else if(n==25){ x=2; y=2; }
    else if(n==26){ x=2; y=7; }
    else if(n==27){ x=2; y=13; }
    else if(n==28){ x=2; y=16; }
    else if(n==29){ x=2; y=22; }
    else if(n==30){ x=2; y=27; }
    else if(n==31){ x=3; y=2; }
    else if(n==32){ x=3; y=7; }
    else if(n==33){ x=3; y=13; }
    else if(n==34){ x=3; y=16; }
    else if(n==35){ x=3; y=22; }
    else if(n==36){ x=3; y=27; }
    else if(n==37){ x=4; y=2; }
    else if(n==38){ x=4; y=7; }
    else if(n==39){ x=4; y=13; }
    else if(n==40){ x=4; y=16; }
    else if(n==41){ x=4; y=22; }
    else if(n==42){ x=4; y=27; }
    else if(n==43){ x=5; y=2; }
    else if(n==44){ x=5; y=3; }
    else if(n==45){ x=5; y=4; }
    else if(n==46){ x=5; y=5; }
    else if(n==47){ x=5; y=6; }
    else if(n==48){ x=5; y=7; }
    else if(n==49){ x=5; y=8; }
    else if(n==50){ x=5; y=9; }
    else if(n==51){ x=5; y=10; }
    else if(n==52){ x=5; y=11; }
    else if(n==53){ x=5; y=12; }
    else if(n==54){ x=5; y=13; }
    else if(n==55){ x=5; y=14; }
    else if(n==56){ x=5; y=15; }
    else if(n==57){ x=5; y=16; }
    else if(n==58){ x=5; y=17; }
    else if(n==59){ x=5; y=18; }
    else if(n==60){ x=5; y=19; }
    else if(n==61){ x=5; y=20; }
    else if(n==62){ x=5; y=21; }
    else if(n==63){ x=5; y=22; }
    else if(n==64){ x=5; y=23; }
    else if(n==65){ x=5; y=24; }
    else if(n==66){ x=5; y=25; }
    else if(n==67){ x=5; y=26; }
    else if(n==68){ x=5; y=27; }
    else if(n==69){ x=6; y=2; }
    else if(n==70){ x=6; y=7; }
    else if(n==71){ x=6; y=10; }
    else if(n==72){ x=6; y=19; }
    else if(n==73){ x=6; y=22; }
    else if(n==74){ x=6; y=27; }
    else if(n==75){ x=7; y=2; }
    else if(n==76){ x=7; y=7; }
    else if(n==77){ x=7; y=10; }
    else if(n==78){ x=7; y=19; }
    else if(n==79){ x=7; y=22; }
    else if(n==80){ x=7; y=27; }
    else if(n==81){ x=8; y=2; }
    else if(n==82){ x=8; y=3; }
    else if(n==83){ x=8; y=4; }
    else if(n==84){ x=8; y=5; }
    else if(n==85){ x=8; y=6; }
    else if(n==86){ x=8; y=7; }
    else if(n==87){ x=8; y=10; }
    else if(n==88){ x=8; y=11; }
    else if(n==89){ x=8; y=12; }
    else if(n==90){ x=8; y=13; }
    else if(n==91){ x=8; y=16; }
    else if(n==92){ x=8; y=17; }
    else if(n==93){ x=8; y=18; }
    else if(n==94){ x=8; y=19; }
    else if(n==95){ x=8; y=22; }
    else if(n==96){ x=8; y=23; }
    else if(n==97){ x=8; y=24; }
    else if(n==98){ x=8; y=25; }
    else if(n==99){ x=8; y=26; }
    else if(n==100){ x=8; y=27; }
    else if(n==101){ x=9; y=7; }
    else if(n==102){ x=9; y=13; }
    else if(n==103){ x=9; y=16; }
    else if(n==104){ x=9; y=22; }
    else if(n==105){ x=10; y=7; }
    else if(n==106){ x=10; y=13; }
    else if(n==107){ x=10; y=16; }
    else if(n==108){ x=10; y=22; }
    else if(n==109){ x=11; y=7; }
    else if(n==110){ x=11; y=10; }
    else if(n==111){ x=11; y=11; }
    else if(n==112){ x=11; y=12; }
    else if(n==113){ x=11; y=13; }
    else if(n==114){ x=11; y=14; }
    else if(n==115){ x=11; y=15; }
    else if(n==116){ x=11; y=16; }
    else if(n==117){ x=11; y=17; }
    else if(n==118){ x=11; y=18; }
    else if(n==119){ x=11; y=19; }
    else if(n==120){ x=11; y=22; }
    else if(n==121){ x=12; y=7; }
    else if(n==122){ x=12; y=10; }
    else if(n==123){ x=12; y=19; }
    else if(n==124){ x=12; y=22; }
    else if(n==125){ x=13; y=7; }
    else if(n==126){ x=13; y=10; }
    else if(n==127){ x=13; y=19; }
    else if(n==128){ x=13; y=22; }
    else if(n==129){ x=14; y=1; }
    else if(n==130){ x=14; y=2; }
    else if(n==131){ x=14; y=3; }
    else if(n==132){ x=14; y=4; }
    else if(n==133){ x=14; y=5; }
    else if(n==134){ x=14; y=6; }
    else if(n==135){ x=14; y=7; }
    else if(n==136){ x=14; y=8; }
    else if(n==137){ x=14; y=9; }
    else if(n==138){ x=14; y=10; }
    else if(n==139){ x=14; y=19; }
    else if(n==140){ x=14; y=20; }
    else if(n==141){ x=14; y=21; }
    else if(n==142){ x=14; y=22; }
    else if(n==143){ x=14; y=23; }
    else if(n==144){ x=14; y=24; }
    else if(n==145){ x=14; y=25; }
    else if(n==146){ x=14; y=26; }
    else if(n==147){ x=14; y=27; }
    else if(n==148){ x=14; y=28; }
    else if(n==149){ x=15; y=7; }
    else if(n==150){ x=15; y=10; }
    else if(n==151){ x=15; y=19; }
    else if(n==152){ x=15; y=22; }
    else if(n==153){ x=16; y=7; }
    else if(n==154){ x=16; y=10; }
    else if(n==155){ x=16; y=19; }
    else if(n==156){ x=16; y=22; }
    else if(n==157){ x=17; y=7; }
    else if(n==158){ x=17; y=10; }
    else if(n==159){ x=17; y=11; }
    else if(n==160){ x=17; y=12; }
    else if(n==161){ x=17; y=13; }
    else if(n==162){ x=17; y=14; }
    else if(n==163){ x=17; y=15; }
    else if(n==164){ x=17; y=16; }
    else if(n==165){ x=17; y=17; }
    else if(n==166){ x=17; y=18; }
    else if(n==167){ x=17; y=19; }
    else if(n==168){ x=17; y=22; }
    else if(n==169){ x=18; y=7; }
    else if(n==170){ x=18; y=10; }
    else if(n==171){ x=18; y=19; }
    else if(n==172){ x=18; y=22; }
    else if(n==173){ x=19; y=7; }
    else if(n==174){ x=19; y=10; }
    else if(n==175){ x=19; y=19; }
    else if(n==176){ x=19; y=22; }
    else if(n==177){ x=20; y=2; }
    else if(n==178){ x=20; y=3; }
    else if(n==179){ x=20; y=4; }
    else if(n==180){ x=20; y=5; }
    else if(n==181){ x=20; y=6; }
    else if(n==182){ x=20; y=7; }
    else if(n==183){ x=20; y=8; }
    else if(n==184){ x=20; y=9; }
    else if(n==185){ x=20; y=10; }
    else if(n==186){ x=20; y=11; }
    else if(n==187){ x=20; y=12; }
    else if(n==188){ x=20; y=13;}
    else if(n==189){ x=20; y=16;}
    else if(n==190){ x=20; y=17;}
    else if(n==191){ x=20; y=18;}
    else if(n==192){ x=20; y=19;}
    else if(n==193){ x=20; y=20;}
    else if(n==194){ x=20; y=21;}
    else if(n==195){ x=20; y=22;}
    else if(n==196){ x=20; y=23;}
    else if(n==197){ x=20; y=24;}
    else if(n==198){ x=20; y=25; }
    else if(n==199){ x=20; y=26; }
    else if(n==200){ x=20; y=27; }
    else if(n==201){ x=21; y=2; }
    else if(n==202){ x=21; y=7; }
    else if(n==203){ x=21; y=13; }
    else if(n==204){ x=21; y=16; }
    else if(n==205){ x=21; y=22; }
    else if(n==206){ x=21; y=27; }
    else if(n==207){ x=22; y=2; }
    else if(n==208){ x=22; y=7; }
    else if(n==209){ x=22; y=13; }
    else if(n==210){ x=22; y=16; }
    else if(n==211){ x=22; y=22; }
    else if(n==212){ x=22; y=27; }
    else if(n==213){ x=23; y=2; }
    else if(n==214){ x=23; y=3; }
    else if(n==215){ x=23; y=4; }
    else if(n==216){ x=23; y=7; }
    else if(n==217){ x=23; y=8; }
    else if(n==218){ x=23; y=9; }
    else if(n==219){ x=23; y=10; }
    else if(n==220){ x=23; y=11; }
    else if(n==221){ x=23; y=12; }
    else if(n==222){ x=23; y=13; }
    else if(n==223){ x=23; y=14; }
    else if(n==224){ x=23; y=15; }
    else if(n==225){ x=23; y=16; }
    else if(n==226){ x=23; y=17; }
    else if(n==227){ x=23; y=18; }
    else if(n==228){ x=23; y=19; }
    else if(n==229){ x=23; y=20; }
    else if(n==230){ x=23; y=21; }
    else if(n==231){ x=23; y=22; }
    else if(n==232){ x=23; y=25; }
    else if(n==233){ x=23; y=26; }
    else if(n==234){ x=23; y=27; }
    else if(n==235){ x=24; y=4; }
    else if(n==236){ x=24; y=7; }
    else if(n==237){ x=24; y=10; }
    else if(n==238){ x=24; y=19; }
    else if(n==239){ x=24; y=22; }
    else if(n==240){ x=24; y=25; }
    else if(n==241){ x=25; y=4; }
    else if(n==242){ x=25; y=7; }
    else if(n==243){ x=25; y=10; }
    else if(n==244){ x=25; y=19; }
    else if(n==245){ x=25; y=22; }
    else if(n==246){ x=25; y=25; }
    else if(n==247){ x=26; y=2; }
    else if(n==248){ x=26; y=3; }
    else if(n==249){ x=26; y=4; }
    else if(n==250){ x=26; y=5; }
    else if(n==251){ x=26; y=6; }
    else if(n==252){ x=26; y=7; }
    else if(n==253){ x=26; y=10; }
    else if(n==254){ x=26; y=11;} 
    else if(n==255){ x=26; y=12; }
    else if(n==256){ x=26; y=13; }
    else if(n==257){ x=26; y=16; }
    else if(n==258){ x=26; y=17; }
    else if(n==259){ x=26; y=18; }
    else if(n==260){ x=26; y=19; }
    else if(n==261){ x=26; y=22; }
    else if(n==262){ x=26; y=23; }
    else if(n==263){ x=26; y=24; }
    else if(n==264){ x=26; y=25; }
    else if(n==265){ x=26; y=26; }
    else if(n==266){ x=26; y=27; }
    else if(n==267){ x=27; y=2; }
    else if(n==268){ x=27; y=13; }
    else if(n==269){ x=27; y=16; }
    else if(n==270){ x=27; y=27; }
    else if(n==271){ x=28; y=2; }
    else if(n==272){ x=28; y=13; }
    else if(n==273){ x=28; y=16; }
    else if(n==274){ x=28; y=27; }
    else if(n==275){ x=29; y=2; }
    else if(n==276){ x=29; y=3; }
    else if(n==277){ x=29; y=4; }
    else if(n==278){ x=29; y=5; }
    else if(n==279){ x=29; y=6; }
    else if(n==280){ x=29; y=7; }
    else if(n==281){ x=29; y=8; }
    else if(n==282){ x=29; y=9; }
    else if(n==283){ x=29; y=10; }
    else if(n==284){ x=29; y=11; }
    else if(n==285){ x=29; y=12; }
    else if(n==286){ x=29; y=13; }
    else if(n==287){ x=29; y=14; }
    else if(n==288){ x=29; y=15; }
    else if(n==289){ x=29; y=16; }
    else if(n==290){ x=29; y=17; }
    else if(n==291){ x=29; y=18; }
    else if(n==292){ x=29; y=19; }
    else if(n==293){ x=29; y=20; }
    else if(n==294){ x=29; y=21; }
    else if(n==295){ x=29; y=22; }
    else if(n==296){ x=29; y=23; }
    else if(n==297){ x=29; y=24; }
    else if(n==298){ x=29; y=25; }
    else if(n==299){ x=29; y=26; }
    else if(n==300){ x=29; y=27; }

    Point p;
    p.x = x;
    p.y = y-1;

    return p;
}
