#include <msp.h>
//#include "pellesinterface.c"
#define DEBOUNCE_TIME 1500
#define LED_RED 0
#define LED_RGB 1

extern int counter;
int counter =0;

void button_action(void);
void PORT1_IRQHandler(void);

static inline void ensure_func_gpio_odd (DIO_PORT_Odd_Interruptable_Type *port,
                                         uint8_t mask){
    if ((port->SEL0 & mask) && (port->SEL1 & mask)) {
        port->SELC |= mask;
    } else {
        port->SEL0 &= ~(mask);
        port->SEL1 &= ~(mask);
    }
}

static inline void ensure_func_gpio_even (DIO_PORT_Even_Interruptable_Type *port,
                                          uint8_t mask){
    if ((port->SEL0 & mask) && (port->SEL1 & mask)) {
        port->SELC |= mask;
    } else {
        port->SEL0 &= ~(mask);
        port->SEL1 &= ~(mask);
    }
}

static void init_gpio (void){
    /* Buttons (P1.1, p1.4) */
    // Ensure that function is GPIO
    ensure_func_gpio_odd(P1, (1<<1));
    ensure_func_gpio_odd(P1, (1<<4));
    // Set direction to input
    P1->DIR &= ~((1<<1) | (1<<4));
    // Enable pull resistor
    P1->REN |= ((1<<1) | (1<<4));
    // Set pull direction to up
    P1->OUT |= ((1<<1) | (1<<4));
    // Ensure that interrupts are disabled
    P1->IE &= ~((1<<1) | (1<<4));
    
    /* LEDs (P1.0, P2.0, P2.1, P2.2) */
    // Ensure that function is GPIO
    ensure_func_gpio_odd(P1, (1<<0));
    ensure_func_gpio_even(P2, (1<<0));
    ensure_func_gpio_even(P2, (1<<1));
    ensure_func_gpio_even(P2, (1<<2));
    // Set direction to output
    P1->DIR |= (1<<0);
    P2->DIR |= ((1<<0) | (1<<1) | (1<<2));
    // Ensure that high drive strength is disabled
    P1->DS &= ~(1<<0);
    P2->DS &= ~((1<<0) | (1<<1) | (1<<2));
    // Initialize to driven low
    P1->OUT &= ~(1<<0);
    P2->OUT &= ~((1<<0) | (1<<1) | (1<<2));
    // Ensure that interrupts are disabled
    P1->IE &= ~((1<<0));
    P1->IE &= ~((1<<0) | (1<<1) | (1<<2));
}

int main(void){
    /* Stop watchdog timer */
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;
    
    /* Configure GPIO */
    init_gpio();
    
    /* Configure Interrupts */
    // Trigger interrupts on falling edge
    P1->IES |= ((1<<1) | (1<<4));
    // Clear interrupt flags
    P1->IFG &= ~((1<<1) | (1<<4));
    // Enable pin interrupts
    P1->IE |= ((1<<1) | (1<<4));
    
    NVIC_SetPriority(PORT1_IRQn, 2);
    NVIC->ICPR[PORT1_IRQn > 31] |= ((PORT1_IRQn % 32) << 1);
    NVIC_EnableIRQ(PORT1_IRQn);
    /* Main Loop */
    for (;;) {
        __WFI();
    }
}

void button_action(void){
		if (counter < 1){
				counter = 4;
		}else if (counter > 4){
				counter = 1;
		}
		
		switch(counter){
			case 1:
					P2->OUT ^= (1<<0);
					break;
			case 2:
					P2->OUT ^= (1<<0);
					P1->OUT ^= (1<<0);
					break;
			case 3:
					P1->OUT ^= (1<<0);
					break;
			case 4:
					P2->OUT ^= (1<<0);
					P1->OUT ^= (1<<0);
					break;
		}
}


/* Interrupt Service Routines */
void PORT1_IRQHandler(void){
    if (P1->IFG & (1<<1)) {		
				counter++;
        for (uint32_t i = 0; i < DEBOUNCE_TIME; i++);
        if (!(P1->IN & (1<<1))) {
            button_action();
        }
        P1->IFG &= ~(1<<1);
    }
    if (P1->IFG & (1<<4)) {		
				counter--;
        for (uint32_t i = 0; i < DEBOUNCE_TIME; i++);
        if (!(P1->IN & (1<<4))) {
            button_action();
        }
        P1->IFG &= ~(1<<4);
    }
}
