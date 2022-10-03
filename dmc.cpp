#include "state.h"
#include <z80ex/z80ex.h>

dmcState::dmcState(){

}
dmcState::~dmcState(){

}
void dmcState::genIRQ(){

}
void dmcState::clockAIO(){//check if the AIO should be executed for every channel
	for(int i=0;i<8;i++){
	}
}

void dmcState::clockTimer(){//clock the IRQ timer; if zero generate an interrupt
	if((IRQtmr==0)&&(IRQctrl&0b100)){
		IRQtmr = IRQreload;
		dmcState::genIRQ();
	}
}
