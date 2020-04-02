#ifndef PLAYBOOK_DONOTHING_H
#define PLAYBOOK_DONOTHING_H

#include <entity/contromodule/playbook/playbook.h>
#include <entity/player/role/mrcroles.h>

class Playbook_DoNothing : public Playbook {
private:
    // Roles
    QList<Role_Default*> _rl_default;
    QList<Role_DoNothing*> _rl_doNothing;
    //QList<Role_Volante*> _rl_volante;

    void configure(int numPlayers);
    void run(int numPlayers);
    int maxNumPlayer();
public:
    Playbook_DoNothing();
    QString name();
};

#endif // PLAYBOOK_DONOTHING_H
