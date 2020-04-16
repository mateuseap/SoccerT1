#include "playbook_donothing.h"

QString Playbook_DoNothing::name() {
    return "Playbook_DoNothing";
}

Playbook_DoNothing::Playbook_DoNothing() {
}

int Playbook_DoNothing::maxNumPlayer() {
    return INT_MAX;
}

void Playbook_DoNothing::configure(int numPlayers) {
    for(int i = 0; i < numPlayers; i++) {
        Role_Volante *rl_volante = new Role_Volante();
        usesRole(rl_volante);
        _rl_volante.push_back(rl_volante);
        Role_CentroAvante *rl_centroAvante = new Role_CentroAvante();
        usesRole(rl_centroAvante);
        _rl_centroAvante.push_back(rl_centroAvante);
    }
}

void Playbook_DoNothing::run(int numPlayers) {
    setPlayerRole(0, _rl_volante.at(0));
    //setPlayerRole(1, _rl_centroAvante.at(1));
}
