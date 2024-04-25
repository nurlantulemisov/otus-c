bits 64;
;
;
разрядность машины extern malloc, puts, printf, fflush, abort, free;
;
;
функции которые будут использоваться global main;
;
;
точка входа

  section.data empty_str : db 0x0 int_format : db "%ld ",
  0x0 data : dq 4,
	     8,
	     15,
	     16,
	     23,
	     42 data_length : equ($ - data)
			      / 8

				section.text;
;
;
print_int proc print_int : push rbp;
сохраняем стек mov rbp, rsp;
база стека sub rsp, 16;
выделение 16 байт

  mov rsi,
  rdi;
pointer *struct {
  len, idx
} mov rdi, int_format;
long printf xor rax, rax;
pointer *struct {
  len, idx
} = null ? call printf;

xor rdi, rdi;
null call fflush;
stdout flush

  mov rsp,
  rbp;
return back pop rbp;
ret

  ;
;
;
p proc p : mov rax, rdi and rax, 1 ret

  ;
;
;
add_element proc add_element : push rbp;
сохраняем значение стека push rbx;
зачем сохраняем значение push r14;
то же самое ? mov rbp, rsp;
база стека rsp->rbp sub rsp, 16;
выделение 16 байт

  mov r14,
  rdi;
rdi->r14 value array mov rbx, rsi;
rsi->rbx ptr_next

  mov rdi,
  16;
call malloc;
malloc(16) test rax, rax;
if malloc(16)
  ПОЧЕМУ ИМЕННО rax ? jz abort;
└──► abort

  mov[rax],
  r14;
copy value mov[rax + 8], rbx;
copy ptr_next

  mov rsp,
  rbp;
pop r14;
pop rbx;
pop rbp;
восстанавливаем значение стека ret

  ;
;
;
m proc forEach m : push rbp;
mov rbp, rsp;
sub rsp, 16;
16 byte

  test rdi,
  rdi;
if rdi
  == null jz outm;
return

  push rbp;
push rbx;

mov rbx, rdi;
copy rbx mov rbp, rsi;
copy rsi

  process_loop : mov rdi,
		 [rbx];
get value call rsi;
print value

  mov rsi,
  rbp;
copy func

  mov rdi,
  [rbx + 8];
mov rbx, rdi;
update rbx with new value test rbx, rbx;
check if rbx is null jnz process_loop;
if not null
  ,
    continue the loop

      pop rbx pop rbp

	outm : mov rsp,
	       rbp pop rbp ret

		 f : push rbx push r12 push r13

		       loop_start : mov rax,
				    rsi mov rbx,
				    rdi mov r12,
				    rsi mov r13,
				    rdx

				      test rdi,
				    rdi jz loop_end;
ptr
  == null

    mov rdi,
  [rdi];
get value call rdx test rax,
  rax jz z

    mov rdi,
  [rbx] mov rsi, r12 call add_element mov rsi,
  rax

    loop_continue : mov rdi,
		    [rbx + 8];
ptr = ptr->next mov rdx,
  r13 jmp loop_start

    z : mov rsi,
	r12 jmp loop_continue

	  loop_end : pop r13 pop r12 pop rbx ret

		       free_list :

			 .loop_start test rdi,
	rdi jz.loop_end

	  mov rsi,
	[rdi + 8] call free_node

	  mov rdi,
	rsi

	  jmp
	    .loop_start

	    .loop_end : ret

			  free_node : call free ret

  ;
;
;
main proc main : push rbx;
почему rbx ?

	   xor rax,
  rax;
обнуление регистра rax.mov rbx, data_length;
начало счетчика цикла adding_loop : mov rdi, [data - 8 + rbx * 8];
len - 1 - поэтому data - 8 mov rsi, rax;
передаем ссылку на предыдущую ноду списка call add_element dec rbx;
идем обратно jnz adding_loop

  mov rbx,
  rax;
copy pointer mov r8, rax;
copy list

  mov rdi,
  rax;
copy pointer mov rsi, print_int;
передача функции print_int call m;

mov rdi,
  empty_str call puts

    mov rdx,
  p;
xor rsi, rsi;
rsi = null mov rdi, rbx;
rbx = pointer struct call f;

mov rdi, rax mov rsi,
  print_int call m

    mov rdi,
  rax call free_list

    mov rdi,
  r8 call free_list

    mov rdi,
  empty_str call puts

      pop rbx

    xor rax,
  rax ret
