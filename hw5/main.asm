    bits 64 ;;; разрядность машины
    extern malloc, puts, printf, fflush, abort ;;; функции которые будут использоваться 
    global main  ;;; точка входа

    section   .data
empty_str: db 0x0
int_format: db "%ld ", 0x0
data: dq 4, 8, 15, 16, 23, 42
data_length: equ ($-data) / 8

    section   .text
;;; print_int proc
print_int:
    push rbp                ; сохраняем стек
    mov rbp, rsp            ; база стека
    sub rsp, 16             ; выделение 16 байт

    mov rsi, rdi            ; pointer *struct{len, idx}
    mov rdi, int_format     ; long printf
    xor rax, rax            ; pointer *struct{len, idx} = null ?
    call printf             ; 

    xor rdi, rdi            ; null
    call fflush             ; stdout flush

    mov rsp, rbp            ; return back
    pop rbp                 ; 
    ret

;;; p proc
p:
    mov rax, rdi
    and rax, 1
    ret

;;; add_element proc
add_element:
    push rbp            ; сохраняем значение стека
    push rbx            ; зачем сохраняем значение
    push r14            ; то же самое ?
    mov rbp, rsp        ; база стека rsp -> rbp
    sub rsp, 16         ; выделение 16 байт

    mov r14, rdi        ; rdi -> r14 value array
    mov rbx, rsi        ; rsi -> rbx ptr_next

    mov rdi, 16         ; 
    call malloc         ; malloc(16)
    test rax, rax       ; if malloc(16) ПОЧЕМУ ИМЕННО rax ?
    jz abort            ; └──► abort

    mov [rax], r14      ; copy value
    mov [rax + 8], rbx  ; copy ptr_next

    mov rsp, rbp        ; 
    pop r14             ;
    pop rbx             ;
    pop rbp             ; восстанавливаем значение стека
    ret

;;; m proc
m:
    push rbp            ; 
    mov rbp, rsp        ;
    sub rsp, 16         ; 16 byte

    test rdi, rdi       ; if rdi == null
    jz outm             ; return 

    push rbp            ; 
    push rbx            ;

    mov rbx, rdi        ; copy rbx
    mov rbp, rsi        ; copy rsi

    mov rdi, [rdi]      ; get value
    call rsi            ; print value

    mov rdi, [rbx + 8]  ; 
    mov rsi, rbp
    call m

    pop rbx
    pop rbp

outm:
    mov rsp, rbp
    pop rbp
    ret

;;; f proc
f:
    mov rax, rsi        ;

    test rdi, rdi       ; if ptr == null
    jz outf

    push rbx
    push r12
    push r13

    mov rbx, rdi        ; ptr struct
    mov r12, rsi        ; empty pointer
    mov r13, rdx        ; p func

    mov rdi, [rdi]      ; value from struct
    call rdx            ; 
    test rax, rax       ; ищем 15 и 23 
    jz z                ;

    mov rdi, [rbx]      ;   value from struct
    mov rsi, r12        ;   copy ptr
    call add_element    ;   create struct
    mov rsi, rax        ;   ptr struct to rsi
    jmp ff              ; jmp ff

z:
    mov rsi, r12

ff:
    mov rdi, [rbx + 8]  ; ptr->next
    mov rdx, r13        ; p func
    call f              ; 

    pop r13
    pop r12
    pop rbx

outf:
    ret

;;; main proc
main:
    push rbx                        ; почему rbx ?

    xor rax, rax                    ; обнуление регистра rax.
    mov rbx, data_length            ; начало счетчика цикла
adding_loop:
    mov rdi, [data - 8 + rbx * 8]   ; len - 1 - поэтому data - 8
    mov rsi, rax                    ; передаем ссылку на предыдущую ноду списка
    call add_element
    dec rbx                         ; идем обратно 
    jnz adding_loop

    mov rbx, rax                    ; copy pointer

    mov rdi, rax                    ; copy pointer
    mov rsi, print_int              ; передача функции print_int
    call m                          ; 

    mov rdi, empty_str
    call puts

    mov rdx, p                      ; 
    xor rsi, rsi                    ; rsi = null
    mov rdi, rbx                    ; rbx = pointer struct
    call f                          ; 

    mov rdi, rax
    mov rsi, print_int
    call m

    mov rdi, empty_str
    call puts

    pop rbx

    xor rax, rax
    ret
