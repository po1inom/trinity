#pragma once

#include <sys/time.h>
#include <sys/types.h>

#include <stdio.h>

#include "constants.h"
#include "exit.h"
#include "net.h"
#include "types.h"

void * alloc_shared(unsigned int size);
int create_shm(void);
void setup_shm_postargs(void);

struct shm_s {
	unsigned long total_syscalls_done;
	unsigned long successes;
	unsigned long failures;
	unsigned long previous_count;
	unsigned long child_syscall_count[MAX_NR_CHILDREN];

	unsigned long regenerate;
	unsigned int seed;
	unsigned int seeds[MAX_NR_CHILDREN];
	unsigned int reseed_counter;

	//Indices of syscall in syscall table that are active. All indices shifted by +1. Empty index equals to 0.
	int active_syscalls32[MAX_NR_SYSCALL];
	int active_syscalls64[MAX_NR_SYSCALL];
	int active_syscalls[MAX_NR_SYSCALL];
	unsigned int nr_active_syscalls;
	unsigned int nr_active_32bit_syscalls;
	unsigned int nr_active_64bit_syscalls;

	pid_t mainpid;
	pid_t pids[MAX_NR_CHILDREN];
	unsigned char child_type[MAX_NR_CHILDREN];

	pid_t last_reaped;
	bool spawn_no_more;
	char kill_count[MAX_NR_CHILDREN];

	unsigned int running_childs;
	struct timeval tv[MAX_NR_CHILDREN];

	FILE *logfiles[MAX_NR_CHILDREN];

	int pipe_fds[MAX_PIPE_FDS*2];
	int file_fds[NR_FILE_FDS];		/* All children inherit these */
	int perf_fds[MAX_PERF_FDS];
	int epoll_fds[MAX_EPOLL_FDS];
	int eventfd_fds[MAX_EPOLL_FDS];

	struct socketinfo sockets[NR_SOCKET_FDS];

	/* state for the syscall currently in progress. */
	unsigned int previous_syscallno[MAX_NR_CHILDREN];
	unsigned long previous_a1[MAX_NR_CHILDREN];
	unsigned long previous_a2[MAX_NR_CHILDREN];
	unsigned long previous_a3[MAX_NR_CHILDREN];
	unsigned long previous_a4[MAX_NR_CHILDREN];
	unsigned long previous_a5[MAX_NR_CHILDREN];
	unsigned long previous_a6[MAX_NR_CHILDREN];

	unsigned int syscallno[MAX_NR_CHILDREN];
	unsigned long a1[MAX_NR_CHILDREN];
	unsigned long a2[MAX_NR_CHILDREN];
	unsigned long a3[MAX_NR_CHILDREN];
	unsigned long a4[MAX_NR_CHILDREN];
	unsigned long a5[MAX_NR_CHILDREN];
	unsigned long a6[MAX_NR_CHILDREN];

	unsigned long retval[MAX_NR_CHILDREN];

	unsigned long scratch[MAX_NR_CHILDREN];

	int current_fd;
	unsigned int fd_lifetime;

	/* per-child mmaps */
	struct map *mappings[MAX_NR_CHILDREN];
	unsigned int num_mappings[MAX_NR_CHILDREN];

	/* various flags. */
	bool do32bit[MAX_NR_CHILDREN];
	bool do_make_it_fail;
	bool need_reseed;
	enum exit_reasons exit_reason;

	/* locks */
	volatile unsigned char regenerating;
	volatile unsigned char reaper_lock;

	bool ready;
};
extern struct shm_s *shm;

#define SHM_OK 0
#define SHM_CORRUPT 1
