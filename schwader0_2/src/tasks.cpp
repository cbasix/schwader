#include "Arduino.h"
#include "task_management.h"
#include "tasks.h"

//MINIMAL TASK
/*MinimalTask::MinimalTask() {
	task_state = STATE_STOPPED;
	task_id = TSK_...;
}

void MinimalTask::start() {
	Task::start();

	tm->outp->setOutput(OUT_..., ACTIVE);

}

void MinimalTask::update(InputEventData *inp) {
 if (end) {
		exit();
	}
}

void MinimalTask::timer() {}

void MinimalTask::exit() {
	Task::exit();

	tm->outp->setOutput(OUT_..., INACTIVE);
}*/

//OTHER TASKS------------------------------------------

BothSpinnerUpTask::BothSpinnerUpTask() {
	task_state = STATE_STOPPED;
	task_id = TSK_SPINNER_BOTH_UP;
	spinner_left_is_done = false;
	spinner_right_is_done = false;
}

void BothSpinnerUpTask::testStartConditions(InputEventData* inp){
	if (inp->input_type == TYPE_MANUAL
			&& (inp->input_id == IN_SPINNER_LEFT_UP || inp->input_id == IN_SPINNER_RIGHT_UP)
			&& inp->input_value == ACTIVE
			&& tm->inp->getInputState(IN_SPINNER_LEFT_UP) == ACTIVE
			&& tm->inp->getInputState(IN_SPINNER_RIGHT_UP)  == ACTIVE) {

		tm->stopTask(TSK_SPINNER_RIGHT_UP);
		tm->stopTask(TSK_SPINNER_LEFT_UP);
		tm->startTask(TSK_SPINNER_BOTH_UP);
	}
}

void BothSpinnerUpTask::start() {
	Task::start();
	spinner_left_is_done = false;
	spinner_right_is_done = false;

	tm->outp->setCylinder(OUT_SPINNER_LEFT_UP, OUT_SPINNER_LEFT_FLOAT, CYLINDER_MOVE_OUT);
	tm->outp->setCylinder(OUT_SPINNER_RIGHT_UP, OUT_SPINNER_RIGHT_FLOAT, CYLINDER_MOVE_OUT);
}

void BothSpinnerUpTask::update(InputEventData *inp) {
	if (inp->input_id == SENS_SPINNER_LEFT_THIRD && inp->input_value == ACTIVE) {
		spinner_left_is_done = true;
		tm->outp->setCylinder(OUT_SPINNER_LEFT_UP, OUT_SPINNER_LEFT_FLOAT, CYLINDER_HOLD);

	} else if (inp->input_id == SENS_SPINNER_RIGHT_THIRD
			&& inp->input_value == ACTIVE) {
		spinner_right_is_done = true;
		tm->outp->setCylinder(OUT_SPINNER_RIGHT_UP, OUT_SPINNER_RIGHT_FLOAT, CYLINDER_HOLD);
	}

	if (spinner_left_is_done && spinner_right_is_done) {
		exit();
	}
}

void BothSpinnerUpTask::timer() {}

void BothSpinnerUpTask::exit() {
	Task::exit();

	tm->outp->setCylinder(OUT_SPINNER_LEFT_UP, OUT_SPINNER_LEFT_FLOAT, CYLINDER_HOLD);
	tm->outp->setCylinder(OUT_SPINNER_RIGHT_UP, OUT_SPINNER_RIGHT_FLOAT, CYLINDER_HOLD);

}

// Left Spinner up ----------------------------------------------------
LeftSpinnerUpTask::LeftSpinnerUpTask() {
	task_state = STATE_STOPPED;
	task_id = TSK_SPINNER_LEFT_UP;
}

void LeftSpinnerUpTask::testStartConditions(InputEventData *inp){
	//normal start joystick move
	if (inp->input_type == TYPE_MANUAL
			&& inp->input_id == IN_SPINNER_LEFT_UP
			&& inp->input_value == ACTIVE
			&& !tm->getTaskStatus(TSK_SPINNER_BOTH_UP) == STATE_RUNNING) {

		start();

	//special start coming from TSK_SPINNER_BOTH_UP released one joystick
	} else if (tm->getTaskStatus(TSK_SPINNER_BOTH_UP) == STATE_RUNNING
			&& inp->input_id == IN_SPINNER_RIGHT_UP
			&& inp->input_value == INACTIVE
			&& inp->input_type == TYPE_MANUAL) {

		tm->stopTask(TSK_SPINNER_BOTH_UP);
		start();

	}

}

