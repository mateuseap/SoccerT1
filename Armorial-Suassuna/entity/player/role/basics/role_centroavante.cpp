#include "role_centroavante.h"

QString Role_CentroAvante::name(){
    return "Role_CentroAvante";
}

Role_CentroAvante::Role_CentroAvante() {
}

void Role_CentroAvante::initializeBehaviours(){
    // Aqui são inseridos os behaviours possíveis de serem usados
    // na ordem: ID do behaviour, instanciação dele
   usesBehaviour(BHV_RECEIVER, _bh_re = new Behaviour_Receiver());
   usesBehaviour(BHV_MARKBALL, _bh_mb = new Behaviour_MarkBall());
   usesBehaviour(BHV_ATTACKER, _bh_at = new Behaviour_Attacker());
   usesBehaviour(BHV_DONOTHING, _bh_dn = new Behaviour_DoNothing());
}

void Role_CentroAvante::configure(){
    //Controle dos estados
    _actualState = BHV_RECEIVER;
    _beforeState = 200;
    setBehaviour(BHV_RECEIVER);

}

void Role_CentroAvante::run(){
    /*
     * Aqui devem ocorrer os sets de parametros de acordo com o behaviour
     * que estiver sendo executado, de preferencia declare todos os parametros
     * na classe da role, sete-os aqui e envie para o behaviour (usando as funções
     * set presentes neles)
    */

    //Posse de bola, lembrar de criar função!!
    _ourTeamHasBall = false;
    _theirTeamHasBall = false;
    _ativarMarkBall = true;

    for(quint8 i = 0; i < 6; i++){
        if(PlayerBus::theirPlayerAvailable(i)){
            if(PlayerBus::theirPlayer(i)->hasBallPossession()){
                _theirTeamHasBall = true;
                break;
            }
        }
        if(PlayerBus::ourPlayerAvailable(i)){
            if(PlayerBus::ourPlayer(i)->hasBallPossession()){
                _ourTeamHasBall = true;
                break;
            }
        }
    }

    //Avaliar o jogador mais proximo da bola // encapsular
    //if(_ourTeamHasBall==false && _theirTeamHasBall == false){
        for(quint8 i = 0; i < 6; i++){
            if(PlayerBus::ourPlayerAvailable(i)){
                _distMin[i] = WR::Utils::distance(loc()->ball(), PlayerBus::ourPlayer(i)->position());
            }
            else{
                _distMin[i] = 1000;
            }
        }
    //}


    switch(getActualBehaviour()){
    case BHV_RECEIVER:{

       _actualState = getActualBehaviour();

       //Posição
       attackerID = 0;
       _bh_re->setAttackerId(attackerID);

       //if (attackerID < 7){
           _bh_re->setAttackerId(attackerID);
           _bh_re->setFollowAttacker(true);
       //}
       //Attacker
       _distCentroavante = WR::Utils::distance(loc()->ball(),player()->position());
       if(_distCentroavante <= 0.15){
           setBehaviour(BHV_ATTACKER);
       }

       //MarkBall
       //if(_ourTeamHasBall==false && _theirTeamHasBall == false){

           _distCentroavante = WR::Utils::distance(loc()->ball(),player()->position());
           for(int i = 0; i <6 ; i++){
               if(_distCentroavante > _distMin[i]){
                   _ativarMarkBall = false;
                   break;
               }
           }
           if(_ativarMarkBall == true){
               setBehaviour(BHV_MARKBALL);
           }
       //}
    }
    break;

    case BHV_ATTACKER:{

        attackerID = player()->playerId();

        _actualState = getActualBehaviour();

       //Receiver
        _distCentroavante = WR::Utils::distance(loc()->ball(),player()->position());
        if(_distCentroavante > 0.3){
            setBehaviour(BHV_RECEIVER);
            attackerID = 100;
        }
    }
    break;
    case BHV_MARKBALL:{

        _actualState = getActualBehaviour();
        attackerID = player()->playerId();
        _bh_re->setAttackerId(attackerID);

       //Receiver
      /*if(_theirTeamHasBall)
           setBehaviour(BHV_RECEIVER);
       }*/

       //Attacker
       _distCentroavante = WR::Utils::distance(loc()->ball(),player()->position());
       if(_distCentroavante <= 0.2){
           setBehaviour(BHV_ATTACKER);
       }
    }
    break;

    }

    //Printar o behaviours atual
    if(_actualState != _beforeState){

        actualPayer = player()->playerId();
        if(_actualState == BHV_RECEIVER){ std::cout<<"\n Behaviour Receiver - PlayerId:"<< actualPayer<<std::endl; }
        if(_actualState == BHV_MARKBALL){ std::cout<<"\n Behaviour MarkBall - PlayerId:"<< actualPayer<<std::endl; }
        if(_actualState == BHV_ATTACKER){ std::cout<<"\n Behaviour Attacker - PlayerId:"<< actualPayer<<std::endl; }
        if(_actualState == BHV_DONOTHING){ std::cout<<"\n Behaviour Donothing - PlayerId:"<< actualPayer<<std::endl; }
         _beforeState = _actualState;
    }


}
