#include "role_volante.h"

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
}

void Role_Volante::configure(){
    _state = BHV_BARRIER;
}

void Role_Volante::run(){
    switch(_state){
        case BHV_BARRIER:{
            setBehaviour(BHV_BARRIER);
            std::cout<<getActualBehaviour()<<" - "<<" Behaviour da Barreira"<<std::endl;
            if((loc()->isInsideOurArea(loc()->ball())) || (loc()->isInsideTheirArea(loc()->ball()))){
                _state = BHV_AREACLEANER;
            }else if((player()->position().x() < 0) && !(loc()->isInsideTheirArea(loc()->ball())) && !(loc()->isInsideOurArea(loc()->ball()))){
                _state = BHV_MARKBALL;
            }
        }
        break;
        case BHV_MARKBALL:{
            setBehaviour(BHV_MARKBALL);
            std::cout<<getActualBehaviour()<<" - "<<" Behaviour do MarkBall"<<std::endl;
            if((loc()->isInsideOurArea(loc()->ball())) || (loc()->isInsideTheirArea(loc()->ball()))){
                _state = BHV_AREACLEANER;
            }else if((player()->position().x() >= 0) && !(loc()->isInsideOurArea(loc()->ball())) && !(loc()->isInsideTheirArea(loc()->ball()))){
                _state = BHV_BARRIER;
            }
        }
        break;
        case BHV_AREACLEANER:{
            setBehaviour(BHV_AREACLEANER);
            std::cout<<getActualBehaviour()<<" - "<<" Behaviour do AreaCleaner"<<std::endl;
            if((player()->position().x() >= 0) && !(loc()->isInsideOurArea(loc()->ball())) && !(loc()->isInsideTheirArea(loc()->ball()))){
                _state = BHV_BARRIER;
            }else if((player()->position().x() < 0) && !(loc()->isInsideTheirArea(loc()->ball())) && !(loc()->isInsideOurArea(loc()->ball()))){
                _state = BHV_MARKBALL;
            }
        }
        break;
    }
}
