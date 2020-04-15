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
    _ourTeamHasBall = false;
    _theirTeamHasBall = false;

    checkPlayerInsideOurField();
    whoHasBallPossession();

    if(_theirTeamHasBall){
        if(_opponentPlayerInOurField == 1){
            _state = BHV_MARKPLAYER;
        }else if(!(_ourFieldIsSafe)){
            _state = BHV_MARKBALL;
        }else{
            _state = BHV_BARRIER;
        }
    }else if(_ourTeamHasBall){
        if(player()->hasBallPossession()){
            _state = BHV_MAKEAPASS;
        }else{
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
            }else if(_ourTeamHasBall){
                setBehaviour(BHV_BARRIER);
                _bh_barrier->setRadius(5);
            }
        }
        break;
        case BHV_MAKEAPASS:{
            setBehaviour(BHV_MAKEAPASS);
        }
        break;
    }
}
