#ifndef ROLE_DEFAULT_H
#define ROLE_DEFAULT_H

#include <entity/player/behaviour/mrcbehaviours.h>
#include <entity/player/role/role.h>

class Role_Default : public Role
{
private:
    // Behaviours
    Behaviour_AreaCleaner *_bh_areaCleaner;

    // Behaviours ids!
    enum{
        BHV_AREACLEANER
    };

    // Inherited functions
    void configure();
    void run();

    // Mutex
    QMutex _mutex;

public:
    Role_Default();
    void initializeBehaviours();
    QString name();
};

#endif // ROLE_DEFAULT_H
