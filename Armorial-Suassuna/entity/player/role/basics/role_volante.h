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

#ifndef ROLE_VOLANTE_H
#define ROLE_VOLANTE_H

#include <entity/player/behaviour/mrcbehaviours.h>
#include <entity/player/role/role.h>

class Role_Volante : public Role
{
    Q_OBJECT
private:
    QString _information;
    quint8 _targetID, _playerHasBall, _zagueiroID;
    int _state, _opponentPlayerInOurField;
    bool _ourFieldIsSafe, _theirTeamHasBall, _ourTeamHasBall, _nobodyHasBall;
    float _error;
    std::string _zagueiroStatus;

    // Behaviours
    Behaviour_MarkBall *_bh_markBall;
    Behaviour_MarkPlayer *_bh_markPlayer;
    Behaviour_Barrier *_bh_barrier;
    Behaviour_MakeAPass *_bh_makeAPass;

    // Behaviours ID's
    enum{
        BHV_MARKPLAYER,     //0
        BHV_MARKBALL,       //1
        BHV_BARRIER,        //2
        BHV_MAKEAPASS       //3
    };

    // Inherited functions
    void configure();
    void run();

    // Mutex
    QMutex _mutex;

    // Additional functions
    void whoHasBallPossession();
    void checkPlayerInsideOurField();
    void checkPlayersRoles();
    void changeInformationStatus(QString mode);

public:
    Role_Volante();
    void initializeBehaviours();
    QString name();
    QString information();

public slots:
    void receiveZagueiroInformation(std::string information);

signals:
    void sendVolanteInformation(std::string information);
};

#endif // ROLE_VOLANTE_H
