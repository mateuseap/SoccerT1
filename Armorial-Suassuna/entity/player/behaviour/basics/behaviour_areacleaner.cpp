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

#include "behaviour_areacleaner.h"

QString Behaviour_AreaCleaner::name(){
    return "Behaviour_AreaCleaner";
}

Behaviour_AreaCleaner::Behaviour_AreaCleaner(){
}

void Behaviour_AreaCleaner::configure(){
    usesSkill(_skill_goToLookTo = new Skill_GoToLookTo());
    usesSkill(_skill_kick = new Skill_Kick());

    setInitialSkill(_skill_goToLookTo);

    addTransition(STATE_KICK, _skill_goToLookTo, _skill_kick);
    addTransition(STATE_GOTOLOOKTO, _skill_kick, _skill_goToLookTo);

    _state = STATE_GOTOLOOKTO;
};

void Behaviour_AreaCleaner::nearestPlayer(){
    float _x, _y, _xx, _yy, _result, _distance, _temp = MAXFLOAT;
    for(quint8 i = 0 ; i < 6; i++){
        if(player()->playerId() != i){
            if(PlayerBus::ourPlayerAvailable(i)){
                _x = PlayerBus::ourPlayer(i)->position().x()-player()->position().x();
                _y = PlayerBus::ourPlayer(i)->position().y()-player()->position().y();
                _xx = _x*_x;
                _yy = _y*_y;
                _result = _xx+_yy;
                _distance = (float)sqrt(_result);
                if(_temp >= _distance){
                    _nearestPlayerID = i;
                    _temp = _distance;
                    _actualDistance = _temp;
                }
            }
         }
    }
}

void Behaviour_AreaCleaner::run(){
    switch(_state){
        case STATE_GOTOLOOKTO:{
            enableTransition(STATE_GOTOLOOKTO);
            if(loc()->isInsideOurArea(loc()->ball())){
                nearestPlayer();
                std::cout<<"The nearestPlayer is - "<<(int)_nearestPlayerID<<std::endl;
                _skill_goToLookTo->setDesiredPosition(loc()->ball());
                if(PlayerBus::ourPlayerAvailable(_nearestPlayerID)){
                    _skill_goToLookTo->setAimPosition(PlayerBus::ourPlayer(_nearestPlayerID)->position());
                }
                if(player()->isLookingTo(PlayerBus::ourPlayer(_nearestPlayerID)->position(), 0.05)){
                    //std::cout<<"Our Player"<<" X: "<<player()->position().x()<<" Y: "<<player()->position().y()<<std::endl;
                    //std::cout<<"Their Player"<<" X: "<<PlayerBus::ourPlayer(_nearestPlayerID)->position().x()<<" Y: "<<PlayerBus::ourPlayer(_nearestPlayerID)->position().y()<<std::endl;
                    _state = STATE_KICK;
                }
            }
        }
        break;
        case STATE_KICK:{
            if(loc()->isInsideOurArea(loc()->ball())){
                enableTransition(STATE_KICK);
                _skill_kick->setKickPower(0, 4);
                _skill_kick->setAim(PlayerBus::ourPlayer(_nearestPlayerID)->position());
            }
            _state = STATE_GOTOLOOKTO;
        break;
        }
    }
}
