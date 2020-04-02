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
        Role_Default *rl_default = new Role_Default();
        usesRole(rl_default);
        _rl_default.push_back(rl_default);
        Role_DoNothing *rl_doNothing = new Role_DoNothing();
        usesRole(rl_doNothing);
        _rl_doNothing.push_back(rl_doNothing);
        /*Role_Volante *rl_volante = new Role_Volante();
        usesRole(rl_volante);
        _rl_volante.push_back(rl_volante);*/
    }
}

void Playbook_DoNothing::run(int numPlayers) {
    /*for(int i = 0; i < numPlayers; i++){
        quint8 playerId = dist()->getPlayer();
        setPlayerRole(playerId, _rl_volante.at(i));
    }*/
    setPlayerRole(0, _rl_default.at(0));
    setPlayerRole(1, _rl_doNothing.at(1));
    setPlayerRole(2, _rl_doNothing.at(2));
    //setPlayerRole(0, _rl_volante.at(0));
}
