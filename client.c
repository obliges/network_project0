#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
char* protocol(char* message, int op, int shift, int checksum);
unsigned short checksum(const char *buf, unsigned size);
char* extract_argument(char** argv, int argc, char* arg);

int main(int argc, char **argv) {
    struct sockaddr_in serv_addr;
    if (argc != 9) {
	return 0;
    }
    //char* ip = argv[2];
    char* ip = extract_argument(argv, argc, "-h");    
    //int port = atoi(argv[4]);
    int port = atoi(extract_argument(argv, argc, "-p"));
    //int op = atoi(argv[6]);
    int op = atoi(extract_argument(argv, argc, "-o"));
    //int n = atoi(argv[8]);
    int n = atoi(extract_argument(argv, argc, "-s"));
    printf(ip);
    printf(" ");
    printf("%d", port);
    printf(" ");
    printf("%d", op);
    printf(" ");
    printf("%d", n);
    printf("\n");
    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &serv_addr.sin_addr);
    int clientfd = socket(AF_INET, SOCK_STREAM,0);
    connect(clientfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    //ssize_t send(int socket, const void *buffer, size_t length, int flags);
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

char* extract_argument(char** argv, int argc, char* arg) {
    for (int i = 1; i < argc; i = i+2) {
	if (strcmp(argv[i], arg) == 0) {
	    return argv[i+1];
	}
    }
}
