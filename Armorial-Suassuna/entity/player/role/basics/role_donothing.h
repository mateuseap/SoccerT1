#ifndef ROLE_DONOTHING_H
#define ROLE_DONOTHING_H

#include <entity/player/behaviour/mrcbehaviours.h>
#include <entity/player/role/role.h>

class Role_DoNothing : public Role
{
private:
    // Behaviours
    Behaviour_DoNothing *_bh_doNothing;

    // Behaviours ID's
    enum{
        BHV_DONOTHING       //0
    };

    // Inherited functions
    void configure();
    void run();

    // Mutex
    QMutex _mutex;
public:
    Role_DoNothing();
    void initializeBehaviours();
    QString name();
};

#endif // ROLE_DONOTHING_H
