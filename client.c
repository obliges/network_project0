#include<stdio.h>


char* protocol(char* message, int op, int shift, int checksum);
unsigned short checksum(const char *buf, unsigned size);

int main(int argc, char **argv) {
    if (argc != 9) {
	return 0;
    }
    char* ip = argv[2];
    int port = atoi(argv[4]);
    int op = atoi(argv[6]);
    int n = atoi(argv[8]);
    printf(ip);
    printf(" ");
    printf("%d", port);
    printf(" ");
    printf("%d", op);
    printf(" ");
    printf("%d", n);
    printf("\n");

    return 0;
}



char* protocol(char* message, int op, int n, int checksum) {

}

unsigned short checksum(const char *buf, unsigned size) {
    unsigned long long sum = 0;
    const unsigned long long *b = (unsigned long long *) buf;

    unsigned t1, t2;
    unsigned short t3, t4;

    while (size >= sizeof(unsigned long long)) {
	unsigned long long s = * b++;
	sum += s;
	if (sum < s) sum++;
	size -= 8;
    }

    buf = (const char *) b;
    if (size & 4) {
	unsigned s = *(unsigned *)buf;
	sum += s;
	if (sum < s) sum++;
	buf += 4;
    }

    if (size & 2) {
	unsigned short s = *(unsigned short *) buf;
	sum += s;
	if (sum < s) sum++;
	buf += 2;
    }

    if (size) {
	unsigned char s = *(unsigned char *) buf;
	sum += s;
	if (sum < s) sum++;
    }

    t1 = sum;
    t2 = sum >> 32;
    t1 += t2;
    if (t1 < t2) t1++;
    t3 = t1;
    t4 = t1 >> 16;
    t3 += t4;
    if (t3 < t4) t3++;

    return -t3;
}
