#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<unistd.h>
unsigned short checksum(const char *buf, unsigned size);
char* extract_argument(char** argv, int argc, char* arg);

typedef struct MESSAGE{
    short op;
    short shift;
    unsigned short checksum;
    unsigned long long length;
    char* data;
} Message;


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
    /*if (ip == NULL || port == NULL || op = NULL || n = NULL) {
	return 0;
    }
    if (port < 0 || port > 65535 || (op != 0 && op != 1)) {
	return 0;
    }*/
    n = n%26;
    if (n < 0) {
	n = 26 + n;
    }
    printf(ip);
    printf(" ");
    printf("%d", port);
    printf(" ");
    printf("%d", op);
    printf(" ");
    printf("%d", n);
    printf("\n");
    printf("%d", sizeof(unsigned short));
    printf("%d", sizeof(short));
    
    short op1 = (short) op;
    short n1 = (short) n;
    
    printf("%d", op1);
    printf("\n");
    printf("%d", n1);
    printf("\n");    

    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &serv_addr.sin_addr);
    int clientfd;
    if ((clientfd = socket(AF_INET, SOCK_STREAM,0)) < 0) {
	printf("clientfd error");
	return 0;
    }
    if (connect(clientfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
	perror("error occured");
	return 0;
    }
    
    int length = 0;
    char* content;
    char buff[1024];
    content =  (char*) calloc(1, 9999984);
    /*
    while(1) {
	char ch = getchar();
	if (ch == EOF) {
	    break;
	}
	content[length] = ch;
	length++;
    }*/
    
    while(fgets(buff, 1023, stdin) != NULL) {
	int tepLength = strlen(buff);

	for (int i = length; i<length+tepLength; i++) {
	    content[i] = buff[i - length];
	}
	length += tepLength;

	for (int i = 0; i<1024; i++) {
	    buff[i] = 0;
	}
    }

    printf(content);
    Message* message = (Message*) calloc(1, sizeof(Message));
    memset(message, 0, sizeof(Message));
    message->op = htobe16(op1);
    message->shift = htobe16(n1);
    message->length = htobe64(length+16);
    message->data = strdup(content);
    message->checksum = checksum((char*)message,(unsigned) length);
    write(clientfd, (char*)&message->op, 2);
    write(clientfd, (char*)&message->shift, 2);
    write(clientfd, (char*)&message->checksum, 4);
    write(clientfd, (char*)&message->length, 8);
    write(clientfd, &message->data, message->length - 16);
    printf(message->data);
    Message* message1 = (Message*) calloc(1, sizeof(Message));
    int koz2 = read(clientfd, (char*)&message1->op, 2);
    perror("Why the read error occurs");
    printf("%d", koz2);
    read(clientfd, (char*)&message1->shift, 2);
    read(clientfd, (char*)&message1->checksum, 4);
    read(clientfd, (char*)&message1->length, 8);
    /*
    message1->data = (char*)calloc(length, sizeof(char));
    char buf[1024] = {0};
    int nRead = 0;
    printf("Before safe read");
    while(length - nRead) {
	int toRead = length-nRead < 1023 ? length-nRead : 1023;
	read(clientfd, buf, toRead);
	for (int i = nRead; i<nRead + strlen(buf); i++) {
	    message1->data[i] = buf[i-nRead];
	}
	nRead += strlen(buf);

	for(int i = 0; i<1024; i++) {
	    buf[i] = 0;
	}

    }
    
    printf(content);
    free(content);
    printf(message->data);
    printf(message1->data);
    free(message1->data);
    free(message1);
    */
    free(message->data);
    free(message);
    
    close(clientfd);
    return 0;
}




unsigned short checksum(const char *buf, unsigned size) {
    unsigned long long sum = 0;
    //const unsigned long long *b = (unsigned long long *) buf;
    unsigned t1, t2;
    unsigned short t3, t4;
    const unsigned long long *a = (unsigned long long *) buf;
    for (int i = 0; i <= 1; i++) {
	unsigned long long *s = *a++;
        sum += s;
	if (sum < s) sum++;
    }
    const unsigned long long *b = (unsigned long long *) (((Message *)buf)->data);
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
