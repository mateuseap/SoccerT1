/***
 * Maracatronics Robotics
 * Federal University of Pernambuco (UFPE) at Recife
 * http://www.maracatronics.com/
 *
 * This file is part of Armorial project.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***/

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
        /*Role_Default *rl_default = new Role_Default();
        usesRole(rl_default);
        _rl_default.push_back(rl_default);*/
        Role_DoNothing *rl_doNothing = new Role_DoNothing();
        usesRole(rl_doNothing);
        _rl_doNothing.push_back(rl_doNothing);
        Role_Volante *rl_volante = new Role_Volante();
        usesRole(rl_volante);
        _rl_volante.push_back(rl_volante);
    }
}

void Playbook_DoNothing::run(int numPlayers) {
    /*for(int i = 0; i < numPlayers; i++){
        quint8 playerId = dist()->getPlayer();
        setPlayerRole(playerId, _rl_volante.at(i));
    }*/
    //setPlayerRole(0, _rl_default.at(0));
    setPlayerRole(0, _rl_volante.at(0));
    setPlayerRole(1, _rl_doNothing.at(1));
    setPlayerRole(2, _rl_doNothing.at(2));
}
