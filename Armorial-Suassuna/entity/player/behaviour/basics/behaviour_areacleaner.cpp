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
                _skill_kick->setZPower(0.4);
                _skill_kick->setAim(PlayerBus::ourPlayer(_nearestPlayerID)->position());
            }
            _state = STATE_GOTOLOOKTO;
        break;
        }
    }
}