void LeftSpinnerUpTask::start() {
	Task::start();

	//Serial.println("LeftSpinnerUpTask::start()");

	tm->outp->setCylinder(OUT_SPINNER_LEFT_UP, OUT_SPINNER_LEFT_FLOAT, CYLINDER_MOVE_OUT);
}

void LeftSpinnerUpTask::update(InputEventData *inp) {
	if (inp->input_id == SENS_SPINNER_LEFT_UP
			&& inp->input_value == ACTIVE) {
		exit();

	} else if (inp->input_id == IN_SPINNER_LEFT_UP
			&& inp->input_value == INACTIVE) {
		exit();
	}
}

void LeftSpinnerUpTask::timer() {}

void LeftSpinnerUpTask::exit() {
	Task::exit();

	//Serial.println("LeftSpinnerUpTask::exit()");

	tm->outp->setCylinder(OUT_SPINNER_LEFT_UP, OUT_SPINNER_LEFT_FLOAT, CYLINDER_HOLD);
}


RightSpinnerUpTask::RightSpinnerUpTask() {
	task_state = STATE_STOPPED;
	task_id = TSK_SPINNER_RIGHT_UP;
}

void RightSpinnerUpTask::testStartConditions(InputEventData *inp){
	//normal start joystick move
	if (inp->input_type == TYPE_MANUAL
			&& inp->input_id == IN_SPINNER_RIGHT_UP
			&& inp->input_value == ACTIVE
			&& !tm->getTaskStatus(TSK_SPINNER_BOTH_UP) == STATE_RUNNING) {

		start();

	//special start coming from TSK_SPINNER_BOTH_UP released one joystick
	} else if (tm->getTaskStatus(TSK_SPINNER_BOTH_UP) == STATE_RUNNING
			&& inp->input_id == IN_SPINNER_LEFT_UP
			&& inp->input_value == INACTIVE
			&& inp->input_type == TYPE_MANUAL) {

		tm->stopTask(TSK_SPINNER_BOTH_UP);
		start();

	}

}

void RightSpinnerUpTask::start() {
	Task::start();

	tm->outp->setCylinder(OUT_SPINNER_RIGHT_UP, OUT_SPINNER_RIGHT_FLOAT, CYLINDER_MOVE_OUT);

}

void RightSpinnerUpTask::update(InputEventData *inp) {
	if (inp->input_id == SENS_SPINNER_RIGHT_UP
			&& inp->input_value == ACTIVE) {
		exit();

	} else if (inp->input_id == IN_SPINNER_RIGHT_UP
			&& inp->input_value == INACTIVE) {
		exit();
	}
}

void RightSpinnerUpTask::timer() {}

void RightSpinnerUpTask::exit() {
	Task::exit();

	tm->outp->setCylinder(OUT_SPINNER_RIGHT_UP, OUT_SPINNER_RIGHT_FLOAT, CYLINDER_HOLD);
}


LeftSpinnerFloatTask::LeftSpinnerFloatTask() {
	task_state = STATE_STOPPED;
	task_id = TSK_SPINNER_LEFT_FLOAT;
}

void LeftSpinnerFloatTask::testStartConditions(InputEventData* inp){
	//normal start joystick move
	if (inp->input_id == IN_SPINNER_LEFT_FLOAT
			&& inp->input_value == ACTIVE
			&& inp->input_type == TYPE_MANUAL) {

		start();
	}
}

void LeftSpinnerFloatTask::start() {
	Task::start();
//	Serial.println("LeftSpinnerFloatTask::start()");
	tm->outp->setCylinder(OUT_SPINNER_LEFT_UP, OUT_SPINNER_LEFT_FLOAT, CYLINDER_MOVE_IN_OR_FLOAT);
}

void LeftSpinnerFloatTask::update(InputEventData *inp) {
	exit();
}

void LeftSpinnerFloatTask::timer() {}

void LeftSpinnerFloatTask::exit() {
	Task::exit();
}


