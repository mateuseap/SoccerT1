#ifndef ROLE_CENTROAVANTE_H
#define ROLE_CENTROAVANTE_H

#include <entity/player/behaviour/mrcbehaviours.h>
#include <entity/player/role/role.h>
#include <entity/player/behaviour/basics/behaviour_receiver.h>

class Role_CentroAvante : public Role
{
private:
    //Variaveis
    quint8 _targetID;
    quint8 _playerHasBall;
    int _state;
    int _opponentPlayerInOurField;
    bool _ourFieldIsSafe;
    bool _ourTeamHasBall;
    bool _theirTeamHasBall;
    Position _ballPosition;
    Position _centroavantePosition;
    Position _segAtacantePosition;
    Position _volantePosition;
    float _distMin[6];
    float _distCentroavante;
    bool  _ativarMarkBall;
    int _actualState;
    int _beforeState;
    int actualPayer;
    quint8 attackerID;


    // Behaviours
    Behaviour_Receiver *_bh_re;
    Behaviour_MarkBall *_bh_mb;
    Behaviour_Attacker *_bh_at;
    Behaviour_DoNothing *_bh_dn;

    // Behaviours ids!
    enum{
        BHV_RECEIVER,       //0
        BHV_MARKBALL,       //1
        BHV_ATTACKER,       //2
        BHV_DONOTHING       //3
    };

    // Inherited functions
    void configure();
    void run();

    // Mutex
    QMutex _mutex;

    //Time com posse de bola
    void teamHasBallPossession();

public:
    Role_CentroAvante();
    void initializeBehaviours();
    QString name();
};

#endif // ROLE_CENTROAVANTE_H
