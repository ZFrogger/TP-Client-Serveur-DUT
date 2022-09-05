#include <iostream>
#include <thread>

#include <sys/time.h>
#include "socket.h"

using namespace std;
using namespace stdsock;

int main(int argc, char* argv[])
{
    int port;

    if(argc!=3)
    {
        printf("usage: %s server_address port\n", argv[0]);
        return 0;
    }

    if(sscanf(argv[2], "%d", &port)!=1)
    {
        printf("usage: %s server_address port\n", argv[0]);
        return 1;
    }

    //
    StreamSocket *sock=new StreamSocket(argv[1],port);

    int err= sock->connect();
    if (err!=0) {
        delete sock;
        perror("[-]Error in connection: ");
        return(err);
    }
    cout << "[+]Connected to Server.\n";
    string msg_send, msg;
    bool playing = true;
    while (playing) {
        cout << "Message : ";
        getline(cin,msg_send);
        if (msg_send != "") {
            cout << "Message send : " << msg_send << endl;
            sock->send(msg_send);
            if(sock->read(msg) < 0){
                    printf("[-]Error in receiving data.\n");
            }else{
                cout << "Message reçu : \t"<< msg << endl;
                if (msg == "999") {
                    playing = false;
                } else if (msg == "100") { // Carte joué et combat gagné
                    cout << "Combat gagné !" << endl;
                } else if (msg == "101") { // Carte joué mais combat perdu
                    cout << "Combat perdu." << endl;
                } else if (msg == "102") { // Carte joué et égalité
                    cout << "Egalité." << endl;
                } else if (msg == "200") { // Carte joué et partie gagné
                    cout << "Vous avez gagné !" << endl;
                    playing = false;
                } else if (msg == "201") { // Carte joué et partie perdu
                    cout << "Vous avez perdu." << endl;
                    playing = false;
                } else if (msg == "202") { // Carte joué et partie égalité
                    cout << "Egalité, vous n'avez plus de cartes." << endl;
                    playing = false;
                } else if (msg == "300") { // Erreur carte non jouée
                    cout << "Erreur de carte jouée." << endl;
                }
                else if (msg == "77") { // Commande non reconnue
                    cout << "Erreur commande non reconnue" << endl;
                }

            }
            msg.clear();
            msg_send.clear();
        }
    }
    delete sock;
    return 0;
}
