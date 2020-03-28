#ifndef BEHAVIOUR_AREACLEANER_H
#define BEHAVIOUR_AREACLEANER_H

#include <entity/player/behaviour/behaviour.h>
#include <entity/player/skills/skills_include.h>

class Behaviour_AreaCleaner : public Behaviour {
private:
    void configure();
    void run();
    int _state;

    Skill_GoToLookTo *_skill_goToLookTo;
    Skill_Kick *_skill_kick;

    enum{
        STATE_GOTOLOOKTO,
        STATE_KICK
    };
public:
    Behaviour_AreaCleaner();
    QString name();
};

#endif // BEHAVIOUR_AREACLEANER_H
