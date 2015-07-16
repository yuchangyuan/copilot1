#ifndef SYS_H
#define SYS_H

void sys_init();

void sys_wait(int n);
void sys_wait_next_tick();
void sys_step();

#endif
