
PROCESSOR 18F8722
    
#include <xc.inc>

; configurations
CONFIG OSC = HSPLL, FCMEN = OFF, IESO = OFF, PWRT = OFF, BOREN = OFF, WDT = OFF, MCLRE = ON, LPT1OSC = OFF, LVP = OFF, XINST = OFF, DEBUG = OFF

; global definitions
; variables that need to be accessed globally

GLOBAL _t1, _t2, _t3    ; variables for time delay
GLOBAL led_flag         ; state of LEDs

; allocate memory in data bank for variables
PSECT udata_acs
    led_flag:
        DS 1    ; allocate 1 byte
    _t1:
        DS 1    ; allocate 1 byte
    _t2:
        DS 1    ; allocate 1 byte
    _t3:
        DS 1    ; allocate 1 byte

; set reset vector to indicate the start of your program
	
PSECT resetVec,class=CODE,reloc=2
resetVec:
    goto    main


; 1 second delay implemented as busy-wait
; while executing this procedure, for 1 second,
; your program will not respond to any input.
busy_delay_1sec:
    movlw 0x84      ; copy desired value to W
    movwf _t3       ; copy W into t3
    _loop3:
        movlw 0xAF      ; copy desired value to W
        movwf _t2       ; copy W into t2
        _loop2:
            movlw 0x8F      ; copy desired value to W
            movwf _t1       ; copy W into t1
            _loop1:
                decfsz _t1, 1   ; decrement t1, if 0 skip next 
                goto _loop1     ; else keep counting down
                decfsz _t2, 1   ; decrement t2, if 0 skip next 
                goto _loop2     ; else keep counting down
                decfsz _t3, 1   ; decrement t3, if 0 skip next 
                goto _loop3     ; else keep counting down
                return    

init:

    ; clear led_flag
    CLRF led_flag       ; initially, flag is set to 0

    ; set RA4 as input source, clear PORTA LEDs
    movlw 00010000B     ; move literal written as a binary value to W 
    movwf TRISA         ; move W to TRISA to indicate input/output direction 
                        ; only RA4 pin is input
    clrf  LATA          ; clear output latch so that all LEDs in PORTA is turned off

    ; initialize PORTB as output source
    movlw 0x00          ; move literal written as a hexadecimal value to W
    movwf TRISB         ; move W to TRISB to indicate input/output direction 
                        ; all pins are outputs
    ; light_portb_leds
    movlw 0xFF          ; move all 1's to W
    movwf LATB          ; move W to LATB to light all leds


ra4_task:
    btfsc PORTA, 4          ; if ra4 is currently not pressed, return
    goto wait_ra4_release   ; if pressed, wait until released
    return

    wait_ra4_release:
        btfsc PORTA, 4          ; if ra4 is not released
        goto wait_ra4_release   ; wait until it is released (else skip here)
        goto update_flag        ; change led_flag to change the state of the leds

    update_flag:
        movlw 0x00          ; move 0 to W
        ;; CPFSEQ: compare register f with W, skip next instruction if they are equal
        cpfseq led_flag     ; compare led_flag with W
        goto reset_flag     ; if not equal to W (not zero), reset
        goto set_flag     ; if equal to W (zero), set

    set_flag:
        movlw 0x01          ; move 1 to W
        movwf led_flag      ; set led_flag to W
        return

    reset_flag:
        clrf led_flag
        return

led_task:
    ;; CPFSEQ: compare register f with W, skip next instruction if they are equal
    movlw 0x00          
    cpfseq led_flag
    goto turn_on_leds
    goto turn_off_leds

    turn_on_leds:
        movlw 0xFF
        movwf LATB
        return

    turn_off_leds:
        clrf LATB
        return

PSECT CODE
main:
    call init               ; call initialization procedure
    call busy_delay_1sec    ; call 1 second delay
    clrf LATB               ; clear output latch of PORTB to turn off LEDs
    ; round-robin loop with tasks
    ; each press and release of ra4 turns on or turns off PORTB LEDs.
    loop:
        call ra4_task
        call led_task
        goto loop

end resetVec