
#ifndef _ASMARM_TIMEX_H
#define _ASMARM_TIMEX_H

typedef unsigned long cycles_t;
#define get_cycles()	({ cycles_t c; read_current_timer(&c) ? 0 : c; })

#endif
