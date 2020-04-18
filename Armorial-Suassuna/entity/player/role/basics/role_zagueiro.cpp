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

#include "role_zagueiro.h"

QString Role_Zagueiro::name(){
    return "Role_Zagueiro";
}

QString Role_Zagueiro::information(){
    return _information;
}

Role_Zagueiro::Role_Zagueiro(){
}

void Role_Zagueiro::initializeBehaviours(){
    usesBehaviour(BHV_BARRIER, _bh_barrier = new Behaviour_Barrier());
    usesBehaviour(BHV_MAKEAPASS, _bh_makeAPass = new Behaviour_MakeAPass());
    usesBehaviour(BHV_MARKPLAYER, _bh_markPlayer = new Behaviour_MarkPlayer());
}

void Role_Zagueiro::configure(){
}

void Role_Zagueiro::whoHasBallPossession(){
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

void Role_Zagueiro::checkPlayersRoles(){
    for(quint8 i = 0; i < 6; i++){
        if(PlayerBus::ourPlayerAvailable(i)){
            if(PlayerBus::ourPlayer(i)->roleName().toStdString() == "Role_Volante"){
                _volanteID = i;
                if(PlayerBus::ourPlayer(i)->playerInformation().toStdString() == "MarkPlayer_Mode"){
                    _volanteStatus = "MarkPlayer_Mode";
                }else if(PlayerBus::ourPlayer(i)->playerInformation().toStdString() == "MakeAPass_Mode"){
                    _volanteStatus = "MakeAPass_Mode";
                }
            }
        }
    }
}

void Role_Zagueiro::nearestOpponentToOurGoal(){
    _distanceToOurGoal = 100000000;
    for(quint8 i = 0; i < 6; i++){
        if(PlayerBus::theirPlayerAvailable(i)){
            if(loc()->isInsideOurField(PlayerBus::theirPlayer(i)->position())){
                float _actualDistanceToOurGoal = PlayerBus::theirPlayer(i)->distOurGoal();
                if(_actualDistanceToOurGoal <= _distanceToOurGoal){
                    _distanceToOurGoal = _actualDistanceToOurGoal;
                    _nearestOpponentToOurGoalID = i;
                }
            }
        }
    }
}

void Role_Zagueiro::checkCriticZone(){
    if(player()->team()->teamColor() == Colors::YELLOW){
        _criticArea = 6.3;
    }else if(player()->team()->teamColor() == Colors::BLUE){
        _criticArea = 2.5;
    }
    for(quint8 i = 0; i < 6; i ++){
        if(PlayerBus::theirPlayerAvailable(i)){
            if(((PlayerBus::theirPlayer(i)->distOurGoal() >= _criticArea) && (player()->team()->teamColor() == Colors::YELLOW)) || ((PlayerBus::theirPlayer(i)->distOurGoal() <= _criticArea) && (player()->team()->teamColor() == Colors::BLUE))){
                _criticZoneIsSafe = false;
                break;
            }else{
                if(PlayerBus::ourPlayerAvailable(_volanteID)){
                    if(_volanteStatus != "MarkPlayer_Mode"){
                        nearestOpponentToOurGoal();
                        _targetID = _nearestOpponentToOurGoalID;
                    }
                }
            }
        }
    }
}

void Role_Zagueiro::checkPlayerInsideOurField(){
    for(quint8 i = 0; i < 6; i ++){
        if(PlayerBus::theirPlayerAvailable(i)){
            if(loc()->isInsideOurField(PlayerBus::theirPlayer(i)->position())){
                _opponentPlayerInOurField++;
                if(_opponentPlayerInOurField >= 1){
                    _ourFieldIsSafe = false;
                }
            }
        }
    }
}

void Role_Zagueiro::receiveVolanteInformation(std::string information){
    std::cout<<information<<std::endl;
    _volanteStatus = information;
}

void Role_Zagueiro::run(){
    _information = "Nothing";
    _theirTeamHasBall = false;
    _ourTeamHasBall = false;
    _nobodyHasBall = false;
    _criticZoneIsSafe = true;
    _ourFieldIsSafe = true;

    whoHasBallPossession();
    checkPlayerInsideOurField();
    //checkPlayersRoles();
    checkCriticZone();

    //std::cout<<_volanteStatus.toStdString()<<" - volanteMode"<<std::endl;
    //std::cout<<(int)_nearestOpponentToOurGoalID<<" || "<<_distanceToOurGoal<<" - nearestToOurGoal"<<std::endl;

    if((_ourTeamHasBall == false) && (_theirTeamHasBall == false)){
        _nobodyHasBall = true;
    }

    if(_theirTeamHasBall){
        if(_ourFieldIsSafe){
            _state = BHV_BARRIER;
        }else{
            if(_criticZoneIsSafe){
                if(_volanteStatus != "MarkPlayer_Mode"){
                    nearestOpponentToOurGoal();
                    _state = BHV_MARKPLAYER;
                }else{
                    _state = BHV_BARRIER;
                }
            }else{
                _state = BHV_BARRIER;
            }
        }
    }else if(_ourTeamHasBall){
        if(_ourFieldIsSafe){
            if((player()->hasBallPossession()) || (player()->distBall() <= 0.6)){
                _state = BHV_MAKEAPASS;
            }else{
                _state = BHV_BARRIER;
            }
        }else{
            if(_criticZoneIsSafe){
                if((player()->hasBallPossession()) || (player()->distBall() <= 0.6)){
                    _state = BHV_MAKEAPASS;
                }else{
                    if(_volanteStatus != "MarkPlayer_Mode"){
                        nearestOpponentToOurGoal();
                        _state = BHV_MARKPLAYER;
                    }else{
                        _state = BHV_BARRIER;
                    }
                }
            }else{
                _state = BHV_BARRIER;
            }
        }
    }else if(_nobodyHasBall){
        if(_ourFieldIsSafe){
            if(player()->distBall() <= 1.5){
                 _state = BHV_MAKEAPASS;
            }else{
                 _state = BHV_BARRIER;
            }
        }else{
            if(_criticZoneIsSafe){
                if(_volanteStatus != "MarkPlayer_Mode"){
                    nearestOpponentToOurGoal();
                    _state = BHV_MARKPLAYER;
                }else{
                    _state = BHV_BARRIER;
                }
            }else{
                _state = BHV_BARRIER;
            }
        }
    }

    switch(_state){
        case BHV_BARRIER:{
            //_information = "Barrier_Mode";
            emit sendZagueiroInformation("Barrier_Mode");
            _bh_barrier->setRadius(1.55);
            setBehaviour(BHV_BARRIER);
        }
        break;
        case BHV_MARKPLAYER:{
            _bh_markPlayer->setTargetID(_targetID);
            setBehaviour(BHV_MARKPLAYER);
            if(player()->distBall() <= 0.6){
                _state = BHV_MAKEAPASS;
            }
        }
        break;
        case BHV_MAKEAPASS:{
            //_information = "MakeAPass_Mode";
            setBehaviour(BHV_MAKEAPASS);
        }
        break;
    }
}
