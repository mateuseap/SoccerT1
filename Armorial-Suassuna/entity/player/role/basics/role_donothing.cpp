#include "role_donothing.h"

QString Role_DoNothing::name(){
    return "Role_DoNothing";
}

Role_DoNothing::Role_DoNothing() {
}

void Role_DoNothing::initializeBehaviours(){
    usesBehaviour(BHV_DONOTHING, _bh_doNothing = new Behaviour_DoNothing());
}

void Role_DoNothing::configure(){
}

void Role_DoNothing::run(){
    setBehaviour(BHV_DONOTHING);
}
