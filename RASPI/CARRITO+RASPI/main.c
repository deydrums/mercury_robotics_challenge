/*---------------------------------------------------LIBRERÍAS-----------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "inc/hw_memmap.h"
#include "driverlib/timer.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "driverlib/interrupt.h"
#include "inc/hw_gpio.h"
#include "driverlib/uart.h"


/*-----------------------------------------------DEFINICIONES-----------------------------------------------*/
#define F_Hz 50
uint32_t T_ciclos;
uint32_t T_alto,T_alto2,T_alto3,T_alto4 = 0;
/*--------------------------------------------VARIABLES GLOBALES-----------------------------------------------*/
int Contador=5;
int flag = 0; //Angulo
int Status;
int s1 = 0;
int s2 = 0;
int s3 = 65;
int s4 = 110;
char car;

/*------------------------------------------RUTINAS DE INTERRUPCIÓN---------------------------------------------*/

//Rutina para interrupcion de TIMER
void IntTimer (void){
    TimerIntClear(TIMER0_BASE,TIMER_TIMA_TIMEOUT);
        if ((Status ==1) & (Contador < 40)){ //45
            Contador = Contador +1;
        }else if ((Status ==2)& (Contador > 5)){
         Contador = Contador -1;
        }

        if ((Status ==3) & (s2 < 180)){ //45
            s2 = s2 +1;
        }else if ((Status ==4)& (s2 > 0)){
            s2 = s2 -1;
        }

            if ((Status == 5)&(s3 < 120)){
                s3 = s3 +1;
            }else if ((Status == 6) & (s3 > 0)){
                s3 = s3 - 1;
            }
                if ((Status == 7)&(s4 < 120)){
                    s4 = s4 + 1;
                }else if ((Status == 8) & (s4 > 0)){
                    s4 = s4 - 1;
                }

                     }

void UARTIntHandler(void) {
    uint32_t ui32Status;
    ui32Status = UARTIntStatus(UART0_BASE, true); //get interrupt status
    UARTIntClear(UART0_BASE, ui32Status); //clear the asserted interrupts
    while(UARTCharsAvail(UART0_BASE))
        {
        car = UARTCharGetNonBlocking(UART0_BASE);
        UARTCharPut(UART0_BASE, car);
        SysCtlDelay(SysCtlClockGet() / (1000 * 3)); //delay ~1 msec
        }
}
void Configuraciones(void){
    SysCtlClockSet(SYSCTL_XTAL_16MHZ|SYSCTL_SYSDIV_2_5);                                                 //Definición de reloj a 80MHz
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_6|GPIO_PIN_7);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE,TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER0_BASE,TIMER_A,900000);
    TimerEnable(TIMER0_BASE, TIMER_A);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 9600,
            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
}
/*----------------------------------------------CONFIGURACIONES PWM---------------------------------------------*/
void PWM_Init(void){
    T_ciclos =  ((SysCtlClockGet()/64)/F_Hz)-1;
    PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN);
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, T_ciclos);
    PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT, true);
    PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT, true);
    PWMGenEnable(PWM1_BASE, PWM_GEN_3);

    PWMGenConfigure(PWM0_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN);
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_2, T_ciclos);
    PWMOutputState(PWM0_BASE, PWM_OUT_4_BIT, true);
    PWMOutputState(PWM0_BASE, PWM_OUT_5_BIT, true);
    PWMGenEnable(PWM0_BASE, PWM_GEN_2);

}
void PWM(void){
      SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
      SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
      SysCtlPWMClockSet(SYSCTL_PWMDIV_64);
      SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
      SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
      GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_2);
      GPIOPinConfigure(GPIO_PF2_M1PWM6);
      GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_3);
      GPIOPinConfigure(GPIO_PF3_M1PWM7);
      GPIOPinTypePWM(GPIO_PORTE_BASE, GPIO_PIN_4);
      GPIOPinConfigure(GPIO_PE4_M0PWM4);
      GPIOPinTypePWM(GPIO_PORTE_BASE, GPIO_PIN_5);
      GPIOPinConfigure(GPIO_PE5_M0PWM5);
      GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_4);

}
/*-----------------------------------------CONFIGURACIONES DE INTERRUPCION---------------------------------------------*/

void NVIC(void){
    TimerIntEnable(TIMER0_BASE,TIMER_TIMA_TIMEOUT);                            //Configura la interrupcion del timer
    IntEnable(INT_TIMER0A);                                                   // La tiene lista para ejecutarse
    IntEnable(INT_UART0);
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
    IntMasterEnable();                                                       //Habilita todas las interrupciones
}

/*------------------------------------------------------MAIN-----------------------------------------------------------*/

int main(void)
{
    PWM();
    PWM_Init();
    Configuraciones();
    NVIC();

    while(1){
                   PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, T_alto2);
                   PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, T_alto);
                   PWMPulseWidthSet(PWM0_BASE, PWM_OUT_4, T_alto3);
                   PWMPulseWidthSet(PWM0_BASE, PWM_OUT_5, T_alto4);
                   T_alto = (125*(Contador*4+180))/36;
                   T_alto2 = (125*(s2*4+180))/36;
                   T_alto3 = (125*(s3*4+180))/36;
                   T_alto4 = (125*(s4*4+180))/36;


                   switch(car){

                    case 'A':
                       GPIOPinWrite(GPIO_PORTD_BASE,  GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 5);
                       break;
                    case 'B'  :
                        GPIOPinWrite(GPIO_PORTD_BASE,  GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 9);
                       break;
                    case 'C'  :
                        GPIOPinWrite(GPIO_PORTD_BASE,  GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 6);             //1
                       break;
                    case 'D'  :
                        GPIOPinWrite(GPIO_PORTD_BASE,  GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 10);             //2
                       break;
                    case 'E'  :
                        GPIOPinWrite(GPIO_PORTD_BASE,  GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x00);             //2
                        GPIOPinWrite(GPIO_PORTF_BASE,  GPIO_PIN_4, 0);
                        Status = 0;
                       break;
                    case 'L'  :
                        GPIOPinWrite(GPIO_PORTD_BASE,  GPIO_PIN_6, 64);
                       break;
                    case 'X'  :
                        GPIOPinWrite(GPIO_PORTD_BASE,  GPIO_PIN_6, 0);
                       break;



                             case 'F':
                                 Status = 1;
                                break;
                             case 'G':
                                 Status = 2;
                                 break;
                             case 'H':
                                 Status = 3;
                                 break;
                             case 'I':
                                 Status = 4;
                                 break;

                             case 'J':
                                 Status = 5;
                                 break;
                             case 'K':
                                 Status = 6;
                                 break;
                             case 'N':
                                 Status = 7;
                                 break;
                             case 'O':
                                 Status = 8;
                                 break;
                             case 'P':
                                 s3 = 65;
                                 s4 = 110;
                                 break;
                             case 'Q':
                                 GPIOPinWrite(GPIO_PORTF_BASE,  GPIO_PIN_4, 16);
                                 break;
                         }


    }
}

