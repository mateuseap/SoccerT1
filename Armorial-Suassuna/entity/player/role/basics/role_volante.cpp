#include "role_volante.h"

QString Role_Volante::name(){
    return "Role_Volante";
}

Role_Volante::Role_Volante(){
}

void Role_Volante::initializeBehaviours(){
    usesBehaviour(BHV_AREACLEANER, _bh_areaCleaner = new Behaviour_AreaCleaner());
    usesBehaviour(BHV_MARKBALL, _bh_markBall = new Behaviour_MarkBall());
    usesBehaviour(BHV_MARKPLAYER, _bh_markPlayer = new Behaviour_MarkPlayer());
    usesBehaviour(BHV_BARRIER, _bh_barrier = new Behaviour_Barrier());
    usesBehaviour(BHV_DONOTHING, _bh_doNothing = new Behaviour_DoNothing());
    usesBehaviour(BHV_FOLLOWBALL, _bh_followBall = new Behaviour_FollowBall());
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
            _state = BHV_DONOTHING;
        }
    }else if(_ourTeamHasBall){
        _state = BHV_DONOTHING;
    }

    switch(_state){
        case BHV_MARKPLAYER:{
            std::cout<<"ourTeam don't has ball possesion"<<std::endl;
            _bh_markPlayer->setTargetID(_targetID);
            setBehaviour(BHV_MARKPLAYER);
            if(player()->distBall() <= 1.0){
                std::cout<<"Going to kick the ball"<<std::endl;
                //player()->angleTo(loc()->ball());
                //player()->nextPosition().setPosition(loc()->ball().x()+1, loc()->ball().y()+1, loc()->ball().z()+1);
                player()->kick();
            }
            std::cout<<BHV_MARKPLAYER<<" - Behaviour markPlayer"<<std::endl;
        }
        break;
        case BHV_MARKBALL:{
            std::cout<<"ourTeam don't has ball possesion"<<std::endl;
            setBehaviour(BHV_MARKBALL);
            if(player()->distBall() <= 1.0){
                std::cout<<"Going to kick the ball"<<std::endl;
                //player()->angleTo(loc()->ball());
                //player()->nextPosition().setPosition(loc()->ball().x()+1, loc()->ball().y()+1, loc()->ball().z()+1);
                player()->kick();
            }
            std::cout<<BHV_MARKBALL<<" - Behaviour markBall"<<std::endl;
        }
        break;
        case BHV_DONOTHING:{
            if(_ourTeamHasBall){
                std::cout<<"ourTeam ball possesion"<<std::endl;
                setBehaviour(BHV_DONOTHING);
                std::cout<<BHV_DONOTHING<<" - Behaviour doNothing"<<std::endl;
            }else{
                std::cout<<"ourTeam don't has ball possesion"<<std::endl;
                setBehaviour(BHV_DONOTHING);
                std::cout<<BHV_DONOTHING<<" - Behaviour doNothing"<<std::endl;
            }
        }
        break;
    }
}
