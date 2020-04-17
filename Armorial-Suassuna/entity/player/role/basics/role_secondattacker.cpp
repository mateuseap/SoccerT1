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

#include "role_secondattacker.h"

QString Role_SecondAttacker::name(){
    return "Role_SecondAttacker";
}

Role_SecondAttacker::Role_SecondAttacker() {
}

void Role_SecondAttacker::initializeBehaviours(){
    // Aqui são inseridos os behaviours possíveis de serem usados
    // na ordem: ID do behaviour, instanciação dele

    usesBehaviour(BHV_MARKBALL, _bh_markB = new Behaviour_MarkBall());
    usesBehaviour(BHV_MARKPLAYER, _bh_markP = new Behaviour_MarkPlayer());

    usesBehaviour(BHV_RECEIVER, _bh_rec = new Behaviour_Receiver());
    usesBehaviour(BHV_ATTACKER, _bh_atk = new Behaviour_Attacker());

    std::cout << getActualBehaviour() << std::endl;
}

void Role_SecondAttacker::configure(){
    // Aqui são setados parametros que devem ser configurados
    setBehaviour(BHV_RECEIVER);

}

int Role_SecondAttacker::blokeado(){
    for(quint8 i=0;i<6;i++){
        if(PlayerBus::theirPlayerAvailable(i)){
            if(PlayerBus::theirPlayer(i)->position().x()>=(-3.25) && PlayerBus::theirPlayer(i)->position().x()<= 0.00 ){
                return i;
            }
        }
    }
}

bool Role_SecondAttacker::teamHasPossession(){
    for( quint8 i=0;i<6;i++){
        if(PlayerBus::ourPlayerAvailable(i)){
            if (PlayerBus::ourPlayer(i)->hasBallPossession()){
                return true;
            }
        }
    }
    return false;
}
bool Role_SecondAttacker::otherTeamHasPossession(){
    for( quint8 i=0;i<6;i++){
        if(PlayerBus::theirPlayerAvailable(i)){
            if (PlayerBus::theirPlayer(i)->hasBallPossession()){
                return true;
            }
        }
    }
    return false;
}
bool Role_SecondAttacker::otherTeamHasMidPossession(){
    for( quint8 i=0;i<6;i++){
        if(PlayerBus::theirPlayerAvailable(i)){
            if (PlayerBus::theirPlayer(i)->hasBallPossession()){
                if(loc()->ball().x()>=(-3.25) && loc()->ball().x()<=0.00){
                    return true;
                }
            }
        }
    }
    return false;
}

bool Role_SecondAttacker::offensiveMidNotEmpty(){
    for( quint8 i=0;i<6;i++){
        if(PlayerBus::theirPlayerAvailable(i)){
            if(PlayerBus::theirPlayer(i)->position().x()>=(-3.25) && PlayerBus::theirPlayer(i)->position().x()<= 0.00){
                return true;
            }
        }
    }
    return false;
}

bool Role_SecondAttacker::offensiveMidNoBall(){
    for( quint8 i=0;i<6;i++){
        if(PlayerBus::theirPlayerAvailable(i)){
            if(PlayerBus::theirPlayer(i)->position().x()>=(-3.25) && PlayerBus::theirPlayer(i)->position().x()<= 0.00){
                if(!otherTeamHasMidPossession()){
                    return true;
                }
            }
        }
    }
    return false;
}


void Role_SecondAttacker::run(){
    /*
     * Aqui devem ocorrer os sets de parametros de acordo com o behaviour
     * que estiver sendo executado, de preferencia declare todos os parametros
     * na classe da role, sete-os aqui e envie para o behaviour (usando as funções
     * set presentes neles)
    */

    ballfreemid = 0;

    if(loc()->ball().x()>=(-3.25) && loc()->ball().x()<=0.00){
        if(!(otherTeamHasPossession() && teamHasPossession())){
            ballfreemid = 1;
        }
    }

    switch(getActualBehaviour()){
    case BHV_ATTACKER:{
        if(!(player()->hasBallPossession())){
            setBehaviour(BHV_RECEIVER);
        }
    }
    break;
    case BHV_MARKBALL:{
        if(player()->hasBallPossession()){
            setBehaviour(BHV_ATTACKER);
        }else if(teamHasPossession()){
            setBehaviour(BHV_RECEIVER);
        }
    }
    break;
    case BHV_MARKPLAYER:{
        _bh_markP->setTargetID(blokeado());
        if(offensiveMidNotEmpty()){
            setBehaviour(BHV_RECEIVER);
            //_bhv_rec-
        }else if(loc()->ball().x()>=(-3.25) && loc()->ball().x()<= 0.00){
            setBehaviour(BHV_MARKBALL);
        }
    }
    break;
    case BHV_RECEIVER:{
        //SEMPRE ENTRA NO MARKPLAYER,ACHAR UMA FORMA DE CHECAR BEHAVIOURS DE OUTROS ROBOS
        if(otherTeamHasMidPossession() || ballfreemid == 1){
            setBehaviour(BHV_MARKBALL);
        }else if(offensiveMidNoBall()){
            setBehaviour(BHV_MARKPLAYER);
        }else if(player()->hasBallPossession()){
            setBehaviour(BHV_ATTACKER);
        }
        }
    }


/*
    switch(getActualBehaviour()){
    case BHV_ATTACKER:{
        //_bh_atk->
    }
    break;
    case BHV_MARKBALL:{
        //_bh_markB->
    }
    break;
    case BHV_MARKPLAYER:{
        //_bh_markP->setTargetID(blokeado());
    }
    break;
    case BHV_RECEIVER:{
        //SEMPRE ENTRA NO MARKPLAYER,ACHAR UMA FORMA DE CHECAR BEHAVIOURS DE OUTROS ROBOS
        //_bh_rec->
    }
    }
   */

    /*
    if(teste != getActualBehaviour()){
        if(teste ==0){
            std::cout << "BHV_MARKBALL" << std::endl;
        }else if(teste == 1){
            std::cout << "BHV_MARKPLAYER" << std::endl;
        }else if(teste == 2){
            std::cout << "BHV_RECEIVER" << std::endl;
        }else if(teste == 3){
            std::cout << "BHV_ATTACKER" << std::endl;
        }
        teste = getActualBehaviour();
    }
    */
    /*teste = getActualBehaviour();
    if(teste ==0){
        std::cout << "BHV_MARKBALL" << std::endl;
    }else if(teste == 1){
        std::cout << "BHV_MARKPLAYER" << std::endl;
    }else if(teste == 2){
        std::cout << "BHV_RECEIVER" << std::endl;
    }else if(teste == 3){
        std::cout << "BHV_ATTACKER" << std::endl;
    }*/
}
