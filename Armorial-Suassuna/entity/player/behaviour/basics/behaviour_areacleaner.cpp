#include "behaviour_areacleaner.h"
#include <utils/knn/knn.hh>
#include <QObject>

QString Behaviour_AreaCleaner::name() {
    return "Behaviour_AreaCleaner";
}

Behaviour_AreaCleaner::Behaviour_AreaCleaner() {
}

void Behaviour_AreaCleaner::configure() {
    usesSkill(_skill_goToLookTo = new Skill_GoToLookTo());
    usesSkill(_skill_kick = new Skill_Kick());

    setInitialSkill(_skill_goToLookTo);

    addTransition(STATE_KICK, _skill_goToLookTo, _skill_kick);
    addTransition(STATE_GOTOLOOKTO, _skill_kick, _skill_goToLookTo);

    _state: STATE_GOTOLOOKTO;
};

void Behaviour_AreaCleaner::run() {
    switch(_state){
        case STATE_GOTOLOOKTO:{
            enableTransition(STATE_GOTOLOOKTO);
            if(loc()->isInsideOurArea(loc()->ball())){
                /*if(PlayerBus::ourPlayerAvailable(player()->playerId())){
                    std::cout<<"Oi"<<std::endl;
                    QList<kNN::element> ourNearestP = kNN::getOurKNN(2, PlayerBus::ourPlayer(player()->playerId())->position());
                    Position _kickPos;
                    _kickPos.setPosition(PlayerBus::ourPlayer(ourNearestP[1].id)->position().x(), PlayerBus::ourPlayer(ourNearestP[1].id)->position().y(), PlayerBus::ourPlayer(ourNearestP[1].id)->position().z());
                    Position behindBall = WR::Utils::threePoints(loc()->ball(), _kickPos, 0.2, GEARSystem::Angle::pi);
                    std::cout<<"Dale"<<std::endl;
                    _skill_goToLookTo->setDesiredPosition(behindBall);
                    _skill_goToLookTo->setAimPosition(loc()->ball());
                }else{*/
                    Position _center;
                    _center.setPosition(0,0,0);
                    Position behindBall = WR::Utils::threePoints(loc()->ball(), _center, 0.2, GEARSystem::Angle::pi);
                    _skill_goToLookTo->setDesiredPosition(behindBall);
                    _skill_goToLookTo->setAimPosition(loc()->ball());
                //}
                _state = STATE_KICK;
            }
        }
        break;
        case STATE_KICK:{
            if(loc()->isInsideOurArea(loc()->ball())){
                enableTransition(STATE_KICK);
                _skill_kick->setZPower(0.8);
                _skill_kick->setAim(loc()->ball());
            }

            _state = STATE_GOTOLOOKTO;
        break;
        }
    }
}
