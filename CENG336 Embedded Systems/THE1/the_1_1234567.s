PROCESSOR 18F8722
    
#include <xc.inc>

CONFIG OSC = HSPLL, FCMEN = OFF, IESO = OFF, PWRT = OFF, BOREN = OFF, WDT = OFF, MCLRE = ON, LPT1OSC = OFF, LVP = OFF, XINST = OFF, DEBUG = OFF
    

PSECT resetVec,class=CODE,reloc=2
resetVec: 
    goto    main



PSECT utils, reloc=2

    
; CODE segment to hold task implementations
PSECT tasks,reloc=2


; CODE segment to hold the main routine
; You need to use init_complete, ra5_pressed and  job_done labels
PSECT main,reloc=2

    
init_complete:
    movlw 0x0f
    movwf ADCON1
    movlw 0xff	    ; RA5 is input
    movwf TRISA	    
    movlw 0x00
    movwf TRISC
    movlw 0x00
    movwf TRISD
    movlw 0x00
    movwf TRISE
    
    bcf PORTA, 0
    goto loop


set_RA0:
    bsf PORTA, 0
    goto goto_wait

pressed:
    movf PORTC,0	; W = PORTC
    addwf PORTE, 0	; PORTC stored in PORTE
    movf PORTD, 0
    addwf PORTE, 0
    BC set_RA0

    goto_wait:
        goto job_done
    
job_done:
    btfsc PORTA, 5
    goto job_done	    ; RA5 is 1
    goto reset_task		    ; RA5 is 0 --> Released

reset_task:
    clrf PORTE
    bcf PORTA, 0
    goto loop
    

main:
goto init_complete
    loop:
	btfsc PORTA, 5
	goto pressed
	goto loop

    


END resetVec


