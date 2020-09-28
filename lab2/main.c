#include "msp.h"

int main(void)
{
    volatile uint32_t i;

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;  // Stop watchdog timer

		P1DIR |= BIT0; //Configure P1.0 as Output
		P1OUT |= BIT0; //Drive Output High for P1.0
		P2DIR |= BIT0; //Configure P2.0 as Output
		P2OUT |= BIT0; //Drive Output High for P2.0
		P2DIR |= BIT1; //Configure P2.1 as Output
		P2OUT |= BIT1; //Drive Output High for P2.1
		P2DIR |= BIT2; //Configure P2.2 as Output
		P2OUT |= BIT2; //Drive Output High for P2.2
	
		P1DIR &= ~BIT1; //Config P1.1 as input (It will be anyways input after reset)
		P1REN = BIT1; //Enable Pullup/down for P1.1
		P1OUT = BIT1; //Select Pullup resistor for P1.1
		P1DIR &= ~BIT4; //Config P1.4 as input (It will be anyways input after reset)
		P1REN = BIT4; //Enable Pullup/down for P1.4
		P1OUT = BIT4; //Select Pullup resistor for P1.4
	
    while(1)
    {
				if( !(P1IN & BIT1) ){ //Evaluates to True for a 'LOW' on P1.1
					P1OUT ^= BIT0; //Toggle the state of P1.0
			}
				if( !(P1IN & BIT4) ){ //Evaluates to True for a 'LOW' on P1.4
					P2OUT ^= BIT0; //Toggle the state of P2.0
			}
    }
}
