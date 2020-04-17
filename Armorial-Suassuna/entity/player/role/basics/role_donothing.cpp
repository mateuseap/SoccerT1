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

#include "role_donothing.h"

QString Role_DoNothing::name(){
    return "Role_DoNothing";
}

Role_DoNothing::Role_DoNothing(){
}

void Role_DoNothing::initializeBehaviours(){
    usesBehaviour(BHV_DONOTHING, _bh_doNothing = new Behaviour_DoNothing());
}

void Role_DoNothing::configure(){
}

void Role_DoNothing::run(){
    setBehaviour(BHV_DONOTHING);
}
