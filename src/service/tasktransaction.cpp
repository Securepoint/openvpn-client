#include "tasktransaction.h"
#include "srvcli.h"

Transaction * const EndTransaction = 0;

TaskTransaction::TaskTransaction ()
    : command (""),
      params ("")
{
}

void TaskTransaction::apply()
{    
    // Hier kommt das Senden rein
    if (command.isEmpty()) {
        return;
    }

    SrvCLI::instance()->send(this->command, this->params);
    // Kurz warten
    qSleep(50);
}

void TaskTransaction::setCommand(const QString &com)
{
    this->command = com;
}

void TaskTransaction::setParams(const QString &parm)
{
    this->params = parm;
}


