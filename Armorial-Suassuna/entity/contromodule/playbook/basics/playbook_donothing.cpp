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
        /*Role_Default *rl_default = new Role_Default();
        usesRole(rl_default);
        _rl_default.push_back(rl_default);*/
    }
}

void Playbook_DoNothing::run(int numPlayers) {
    /*for(int i = 0; i < numPlayers; i++){
        quint8 playerId = dist()->getPlayer();
        setPlayerRole(playerId, _rl_volante.at(i));
    }*/
    setPlayerRole(0, _rl_volante.at(0));
    //setPlayerRole(1, _rl_default.at(1));
}
