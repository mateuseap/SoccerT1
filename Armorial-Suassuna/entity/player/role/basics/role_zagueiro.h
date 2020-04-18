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

#ifndef ROLE_ZAGUEIRO_H
#define ROLE_ZAGUEIRO_H

#include <entity/player/behaviour/mrcbehaviours.h>
#include <entity/player/role/role.h>

class Role_Zagueiro : public Role
{
    Q_OBJECT
private:
    QString _information;
    quint8 _targetID, _playerHasBall, _volanteID, _nearestOpponentToOurGoalID;
    int _state, _opponentPlayerInOurField;
    bool _ourFieldIsSafe, _theirTeamHasBall, _ourTeamHasBall, _nobodyHasBall, _criticZoneIsSafe;
    float _distanceToOurGoal, _criticArea;
    std::string _volanteStatus;

    // Behaviours
    Behaviour_Barrier *_bh_barrier;
    Behaviour_MakeAPass *_bh_makeAPass;
    Behaviour_MarkPlayer *_bh_markPlayer;

    // Behaviours ID's
    enum{
        BHV_BARRIER,       //0
        BHV_MAKEAPASS,     //1
        BHV_MARKPLAYER     //2
    };

    // Inherited functions
    void configure();
    void run();

    // Mutex
    QMutex _mutex;

    // Additional functions
    void whoHasBallPossession();
    void checkPlayersRoles();
    void nearestOpponentToOurGoal();
    void checkCriticZone();
    void checkPlayerInsideOurField();

public:
    Role_Zagueiro();
    void initializeBehaviours();
    QString name();
    QString information();

public slots:
    void receiveVolanteInformation(std::string information);

signals:
    void sendZagueiroInformation(std::string information);
};

#endif // ROLE_ZAGUEIRO_H


