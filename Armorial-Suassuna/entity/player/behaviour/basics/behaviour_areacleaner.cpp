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
    float _x, _y, _z, _xx, _yy, _zz, _result, _distance, _actualDistance = MAXFLOAT;
    for(quint8 i = 0 ; i < 6; i++){
        if(player()->playerId() != i){
            if(PlayerBus::ourPlayerAvailable(i)){
                _x = PlayerBus::ourPlayer(i)->position().x()-player()->position().x();
                _y = PlayerBus::ourPlayer(i)->position().y()-player()->position().y();
                _z = PlayerBus::ourPlayer(i)->position().z()-player()->position().z();
                _xx = _x*_x;
                _yy = _y*_y;
                _zz = _z*_z;
                _result = _xx+_yy+_zz;
                _distance = (float)sqrt(_result);
                if(_actualDistance > _distance){
                    _nearestPlayerID = i;
                    _actualDistance = _distance;
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
                if(player()->isLookingTo(PlayerBus::ourPlayer(_nearestPlayerID)->position(), 0.1)){
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
