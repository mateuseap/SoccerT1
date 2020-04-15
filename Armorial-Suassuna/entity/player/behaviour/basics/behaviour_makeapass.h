#ifndef BEHAVIOUR_MAKEAPASS_H
#define BEHAVIOUR_MAKEAPASS_H

#include <entity/player/behaviour/behaviour.h>
#include <entity/player/skills/skills_include.h>

class Behaviour_MakeAPass : public Behaviour {
private:
    quint8 _nearestPlayerID;
    float _actualDistance;
    int _state;

    // Skills
    Skill_GoToLookTo *_skill_goToLookTo;
    Skill_Kick *_skill_kick;

    // Skills ID's
    enum{
        STATE_GOTOLOOKTO,   //0
        STATE_KICK          //1
    };

    // Inherited functions
    void configure();
    void run();

    // Additional functions
    void nearestPlayer();
public:
    Behaviour_MakeAPass();
    QString name();
};

#endif // BEHAVIOUR_MAKEAPASS_H
