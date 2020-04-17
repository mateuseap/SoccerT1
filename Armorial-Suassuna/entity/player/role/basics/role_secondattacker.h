/***
 * Maracatronics Robotics
 * Federal University of Pernambuco (UFPE) at Recife
 * http://www.maracatronics.com/
 *
 * This file is part of Armorial project.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***/

#ifndef ROLE_SECONDATTACKER_H
#define ROLE_SECONDATTACKER_H

#include <entity/player/behaviour/mrcbehaviours.h>
#include <entity/player/role/role.h>

class Role_SecondAttacker : public Role
{
private:
    bool ballfreemid;
    int teste = 99;

    // Behaviours
    Behaviour_MarkPlayer *_bh_markP;
    Behaviour_MarkBall *_bh_markB;

    Behaviour_Receiver *_bh_rec;
    Behaviour_Attacker *_bh_atk;

    // Behaviours ids!
    enum{
        BHV_MARKBALL,
        BHV_MARKPLAYER,
        BHV_RECEIVER,
        BHV_ATTACKER
    };

    // Inherited functions
    void configure();
    void run();

    // Mutex
    QMutex _mutex;

public:
    Role_SecondAttacker();
    void initializeBehaviours();
    QString name();
    int blokeado();
    bool otherTeamHasMidPossession();
    bool teamHasPossession();
    bool offensiveMidNoBall();
    bool otherTeamHasPossession();
    bool offensiveMidNotEmpty();
};


#endif // ROLE_SECONDATTACKER_H
