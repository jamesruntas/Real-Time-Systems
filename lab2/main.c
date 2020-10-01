#include "msp.h"
//Lab 2 James Runtas 101109175 SYSC3310 Fall 2020


int main (void){
int rgb_val = 0; 	
int led1_state = 0;
volatile uint32_t i;

WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;   // disabling watchdog timer
	
P1DIR |= (uint8_t) ((1 << 0)); //output 1.0
P2DIR |=  (uint8_t) ((1 << 0) | (1 << 1) | (1 << 2)); //output 2.0, 2.1, 2.2

P1->SEL0 &= (uint8_t)(~((1<<0) | (1<<1) | (1<<4))); // setting up inputs 1.0, 1.1 and 1.4
P1->SEL1 &= (uint8_t) (~((1<<0) |(1<<1) | (1<<4)));
P2->SEL0 &= (uint8_t)(~((1<<0) | (1<<1) |  (1<<2))); // setting up inputs 2.0, 2.1 and 2.2
P2->SEL1 &= (uint8_t) (~((1<<0) | (1<<1) |  (1<<2)));

P1DIR |=  (uint8_t) ((0 << 1) | (0 << 4)); 
P1REN |= (uint8_t) ((0 << 1) | (0 << 4));
P1OUT |=(uint8_t) ((0 << 1) | (0 << 4));
	
	

while(1){
	while ((P1IN & (uint8_t)(1<<1))&&(P1IN & (uint8_t)(1<<4))){  //debouncing
	}
    i = 100;

    while(i > 0){i--;}

    if(!(P1IN & (uint8_t)(1<<1))){               //handle if button 1 was pressed (case 1) 		
    	P1OUT ^= (uint8_t)(1<<0);                // Toggle P1.0
	    
	    
	    
	//I still need some way to turn off LED2 which will be coded right here	
	    
	    
	    
	    
	    
    }
    if(!(P1IN & (uint8_t)(1<<1))){  //handle if button 1 was pressed (case 2)	
    	led_val++;
	    
	if (led1_state == 1){  //if led1 is on, we need to turn it off
		P1OUT ^= (uint8_t)(0<<0); //turn off led1
		led1_state = 0;
        }
						
	P2->OUT &= (uint8_t) (rgb_val);   //now that led1 is off we can turn led2 on
	P2->OUT |= (uint8_t) (rgb_val);
    } 
		
    if(!(P1IN & (uint8_t)(1<<4))){ //handle if button 2 was pressed
			
	rgb_val++;
	if (rgb_val > 7){  //if the end of rgb color cycle is reached, reset
		rgb_val = 1;
	}
	    
	P2->OUT &= (uint8_t) (rgb_val);   //output new color on led2
	P2->OUT |= (uint8_t) (rgb_val);
     }
     else{
	continue;
     }
		
		
} //while (1)
}//int main



