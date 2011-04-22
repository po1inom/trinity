#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "trinity.h"

unsigned int socket_fds[MAX_FDS];
unsigned int socks=0;

static int spin=0;
static char spinner[]="-\\|/";

static char *cachefilename="trinity.socketcache";

#define TYPE_MAX 128
#define PROTO_MAX 256

#define MAX_PER_DOMAIN 5
#define MAX_TRIES_PER_DOMAIN 10
static char sockarray[PF_MAX];

void generate_sockets(unsigned int nr_to_create)
{
	int fd;
	unsigned int i, tries;
	int cachefile;

	unsigned int domain, type, protocol;
	unsigned int buffer[3];

	cachefile = creat(cachefilename, S_IWUSR|S_IRUSR);
	if (cachefile < 0) {
		printf("Couldn't open cachefile for writing! (%s)\n",
			strerror(errno));
		exit(EXIT_FAILURE);
	}

	while (socks < nr_to_create) {
		for (i = 0; i < PF_MAX; i++)
			sockarray[i] = 0;

		for (i = 0; i < PF_MAX; i++) {
			tries = 0;

			if (sockarray[i] == MAX_PER_DOMAIN)
				break;

			domain = i;
			type = rand() % TYPE_MAX;
			protocol = rand() % PROTO_MAX;

			printf("%c (%d sockets created. needed:%d) [domain:%d type:%d proto:%d]    \r",
				spinner[spin++], socks, nr_to_create-socks,
				domain, type, protocol);
			if (spin == 4)
				spin = 0;

			fd = socket(domain, type, protocol);
			if (fd > -1) {
				socket_fds[socks] = fd;

				writelog_nosync("fd[%i] = domain:%i type:%i protocol:%i\n",
					fd, domain, type, protocol);

				sockarray[i]++;
				socks++;

				buffer[0] = domain;
				buffer[1] = type;
				buffer[2] = protocol;
				write(cachefile, &buffer, sizeof(int) * 3);

				if (socks == nr_to_create)
					goto done;
			} else {
				tries++;
			}
			if (tries == MAX_TRIES_PER_DOMAIN)
				break;
		}
	}

done:
	close(cachefile);
	printf("\ncreated %d sockets\n", socks);
	writelog("created %d sockets\n\n", socks);
}

void open_sockets()
{
	int cachefile;
	unsigned int domain, type, protocol;
	unsigned int buffer[3];
	unsigned int i;
	int bytesread=-1;
	int fd;

	cachefile = open(cachefilename, O_RDONLY);
	if (cachefile < 0) {
		printf("Couldn't find socket cachefile. Regenerating.\n");
		generate_sockets(MAX_FDS/2);
		return;
	}

	while (bytesread != 0) {
		bytesread = read(cachefile, buffer, sizeof(int) * 3);
		if (bytesread == 0)
			break;

		domain = buffer[0];
		type = buffer[1];
		protocol = buffer[2];

		fd = socket(domain, type, protocol);
		if (fd < 0) {
			printf("Cachefile is stale. Need to regenerate.\n");
			close(cachefile);
			unlink(cachefilename);

			for (i = 0; i < socks; i++) {
				close(socket_fds[i]);
				socket_fds[i] = 0;
				fd_idx--;
			}
			socks = 0;

			generate_sockets(MAX_FDS/2);
			return;
		}
		socket_fds[socks] = fd;
		writelog_nosync("fd[%i] = domain:%i type:%i protocol:%i\n",
			socks+fd_idx, domain, type, protocol);
		socks++;
	}
	if (socks < MAX_FDS/2) {
		printf("Insufficient sockets in cachefile (%d). Regenerating.\n", socks);
		generate_sockets(MAX_FDS/2);
		return;
	}

	printf("(%d sockets created based on info from socket cachefile.)\n", socks);
	synclog();

	close(cachefile);
}