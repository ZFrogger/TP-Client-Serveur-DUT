#include <iostream>
#include <thread>
#include <string>

#include <sys/time.h>
#include "socket.h"
#include "Table.h"

using namespace std;
using namespace stdsock;

int string_to_int(string str) {
    if (str == "0") {
        return 0;
    } else if (str == "1") {
        return 1;
    } else if (str == "2") {
        return 2;
    } else if (str == "3") {
        return 3;
    } else if (str == "4") {
        return 4;
    } else if (str == "5") {
        return 5;
    } else if (str == "6") {
        return 6;
    } else if (str == "7") {
        return 7;
    }
    return -1;
}

void worker(StreamSocket* client1, StreamSocket* client2) {
    cout << "Got client 1 and 2!" << endl;

    cout << "Client 1 : " << client1->getIP() << ":" << client1->getPort() << endl;
    cout << "Client 2 : " << client2->getIP() << ":" << client2->getPort() << endl;
    int cardsj1[8], cardsj2[8];
    for (int i = 0; i < 8; ++i) {
        cardsj1[i] = 1;
        cardsj2[i] = 1;
    }
    int round = 0;
    bool stop = false;
    bool generalj1 = false, generalj2 = false;
    int playedcardj1 = 0, playedcardj2 = 0;
    int points1 = 0, points2 = 0, points_en_jeu = 0;
    bool connected = true;
    bool erreurj1 = true, erreurj2 = true;
    while (connected) {
        erreurj1 = true;
        erreurj2 = true;
        round++;
        cout << "Round "<< round << " : \t" << endl;
        cout << "Score : \t"<< points1 << " : " << points2 << "\nPoints en jeu :\t" << points_en_jeu << endl;
        string msg1, msg2;
        // Joueur 1
        while (erreurj1) {
            msg1.clear();
            erreurj1 = false;
            if (client1->read(msg1) <= 0) {
                client1->send("777");
            }
            if (msg1 == "QUIT") {
                stop = true;
                connected = false;
            } else {
                string command1 = msg1.substr(0 ,msg1.find(' '));
                if (command1 == "PLAY") {
                    string cardmsg = msg1.substr(msg1.find(' ') + 1, 1);
                    cout << "Card J1: \t"<< cardmsg << endl;
                    int card = string_to_int(cardmsg);
                    if (card >= 0 && card <= 7) {
                        if (cardsj1[card]) {
                            cardsj1[card] = 0;
                            playedcardj1 = card;
                        } else {
                            erreurj1 = true;
                            client1->send("300");
                        }
                    } else {
                        erreurj1 = true;
                        client1->send("300");
                    }
                } else {
                    erreurj1 = true;
                    cout << "Server J1: \t"<< msg1 << endl;
                    client1->send("77");
                }
            }

        }
        // Joueur 2
        while (erreurj2) {
            msg2.clear();
            erreurj2 = false;
            if (client2->read(msg2) <= 0) {
                client2->send("777");
            }
            if (msg2 == "QUIT") {
                stop = true;
                connected = false;
            } else {
                string command2 = msg2.substr(0 ,msg2.find(' '));
                if (command2 == "PLAY") {
                    string cardmsg = msg2.substr(msg2.find(' ') + 1, 1);
                    cout << "Card J2: \t"<< cardmsg << endl;
                    int card = string_to_int(cardmsg);
                    if (card >= 0 && card <= 7) {
                        if (cardsj2[card]) {
                            cardsj2[card] = 0;
                            playedcardj2 = card;
                        } else {
                            erreurj2 = true;
                            client2->send("300");
                        }
                    } else {
                        erreurj2 = true;
                        client2->send("300");
                    }
                } else {
                    erreurj2 = true;
                    cout << "Server J2: \t"<< msg2 << endl;
                    client2->send("77");
                }
            }
        }
        if (stop) {
            client1->send("999");
            client2->send("999");
        } else {
            // Jeu
            cout << "Jeu: \t"<< "J1 : " << playedcardj1 << " vs " << "J2 : " << playedcardj2 << endl;
            int general = 0;
            if (generalj1 && generalj2 || !generalj1 && !generalj2) {
                general = 0;
            } else if (generalj1) {
                general = 1;
            } else if (generalj2) {
                general = 2;
            }
            int result = table[general][playedcardj1][playedcardj2];
            bool egalite = false, j1gagne, nocards = true;
            for (int i = 0; i < 8; ++i) {
                if (cardsj1[i] == 1 || cardsj2[i] == 1)
                    nocards = false;
            }
            if (result == 0) {
                cout << "Jeu: \t" << "Egalité" << endl;
                points_en_jeu++;
                egalite = true;
            } else if (result == 1) {
                cout << "Jeu: \t" << "J1 gagne le combat" << endl;
                points_en_jeu++;
                if (playedcardj1 == 4) {
                    points1++;
                }
                points1 += points_en_jeu;
                points_en_jeu = 0;
                j1gagne = true;
            } else if (result == 2) {
                cout << "Jeu: \t" << "J2 gagne le combat" << endl;
                points_en_jeu++;
                if (playedcardj2 == 4) {
                    points2++;
                }
                points2 += points_en_jeu;
                points_en_jeu = 0;
                j1gagne = false;
            } else if (result == -1) {
                cout << "Jeu: \t" << "J2 gagne la partie" << endl;
                client1->send("201");
                client2->send("200");
                connected = false;
                break;
            } else if (result == -2) {
                cout << "Jeu: \t" << "J1 gagne la partie" << endl;
                client1->send("200");
                client2->send("201");
                connected = false;
                break;
            }
            if (points1 >= 4) {
                cout << "Jeu: \t" << "J1 gagne la partie" << endl;
                client1->send("200");
                client2->send("201");
                connected = false;
                break;
            } else if (points2 >= 4) {
                cout << "Jeu: \t" << "J2 gagne la partie" << endl;
                client1->send("201");
                client2->send("200");
                connected = false;
                break;
            } else if (nocards) {
                client1->send("202");
                client2->send("202");
                connected = false;
                break;
            } else if (!egalite) {
                if (j1gagne) {
                    client1->send("100");
                    client2->send("101");
                } else {
                    client1->send("101");
                    client2->send("100");
                }
            } else if (egalite) {
                client1->send("102");
                client2->send("102");
            }

        }
    }
    delete client1;
    cout << "Client 1 disconnected" << endl;
    delete client2;
    cout << "Client 2 disconnected" << endl;
}


int main()
{
    ConnectionPoint *server=new ConnectionPoint(7777);

    int err= server->init();
    if (err != 0) {
        cout << strerror(err) << endl;
        exit(err);
    }

    while (true) {
        StreamSocket* client1 = server->accept();
        StreamSocket* client2 = server->accept();

        if (!client1->valid()) {
            delete client1;
            continue;
        }
        if (!client2->valid()) {
            delete client2;
            continue;
        }

        thread (worker, client1, client2).detach();
    }



    delete server;
    return 0;
}
