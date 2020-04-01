#include "role_volante.h"
//#include <entity/contromodule/mrcteam.h>

QString Role_Volante::name(){
    return "Role_Volante";
}

Role_Volante::Role_Volante() {
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
    //_state = BHV_BARRIER;
}

void Role_Volante::run(){
    _opPinOurF = 0;
    _ourFisSafe = true;
    _ourThasBall = false;
    _theirThasBall = false;

    for(quint8 i = 0; i < 6; i++){
        if(PlayerBus::theirPlayerAvailable(i)){
            if(loc()->isInsideOurField(PlayerBus::theirPlayer(i)->position())){
                _opPinOurF++;
                _tgID = i;
                if(_opPinOurF > 1){
                    _ourFisSafe = false;
                    break;
                }
            }
            if(PlayerBus::theirPlayer(i)->hasBallPossession()){
                _playerHasB = i;
                _theirThasBall = true;
            }
        }
        if(PlayerBus::ourPlayerAvailable(i)){
            if(PlayerBus::ourPlayer(i)->hasBallPossession()){
                _playerHasB = i;
                _ourThasBall = true;
            }
        }
    }

    if(_theirThasBall){
        std::cout<<"ourTeam don't has ball possesion"<<std::endl;
        if(_opPinOurF == 1){
            _bh_markPlayer->setTargetID(_tgID);
            setBehaviour(BHV_MARKPLAYER);
            if(player()->distBall() <= 1.0){
                std::cout<<"Going to kick the ball"<<std::endl;
                //player()->angleTo(loc()->ball());
                //player()->nextPosition().setPosition(loc()->ball().x()+1, loc()->ball().y()+1, loc()->ball().z()+1);
                player()->kick();
            }
            std::cout<<BHV_MARKPLAYER<<" - Behaviour markPlayer"<<std::endl;
        }else if(!(_ourFisSafe)){
            setBehaviour(BHV_MARKBALL);
            if(player()->distBall() <= 1.0){
                std::cout<<"Going to kick the ball"<<std::endl;
                //player()->angleTo(loc()->ball());
                //player()->nextPosition().setPosition(loc()->ball().x()+1, loc()->ball().y()+1, loc()->ball().z()+1);
                player()->kick();
            }
            std::cout<<BHV_MARKBALL<<" - Behaviour markBall"<<std::endl;
        }else{
            setBehaviour(BHV_DONOTHING);
            std::cout<<BHV_DONOTHING<<" - Behaviour doNothing"<<std::endl;
        }
    }else if(_ourThasBall){
        std::cout<<"ourTeam ball possesion"<<std::endl;
        setBehaviour(BHV_DONOTHING);
        std::cout<<BHV_DONOTHING<<" - Behaviour doNothing"<<std::endl;
    }
}
