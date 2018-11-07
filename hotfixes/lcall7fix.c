/*
 * Prosta, g³upia poprawka b³êdu lcall7 na dzia³aj±cych serwerach.
 * Przetestowane na 2.2.20 z PLD. Inne powinny te¿ dzia³aæ. Po prostu
 * usuwa on z tablicy desktyptorów wpis odpowiedzialny za lcall7 i
 * jakakolwiek próba u¿ycia tego spowoduje Bus error.
 *
 * PROGRAM MO¯E ZAWIESIÆ TWOJ¡ MASZYNÊ. URUCHAMIASZ GO NA W£ASNE
 * RYZYKO.
 *
 * 20021114, wojtekka
 */

/*
 * Silly lcall7 hotfix. Tested on 2.2.20 from PLD, but others should work
 * as well. It simply removes lcall7 entry from descriptor table and any
 * use gives Bus error.
 *
 * IT MIGHT HANG YOUR MACHINE. USE AT YOUR OWN RISK.
 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

typedef struct {
	unsigned short limit;
	unsigned long base;
} __attribute__ ((packed)) gdt_t;

typedef unsigned char descr_t[8];

#define descr_base(x) (x[2] | x[3] << 8 | x[4] << 16 | x[7] << 24)
#define descr_access(x) (x[5])
#define descr_limit(x) (x[0] | x[1] << 8 | (x[6] & 15) << 16)

int main()
{
	descr_t d, e[6];
	int fd, i;
	gdt_t g;

	/* wczytaj gdt */
	__asm__ __volatile__("sgdt %0": "=m" (g));

	if ((fd = open("/dev/kmem", O_RDWR)) == -1) {
		perror("open");
		return 1;
	}

	for (i = 0; i < 32; i++) {
		/* czytamy kolejny wpis gdt */
		if (lseek(fd, g.base + i * sizeof(d), SEEK_SET) == -1) {
			perror("lseek");
			return 1;
		}
		if (read(fd, &d, sizeof(d)) != sizeof(d)) {
			perror("read");
			return 1;
		}

		/* pomijamy puste deskryptory */
		if (!(descr_access(d) & 128))
			continue;

		/* szukamy tylko ldt */
		if ((descr_access(d) & 16) || (descr_access(d) & 15) != 2 || (descr_limit(d) < 6))
			continue;

		/* czytamy pierwszy wpis ldt, który robi za call gate */
		if (lseek(fd, descr_base(d), SEEK_SET) == -1) {
			perror("lseek2");
			return 1;
		}
		if (read(fd, &e[0], sizeof(e)) != sizeof(e)) {
			perror("read2");
			return 1;
		}
		
		/* je¶li to nie jest call gate, daj spokój */
		if ((descr_access(e[0]) & 15) != 12)
			continue;

		/* sprawd¼, czy ju¿ nie zrobili¶my tego */
		if (!(descr_access(e[0]) & 128)) {
			printf("lcall7 is already disabled\n");
		} else {
			/* wy³±cz deskryptor */
			descr_access(e[0]) &= ~128;

			printf("lcall7 disabled\n");
		}

		/* je¶li to 2.4 to mamy jeszcze lcall27 */
		if ((descr_access(e[4]) & 15) == 12) {
			if (!(descr_access(e[4]) & 128)) {
				printf("lcall27 is already disabled\n");
			} else {
				descr_access(e[4]) &= ~128;
				printf("lcall27 disabled\n");
			}
		}

		/* zapisz */
		if (lseek(fd, descr_base(d), SEEK_SET) == -1) {
			perror("lseek3");
			return 1;
		}
		if (write(fd, &e[0], sizeof(e)) != sizeof(e)) {
			perror("write");
			return 1;
		}

		goto cleanup;
	}

	printf("Unable to disable lcall7\n");

cleanup:
	close(fd);

	return 0;
}