RightSpinnerFloatTask::RightSpinnerFloatTask() {
	task_state = STATE_STOPPED;
	task_id = TSK_SPINNER_RIGHT_FLOAT;
}

void RightSpinnerFloatTask::testStartConditions(InputEventData* inp){
	//normal start joystick move
	if (inp->input_id == IN_SPINNER_RIGHT_FLOAT
			&& inp->input_value == ACTIVE
			&& inp->input_type == TYPE_MANUAL) {

		start();
	}
}

void RightSpinnerFloatTask::start() {
	Task::start();

	tm->outp->setCylinder(OUT_SPINNER_RIGHT_UP, OUT_SPINNER_RIGHT_FLOAT, CYLINDER_MOVE_IN_OR_FLOAT);
}

void RightSpinnerFloatTask::update(InputEventData *inp) {
	exit();
}

void RightSpinnerFloatTask::timer() {}

void RightSpinnerFloatTask::exit() {
	Task::exit();
}


//---------------------------------END TASKS-------------------------------------
//LED Task muss ganz am ende der Tasklist stehen, sodass er die isOutputChanging() Funktion nutzen kann.

LedTask::LedTask() {
	task_state = STATE_STOPPED;
	task_id = TSK_LED;


}

void LedTask::testStartConditions(InputEventData* inp){
	//Serial.println(" LedTask::testStartConditions()");

	if(inp->input_type == TYPE_MESSAGE
			&& inp->input_id == MESSAGE_STARTUP){
		start();
	}
}

void LedTask::start() {
	Task::start();
	//Serial.println(" LedTask::start()");
}

void LedTask::update(InputEventData *inp) {
	//Serial.println(" LedTask::update()");
	// LED Schwimmstellung rechts
	if (tm->outp->isOutputChanging(OUT_SPINNER_RIGHT_FLOAT)){
		//Serial.println(" LedTask::update OutputChanging(OUT_SPINNER_RIGHT_FLOAT)");

		if(tm->outp->getOutputState(OUT_SPINNER_RIGHT_FLOAT) == ACTIVE) {

			tm->outp->setLed(LED_SPINNER_RIGHT_FLOAT, ACTIVE);
			//Serial.println(" LedTask::update OutputChanging(OUT_SPINNER_RIGHT_FLOAT) ACTIVE");
		} else {

			tm->outp->setLed(LED_SPINNER_RIGHT_FLOAT, INACTIVE);
			//Serial.println(" LedTask::update OutputChanging(OUT_SPINNER_RIGHT_FLOAT) INACTIVE");
		}
	}

	//LED Schwimmstellung links
	if (tm->outp->isOutputChanging(OUT_SPINNER_LEFT_FLOAT)){
		//Serial.println(" LedTask::update OutputChanging(OUT_SPINNER_LEFT_FLOAT)");

		if (tm->outp->getOutputState(OUT_SPINNER_LEFT_FLOAT) == ACTIVE) {

			//Serial.println(" LedTask::update OutputChanging(OUT_SPINNER_LEFT_FLOAT) ACTIVE");
			tm->outp->setLed(LED_SPINNER_LEFT_FLOAT, ACTIVE);

		} else {
			//Serial.println(" LedTask::update OutputChanging(OUT_SPINNER_LEFT_FLOAT) INACTIVE");
			tm->outp->setLed(LED_SPINNER_LEFT_FLOAT, INACTIVE);

		}
	}
}

void LedTask::timer() {}

void LedTask::exit() {
	Task::exit();

	tm->outp->setLed(LED_SPINNER_LEFT_FLOAT, INACTIVE);
	tm->outp->setLed(LED_SPINNER_LEFT_FLOAT, INACTIVE);
}

//Pressure Task muss ganz am ende der Tasklist stehen, sodass er die isOutputChanging() Funktion nutzen kann.

PressureTask::PressureTask() {
	task_state = STATE_STOPPED;
	task_id = TSK_PRESSURE;


}

void PressureTask::testStartConditions(InputEventData* inp){
	//Serial.println(" LedTask::testStartConditions()");

	if(inp->input_type == TYPE_MESSAGE
			&& inp->input_id == MESSAGE_STARTUP){
		start();
	}
}

