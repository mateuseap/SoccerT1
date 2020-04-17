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

#include "role_volante.h"

QString Role_Volante::name(){
    return "Role_Volante";
}

Role_Volante::Role_Volante(){
}

void Role_Volante::initializeBehaviours(){
    usesBehaviour(BHV_MARKPLAYER, _bh_markPlayer = new Behaviour_MarkPlayer());
    usesBehaviour(BHV_MARKBALL, _bh_markBall = new Behaviour_MarkBall());
    usesBehaviour(BHV_BARRIER, _bh_barrier = new Behaviour_Barrier());
    usesBehaviour(BHV_MAKEAPASS, _bh_makeAPass = new Behaviour_MakeAPass());
}

void Role_Volante::configure(){
}

void Role_Volante::whoHasBallPossession(){
    for(quint8 i = 0; i < 6; i++){
        if(PlayerBus::theirPlayerAvailable(i)){
            if(PlayerBus::theirPlayer(i)->hasBallPossession()){
                _playerHasBall = i;
                _theirTeamHasBall = true;
                break;
            }
        }
        if(PlayerBus::ourPlayerAvailable(i)){
            if(PlayerBus::ourPlayer(i)->hasBallPossession()){
                _playerHasBall = i;
                _ourTeamHasBall = true;
                break;
            }
        }
    }
}

void Role_Volante::checkPlayerInsideOurField(){
    for(quint8 i = 0; i < 6; i ++){
        if(PlayerBus::theirPlayerAvailable(i)){
            if(loc()->isInsideOurField(PlayerBus::theirPlayer(i)->position())){
                _opponentPlayerInOurField++;
                _targetID = i;
                if(_opponentPlayerInOurField > 1){
                    _ourFieldIsSafe = false;
                }
            }
        }
    }
}

void Role_Volante::run(){
    _opponentPlayerInOurField = 0;
    _ourFieldIsSafe = true;
    _theirTeamHasBall = false;
    _ourTeamHasBall = false;
    _nobodyHasBall = false;

    checkPlayerInsideOurField();
    whoHasBallPossession();

    if((_ourTeamHasBall == false) && (_theirTeamHasBall == false)){
        _nobodyHasBall = true;
    }

    if(_theirTeamHasBall){
        if(_opponentPlayerInOurField == 1){
            _state = BHV_MARKPLAYER;
        }else if(!(_ourFieldIsSafe)){
            _state = BHV_MARKBALL;
        }else{
            _state = BHV_BARRIER;        }
    }else if(_ourTeamHasBall){
        if(player()->hasBallPossession()){
            _state = BHV_MAKEAPASS;
        }else{
            _state = BHV_BARRIER;
        }
    }else if(_nobodyHasBall){
        if(loc()->isInsideOurField(loc()->ball())){
            _state = BHV_MAKEAPASS;
        }else if((loc()->isInsideTheirField(loc()->ball())) && (_state != BHV_MAKEAPASS)){
            _state = BHV_BARRIER;
        }
    }

    switch(_state){
        case BHV_MARKPLAYER:{
            _bh_markPlayer->setTargetID(_targetID);
            setBehaviour(BHV_MARKPLAYER);
            if(player()->distBall() <= 0.5){
                _state = BHV_MAKEAPASS;
            }
        }
        break;
        case BHV_MARKBALL:{
            setBehaviour(BHV_MARKBALL);
            if(player()->distBall() <= 0.5){
                _state = BHV_MAKEAPASS;
            }
        }
        break;
        case BHV_BARRIER:{
            if(_theirTeamHasBall){
                setBehaviour(BHV_BARRIER);
                _bh_barrier->setRadius(1.55);
            }else if((_ourTeamHasBall) || (_nobodyHasBall)){
                setBehaviour(BHV_BARRIER);
                _bh_barrier->setRadius(5);
                if(player()->distBall() <= 1){
                    _state = BHV_MAKEAPASS;
                }
            }
        }
        break;
        case BHV_MAKEAPASS:{
            setBehaviour(BHV_MAKEAPASS);
        }
        break;
    }
}
