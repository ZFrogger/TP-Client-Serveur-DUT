#ifndef TABLE_H
#define TABLE_H

// ici table[0][a][b] renvoie 0 si égalité, 1 si la carte a gagne, 2 si c'est la b.
// Et négatif si des joueurs perd la partie (-2 si a=princesse, b=prince, ou -1 si inverse)
// on a le numéro joueur gagnant en faisant 3+res car 3-2=1 et 3-1=2
// table[0] si aucun ou 2 generaux
// table[1] si joueur1 a joue le general
// table[2] si joueur2 a joue le general
// donc tester table[1][a][b] ou table[2][a][b] selon j1 ou j2 a joué le général au tour préc.

//Musicien : 0       //Princesse : 1        //Espion : 2         //Assassin : 3
//Ambassadeur : 4   //Magicien : 5         //General : 6        //Prince : 7
const int table[3][8][8]={{{0,0,0,0,0,2,0,0},
 {0,0,2,1,2,2,2,-2},
 {0,1,0,1,2,2,2,2},
 {0,2,2,0,1,2,1,2},
 {0,1,1,2,0,2,2,2},
 {1,1,1,1,1,0,2,2},
 {0,1,1,2,1,1,0,2},
 {0,-1,1,1,1,1,1,0}},
{{0,0,0,0,0,2,0,0},
 {0,1,1,0,2,2,2,-2},
 {0,1,1,2,0,2,2,2},
 {0,2,2,2,2,0,1,2},
 {0,1,1,2,1,1,0,2},
 {1,1,1,1,1,1,1,0},
 {0,0,0,0,0,0,0,0},
 {0,-1,1,1,1,1,1,1}},
{{0,0,0,0,0,2,0,0},
 {0,2,2,1,2,2,0,-2},
 {0,2,2,1,2,2,0,2},
 {0,0,1,1,1,2,0,2},
 {0,1,0,1,2,2,0,2},
 {1,1,1,0,2,2,0,2},
 {0,1,1,2,0,2,0,2},
 {0,-1,1,1,1,0,0,2}}};

#endif
