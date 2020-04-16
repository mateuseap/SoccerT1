#include "behaviour_makeapass.h"

QString Behaviour_MakeAPass::name() {
    return "Behaviour_MakeAPass";
}

Behaviour_MakeAPass::Behaviour_MakeAPass() {
}

void Behaviour_MakeAPass::configure() {
    usesSkill(_skill_goToLookTo = new Skill_GoToLookTo());
    usesSkill(_skill_kick = new Skill_Kick());

    setInitialSkill(_skill_goToLookTo);

    addTransition(STATE_KICK, _skill_goToLookTo, _skill_kick);
    addTransition(STATE_GOTOLOOKTO, _skill_kick, _skill_goToLookTo);

    _state = STATE_GOTOLOOKTO;
};

void Behaviour_MakeAPass::nearestPlayer(){
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

void Behaviour_MakeAPass::run(){
    switch(_state){
        case STATE_GOTOLOOKTO:{
            enableTransition(STATE_GOTOLOOKTO);
            nearestPlayer();
            _skill_goToLookTo->setDesiredPosition(loc()->ball());
            if(PlayerBus::ourPlayerAvailable(_nearestPlayerID)){
                _skill_goToLookTo->setAimPosition(PlayerBus::ourPlayer(_nearestPlayerID)->position());
            }
            if(player()->isLookingTo(PlayerBus::ourPlayer(_nearestPlayerID)->position(), 0.15)){
                _state = STATE_KICK;
            }
        }
        break;
        case STATE_KICK:{
            enableTransition(STATE_KICK);
            _skill_kick->setZPower(0.4);
            _skill_kick->setAim(PlayerBus::ourPlayer(_nearestPlayerID)->position());
            _state = STATE_GOTOLOOKTO;
        break;
        }
    }
}
