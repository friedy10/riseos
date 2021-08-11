#ifndef SPINLOCK_H
#define SPINLOCK

#define barrier() asm volatile("" \
							   :  \
							   :  \
							   : "memory")
#define cpu_relax() asm volatile("pause\n" \
								 :         \
								 :         \
								 : "memory")
#define EBUSY 1


typedef unsigned spinlock;

// Atomic exchange
static inline unsigned xchg_32(void * ptr, unsigned x)
{
	__asm__ __volatile__("xchgl %0,%1"
						 : "=r"((unsigned)x)
						 : "m"(*(volatile unsigned *)ptr), "0"(x)
						 : "memory");

	return x;
}

static void spin_lock(spinlock * lock)
{
	while (1) {
		if (!xchg_32(lock, EBUSY)) {
			return;
		}

		while (*lock)
			cpu_relax();
	}
}

static void spin_unlock(spinlock * lock)
{
	barrier();
	*lock = 0;
}

static int spin_trylock(spinlock * lock)
{
	return xchg_32(lock, EBUSY);
}

#endif
