#ifndef BEHAVIOUR_AREACLEANER_H
#define BEHAVIOUR_AREACLEANER_H

#include <entity/player/behaviour/behaviour.h>
#include <entity/player/skills/skills_include.h>

class Behaviour_AreaCleaner : public Behaviour {
private:
    quint8 _nearestPlayerID;
    int _state;

    // Skills
    Skill_GoToLookTo *_skill_goToLookTo;
    Skill_Kick *_skill_kick;

    // Skills ID's
    enum{
        STATE_GOTOLOOKTO,
        STATE_KICK
    };

    // Inherited functions
    void configure();
    void run();

    // Additional functions
    void nearestPlayer();
public:
    Behaviour_AreaCleaner();
    QString name();
};

#endif // BEHAVIOUR_AREACLEANER_H
