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

QString Role_Volante::information(){
    return _information;
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
                if(_opponentPlayerInOurField >= 1){
                    _ourFieldIsSafe = false;
                }
            }
        }
    }
}

void Role_Volante::checkPlayersRoles(){
    for(quint8 i = 0; i < 6; i++){
        if(PlayerBus::ourPlayerAvailable(i)){
            if(PlayerBus::ourPlayer(i)->roleName().toStdString() == "Role_Zagueiro"){
                _zagueiroID = i;
                if(PlayerBus::ourPlayer(i)->playerInformation().toStdString() == "Barrier_Mode"){
                    _zagueiroStatus = "Barrier_Mode";
                }else if(PlayerBus::ourPlayer(i)->playerInformation().toStdString() == "MakeAPass_Mode"){
                    _zagueiroStatus = "MakeAPass_Mode";
                }
            }
        }
    }
}

void Role_Volante::changeInformationStatus(QString mode){
    _information = mode;
}

void Role_Volante::receiveZagueiroInformation(std::string information){
    std::cout<<information<<std::endl;
    _zagueiroStatus = information;
}

void Role_Volante::run(){
    _information = "Nothing";
    _opponentPlayerInOurField = 0;
    _error = 0;
    _theirTeamHasBall = false;
    _ourTeamHasBall = false;
    _nobodyHasBall = false;
    _ourFieldIsSafe = true;

    checkPlayerInsideOurField();
    whoHasBallPossession();
    //checkPlayersRoles();

    //std::cout<<_zagueiroStatus.toStdString()<<" - zagueiroMode"<<std::endl;
    //std::cout<<_opponentPlayerInOurField<<std::endl;

    if((_ourTeamHasBall == false) && (_theirTeamHasBall == false)){
        _nobodyHasBall = true;
    }

    if(_theirTeamHasBall){
        if(_opponentPlayerInOurField == 1){
            //changeInformationStatus("MarkPlayer_Mode");
            _state = BHV_MARKPLAYER;
        }else if(!(_ourFieldIsSafe) && (_opponentPlayerInOurField >= 2)){
            _state = BHV_MARKBALL;
        }else if(_ourFieldIsSafe){
            _state = BHV_BARRIER;
        }
    }else if(_ourTeamHasBall){
        if((player()->hasBallPossession()) || (player()->distBall() <= 0.6)){
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

    if((_zagueiroStatus == "Barrier_Mode") && (_state = BHV_BARRIER) && (_theirTeamHasBall)){
        //std::cout<<"oi"<<std::endl;
        _error = 0.935;
    }

    switch(_state){
        case BHV_MARKPLAYER:{
            //std::cout<<"MarkPlayer_Mode ON"<<std::endl;
            emit sendVolanteInformation("MarkPlayer_Mode");
            _bh_markPlayer->setTargetID(_targetID);
            setBehaviour(BHV_MARKPLAYER);
            if(player()->distBall() <= 0.6){
                _state = BHV_MAKEAPASS;
            }
        }
        break;
        case BHV_MARKBALL:{
            setBehaviour(BHV_MARKBALL);
            if(player()->distBall() <= 0.6){
                _state = BHV_MAKEAPASS;
            }
        }
        break;
        case BHV_BARRIER:{
            if(_theirTeamHasBall){
                _bh_barrier->setYError(_error);
                _bh_barrier->setRadius(1.55);
                setBehaviour(BHV_BARRIER);
            }else if(_ourTeamHasBall){
                _bh_barrier->setRadius(5);
                setBehaviour(BHV_BARRIER);
                if(player()->distBall() <= 0.6){
                    _state = BHV_MAKEAPASS;
                }
            }else if(_nobodyHasBall){
                _bh_barrier->setRadius(5);
                setBehaviour(BHV_BARRIER);
                if(player()->distBall() <= 1){
                    _state = BHV_MAKEAPASS;
                }
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
