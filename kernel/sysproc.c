#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "vm.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  kexit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return kfork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return kwait(p);
}

uint64
sys_wait2(void)
{
  uint64 p, usr, kern;
  argaddr(0, &p);
  argaddr(1, &usr);
  argaddr(2, &kern);

  return kwait2(p, usr, kern);
}


uint64
sys_sbrk(void)
{
  uint64 addr;
  int t;
  int n;

  argint(0, &n);
  argint(1, &t);
  addr = myproc()->sz;

  if(t == SBRK_EAGER || n < 0) {
    if(growproc(n) < 0) {
      return -1;
    }
  } else {
    // Lazily allocate memory for this process: increase its memory
    // size but don't allocate memory. If the processes uses the
    // memory, vmfault() will allocate it.
    if(addr + n < addr)
      return -1;
    myproc()->sz += n;
  }
  return addr;
}

uint64
sys_pause(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kkill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64
sys_ctime(void)
{
  uint32 first = *((uint32 *) RTC);
  uint32 second = *((uint32 *) (RTC + 0x4));
  return ((uint64) second << 32 | first);
}

uint64
sys_timtog()
{
  struct proc *p = myproc();
  p->timing = !p->timing;
  if (p -> timing) {
    return 1;
  } else {
    return 0;
  }
}

uint64
sys_getkt(int target_pid)
{
	argint(0, &target_pid);
	struct proc *p = lookup_pid(target_pid);
	if (p) {
		return p -> kern_time;	
	} else {
		return(0);
	}
}

uint64
sys_getut(int target_pid)
{
	argint(0, &target_pid);
	struct proc *p = lookup_pid(target_pid);
	if (p) {
		return p -> user_time;
	} else {
		return(0);
	}
}
