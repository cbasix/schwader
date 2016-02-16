//
// Created by cyberxix on 16.02.16.
//

#ifndef GEARRAKE_CONSTANTS_MESSAGE_H
#define GEARRAKE_CONSTANTS_MESSAGE_H

enum message_types{
    MESSAGE_VALUE_CHANGED_SENSOR,
    MESSAGE_VALUE_CHANGED_MANUAL,
    MESSAGE_POSITION_CHANGED,
    MESSAGE_TIMEOUT_OCCURED,
    MESSAGE_TIMER_OVER,
    MESSAGE_ACTION_STATE,
    MESSAGE_OUTPUT

};

static enum ActionState {
    ACTION_STATE_STARTING,
    ACTION_STATE_STARTED,
    ACTION_STATE_STOPPING,
    ACTION_STATE_STOPPED_OK
};

#endif //GEARRAKE_CONSTANTS_MESSAGE_H
