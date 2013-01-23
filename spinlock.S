.global spin_lock
spin_lock:
  mov (%esp), %ecx
1:
  mov $1, %eax
  xchg %eax, (%ecx)
  test %eax, %eax
  jz 1b
  ret

.global spin_unlock
spin_unlock:
  mov (%esp), %ecx
  movl $0, (%ecx)
  ret

.global spin_trylock
spin_trylock:
  mov (%esp), %ecx
  mov $1, %eax
  xchg %eax, (%ecx)
  not %eax
  ret
