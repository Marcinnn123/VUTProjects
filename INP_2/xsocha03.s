; Autor reseni: Marcon Sochacki xsocha03

; Projekt 2 - INP 2022
; Vernamova sifra na architekture MIPS64

; DATA SEGMENT
                .data
login:          .asciiz "xsocha03"  ; sem doplnte vas login
cipher:         .space  17  ; misto pro zapis sifrovaneho loginu

params_sys5:    .space  8   ; misto pro ulozeni adresy pocatku
                            ; retezce pro vypis pomoci syscall 5
                            ; (viz nize "funkce" print_string)

; CODE SEGMENT
                .text

                ; ZDE NAHRADTE KOD VASIM RESENIM
main:
                lb r11, login(r13) ;zapsani pismena na r11
                slti r23, r11, 97 ; pokud uz neni pismeno       
                bne r23, r0, wrong_input
                slti r23, r11, 123 ; pokud uz neni pismeno
                beq r23, r0, wrong_input
                sll r23, r13, 31
                srl r23, r23, 31 ; modulo jestli mame pripocitat nebo odpocitat 
                beq r23, r0, plus
                b minus
plus:
                addi r11, r11, 19  ; pripoctu s
                slti r23, r11, 123 ; kontrola kruhoveho bufferu 
                bne r23, r0, cipher_save  
                addi r11, r11, -122
                addi r11, r11, 96
                b cipher_save
minus:
                addi r11, r11, -15 ; odectu o
                slti r23, r11, 97 ; kontrola kruhoveho bufferu
                beq r23, r0, cipher_save  
                addi r11, r11, 122
                addi r11, r11, -96
                b cipher_save
cipher_save:    
                sb r11, cipher(r13)
                addi r13, r13, 1 ; zvyseni indexu
                b main

wrong_input:
    jal print_string
    syscall 0

print_string:   ; adresa retezce se ocekava v r4
                addi r4, r0, cipher
                sb r0, cipher(r13) 
                sw      r4, params_sys5(r0)
                daddi   r14, r0, params_sys5    ; adr pro syscall 5 musi do r14
                syscall 5   ; systemova procedura - vypis retezce na terminal
                jr      r31 ; return - r31 je urcen na return address


;xsocha03-r11-r13-r23-r20-r0-r4
