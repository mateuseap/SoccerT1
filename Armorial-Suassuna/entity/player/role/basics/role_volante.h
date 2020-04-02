#ifndef ROLE_VOLANTE_H
#define ROLE_VOLANTE_H

#include <entity/player/behaviour/mrcbehaviours.h>
#include <entity/player/role/role.h>

class Role_Volante : public Role
{
private:
    quint8 _targetID;
    quint8 _playerHasBall;
    int _state;
    int _opponentPlayerInOurField;
    bool _ourFieldIsSafe;
    bool _ourTeamHasBall;
    bool _theirTeamHasBall;

    // Behaviours
    Behaviour_AreaCleaner *_bh_areaCleaner;
    Behaviour_MarkBall *_bh_markBall;
    Behaviour_MarkPlayer *_bh_markPlayer;
    Behaviour_Barrier *_bh_barrier;
    Behaviour_DoNothing *_bh_doNothing;
    Behaviour_FollowBall *_bh_followBall;

    // Behaviours ID's
    enum{
        BHV_AREACLEANER,    //0
        BHV_MARKPLAYER,     //1
        BHV_MARKBALL,       //2
        BHV_BARRIER,        //3
        BHV_DONOTHING,      //4
        BHV_FOLLOWBALL      //5
    };

    // Inherited functions
    void configure();
    void run();

    // Mutex
    QMutex _mutex;

    // Additional functions
    void whoHasBallPossession();
    void checkPlayerInsideOurField();
public:
    Role_Volante();
    void initializeBehaviours();
    QString name();
};

#endif // ROLE_VOLANTE_H
