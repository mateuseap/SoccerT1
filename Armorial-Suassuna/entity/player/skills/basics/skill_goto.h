#ifndef SKILL_GOTO_H
#define SKILL_GOTO_H

#include <entity/player/skills/skill.h>

class Skill_GoTo : public Skill {
private:
    void run();
    Position _desiredPosition;
    double _offsetBall;
public:
    Skill_GoTo();
    QString name();

    void setDesiredPosition(Position pos) { _desiredPosition = pos; }
    void setOffsetToBall(double value) { _offsetBall = value; }
};

#endif // SKILL_GOTO_H
