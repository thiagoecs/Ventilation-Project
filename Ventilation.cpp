/*
===============================================================================
 Name        : main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
 */
#include "board.h"

#include <cr_section_macros.h>
#include "DigitalIoPin.h"
#include "I2C.h"
#include "LiquidCrystal.h"
#include "SerialPort.h"
#include "LpcUart.h"
#include "IntegerEdit.h"
#include "SimpleMenu.h"
#include "Pressure.h"
#include "Fan.h"
#include "PID.h"
#define TICKRATE_HZ 1000

SimpleMenu *menu;
std::atomic_int counter, timer, timeout;
static std::atomic<uint32_t> systicks;

static IntegerEdit* fanMenu, *pressureMenu;

std::atomic<bool> mode { true };		//false = auto, true = manual
std::atomic<bool> mainMenu { false };

extern "C" {
void PIN_INT0_IRQHandler(void) {
	if (menu != nullptr) {
		timer = 0;
		timeout = millis();
		menu->event(SimpleMenu::up);
	}
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(0));
}


void PIN_INT1_IRQHandler(void) {
	if (menu != nullptr) {
		timer = 0;
		timeout = millis();
		menu->event(SimpleMenu::down);
	}
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(1));
}

void PIN_INT2_IRQHandler(void) {
	if (menu != nullptr) {
		timer = 0;
		mainMenu = true;
		mode = !mode;
		timeout = millis();
		menu->event(SimpleMenu::show);

	}
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(2));
}
}


extern "C" {
void SysTick_Handler(void) {
	if (counter > 0)
		counter--;

	if (++timer > 1000) {
		if (mainMenu) {
			timer = 0;
			mainMenu = false;
			menu->event(SimpleMenu::show);
		} else {
			timer = 0;
			menu->event(SimpleMenu::ok);
		}
	}
	++systicks;
}
}

uint32_t millis() {
	return systicks;
}

void Sleep(int ms) {
	counter = ms;
	while(counter > 0) {
		__WFI();
	}
}


int main(void) {
	SystemCoreClockUpdate();
	Board_Init();
	Chip_Clock_SetSysTickClockDiv(1);
	SysTick_Config(Chip_Clock_GetSysTickClockRate() / TICKRATE_HZ);
	Chip_PININT_Init(LPC_GPIO_PIN_INT);
	Chip_RIT_Init(LPC_RITIMER);

	//testing on board
	//DigitalIoPin sw1(0, 17, true, true, true), sw2(1, 11, true, true, true), sw3(1, 9, true, true, true);

	DigitalIoPin B1(0,9, true, true, true), B2(0,10,true, true, true), B3(0, 16, true, true, true);
	//B1 - Mode/OK, B2 - up, B3 - down
	DigitalIoPin rs(0,8, false), en(1,6,false), d4(1, 8, false), d5(0,5,false), d6(0,6,false), d7(0,7,false);
	LiquidCrystal *lcd = new LiquidCrystal(&rs, &en, &d4, &d5, &d6, &d7);

	Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_PININT);

	/* Reset the PININT block */
	Chip_SYSCTL_PeriphReset(RESET_PININT);

	/* Configure interrupt channel for the GPIO pin in INMUX block*/
	Chip_INMUX_PinIntSel(0, 0, 9); // putting sw1 on int channel 0
	Chip_INMUX_PinIntSel(1, 0, 10); // putting sw1 on int channel 0
	Chip_INMUX_PinIntSel(2, 0, 16); // putting sw1 on int channel 0

	// testing
	/*Chip_INMUX_PinIntSel(0, 0, 17); // putting sw1 on int channel 0
	Chip_INMUX_PinIntSel(1, 1, 11); // putting sw2 on int channel 0
	Chip_INMUX_PinIntSel(2, 1, 9); // putting sw3 on int channel 0*/


	/* Configure channel interrupt as edge sensitive and falling edge interrupt */
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(0));
	Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH(0));
	Chip_PININT_EnableIntHigh(LPC_GPIO_PIN_INT, PININTCH(0));
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(1));
	Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH(1));
	Chip_PININT_EnableIntHigh(LPC_GPIO_PIN_INT, PININTCH(1));
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(2));
	Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH(2));
	Chip_PININT_EnableIntHigh(LPC_GPIO_PIN_INT, PININTCH(2));

	/* Enable interrupt in the NVIC */
	NVIC_ClearPendingIRQ(PIN_INT0_IRQn);
	NVIC_EnableIRQ(PIN_INT0_IRQn);
	NVIC_ClearPendingIRQ(PIN_INT1_IRQn);
	NVIC_EnableIRQ(PIN_INT1_IRQn);
	NVIC_ClearPendingIRQ(PIN_INT2_IRQn);
	NVIC_EnableIRQ(PIN_INT2_IRQn);

	lcd->begin(16,2);
	lcd->setCursor(0,0);
	lcd->print("Starting up");


	menu = new SimpleMenu;
	pressureMenu = new IntegerEdit(lcd, std::string("Pressure"), 0, 120);
	fanMenu = new IntegerEdit(lcd, std::string("Fan"), 0, 100);
	menu->addItem(pressureMenu);
	menu->addItem(fanMenu);

	Fan fan;

	Pressure pressure(I2C{I2C_config{}}, 0x40, 0xF1);

	PID<int> pid(1, 0, 0.125);

	while(1) {
		auto starttime = millis();
		auto pressure_diff = pressure.getPressure();
		auto fanSpeed = fan.getFrequency();

		if (mode) {
			if (fanMenu->getValue() != fanSpeed)
				fan.setFrequency(fanMenu->getValue());
		} else {
			if (abs(pressureMenu->getValue() - pressure_diff) > 2){
				fan.setFrequency(fanSpeed + pid.calculate(pressureMenu->getValue(), pressure_diff));
			} else {
				timeout = millis();
			}
		}

		fanMenu->setPressure(pressure_diff);
		pressureMenu->setPressure(pressure_diff);
		if(millis()-starttime < 1000){
			Sleep(1000-(millis()-starttime));
		}

		menu->event(SimpleMenu::show);
	}

	return 0;
}

