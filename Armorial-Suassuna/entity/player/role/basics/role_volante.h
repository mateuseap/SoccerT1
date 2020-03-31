#ifndef ROLE_VOLANTE_H
#define ROLE_VOLANTE_H

#include <entity/player/behaviour/mrcbehaviours.h>
//#include <entity/contromodule/mrcteam.h>
#include <entity/player/role/role.h>

class Role_Volante : public Role
{
private:
    //MRCTeam *_checkInfo;
    quint8 _tgID;
    int _state;
    int _opPinOurF;
    bool _ourFisSafe;

    // Behaviours
    Behaviour_AreaCleaner *_bh_areaCleaner;
    Behaviour_MarkBall *_bh_markBall;
    Behaviour_MarkPlayer *_bh_markPlayer;
    Behaviour_Barrier *_bh_barrier;
    Behaviour_DoNothing *_bh_doNothing;

    // Behaviours ids!
    enum{
        BHV_AREACLEANER,    //0
        BHV_MARKPLAYER,     //1
        BHV_MARKBALL,       //2
        BHV_BARRIER,        //3
        BHV_DONOTHING       //4
    };

    // Inherited functions
    void configure();
    void run();

    // Mutex
    QMutex _mutex;

public:
    Role_Volante();
    void initializeBehaviours();
    QString name();
};

#endif // ROLE_VOLANTE_H