void PressureTask::start() {
	Task::start();
	//Serial.println(" LedTask::start()");
}

void PressureTask::update(InputEventData *inp) {
	//Serial.println(" LedTask::update()");
	// LED Schwimmstellung rechts
	if (tm->outp->isOutputChanging(OUT_SPINNER_RIGHT_UP)
			|| tm->outp->isOutputChanging(OUT_SPINNER_LEFT_UP)
			|| tm->outp->isOutputChanging(OUT_SPINNER_REAR_UP)
			|| tm->outp->isOutputChanging(OUT_SPINNER_RIGHT_TELE_OUT)
			|| tm->outp->isOutputChanging(OUT_SPINNER_LEFT_TELE_OUT)
			|| tm->outp->isOutputChanging(OUT_SPINNER_RIGHT_TELE_IN)
			|| tm->outp->isOutputChanging(OUT_SPINNER_LEFT_TELE_IN)
			|| tm->outp->isOutputChanging(OUT_FRAME_UP)
			|| tm->outp->isOutputChanging(OUT_FRAME_DOWN)
			|| tm->outp->isOutputChanging(OUT_STEER_LEFT)
			|| tm->outp->isOutputChanging(OUT_STEER_RIGHT)
			|| tm->outp->isOutputChanging(OUT_WEEL_TELE_LEFT_IN)
			|| tm->outp->isOutputChanging(OUT_WEEL_TELE_RIGHT_IN)
			|| tm->outp->isOutputChanging(OUT_WEEL_TELE_LEFT_OUT)
			|| tm->outp->isOutputChanging(OUT_WEEL_TELE_RIGHT_OUT)
			|| tm->outp->isOutputChanging(OUT_FRAME_LOCK_UP)
			|| tm->outp->isOutputChanging(OUT_FRAME_LOCK_DOWN)){

		if(tm->outp->getOutputState(OUT_SPINNER_RIGHT_UP) == ACTIVE
				|| tm->outp->getOutputState(OUT_SPINNER_LEFT_UP) == ACTIVE
				|| tm->outp->getOutputState(OUT_SPINNER_REAR_UP) == ACTIVE
				|| tm->outp->getOutputState(OUT_SPINNER_RIGHT_TELE_OUT) == ACTIVE
				|| tm->outp->getOutputState(OUT_SPINNER_LEFT_TELE_OUT) == ACTIVE
				|| tm->outp->getOutputState(OUT_SPINNER_RIGHT_TELE_IN) == ACTIVE
				|| tm->outp->getOutputState(OUT_SPINNER_LEFT_TELE_IN) == ACTIVE
				|| tm->outp->getOutputState(OUT_FRAME_UP) == ACTIVE
				|| tm->outp->getOutputState(OUT_FRAME_DOWN) == ACTIVE
				|| tm->outp->getOutputState(OUT_STEER_LEFT) == ACTIVE
				|| tm->outp->getOutputState(OUT_STEER_RIGHT) == ACTIVE
				|| tm->outp->getOutputState(OUT_WEEL_TELE_LEFT_IN) == ACTIVE
				|| tm->outp->getOutputState(OUT_WEEL_TELE_RIGHT_IN) == ACTIVE
				|| tm->outp->getOutputState(OUT_WEEL_TELE_LEFT_OUT) == ACTIVE
				|| tm->outp->getOutputState(OUT_WEEL_TELE_RIGHT_OUT) == ACTIVE
				|| tm->outp->getOutputState(OUT_FRAME_LOCK_UP) == ACTIVE
				|| tm->outp->getOutputState(OUT_FRAME_LOCK_DOWN) == ACTIVE) {

			tm->outp->setRawOutput(OUT_PRESSURE, ACTIVE);
			//Serial.println(" PressureTask::update setOutputRaw(OUT_PRESSURE) ACTIVE");
		} else {

			tm->outp->setRawOutput(OUT_PRESSURE, INACTIVE);
			//Serial.println(" PressureTask::update setOutputRaw(OUT_PRESSURE) INACTIVE");
		}
	}
}

void PressureTask::timer() {}

void PressureTask::exit() {
	Task::exit();

	tm->outp->setRawOutput(OUT_PRESSURE, INACTIVE);
}






