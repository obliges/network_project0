#include<string.h>
#include<ctype.h>
#include<stdio.h>
#include<stdlib.h>

char* encrypt(char* plainmessage, int n);
char* decrypt(char* ciphermessage, int n);




int main(int argc, char **argv) {
    char koz[100] = "abcdefghijklmnopqrstuvwxyz013%$#FDKF";
    char* a = encrypt(koz, 1);
    char* b = decrypt(koz, 1);
    printf(a);
    free(a);
    printf("\n");
    printf(b);
    free(b);
    printf("\n");
    return 0;
}

char* encrypt(char* plainmessage, int n) {
    int move = n%26;
    char* temp = malloc(sizeof(char) * strlen(plainmessage));
    strcpy(temp, plainmessage);
    //char* temp = plainmessage;
    for (int i = 0; temp[i] != '\0'; i++) {
	if (isalpha(temp[i])) {
	    temp[i] = tolower(temp[i]) - (26-move);
	    if (temp[i] > 'z') {
		temp[i] = temp[i] - 'z' + 'a' - 1;
	    }
	    else if (temp[i] < 'a') {
		temp[i] = temp[i] + 'z' - 'a' + 1;
	    }
	}
	else {
	    continue;
	}
    }
    return temp;
}

char* decrypt(char* ciphermessage, int n) {
    int move = n%26;
    char* temp = malloc(sizeof(char) * strlen(ciphermessage));
    strcpy(temp, ciphermessage);

    //char* temp = ciphermessage;
    for (int i = 0; temp[i] != '\0'; i++) {
	if (isalpha(temp[i])) {
	    temp[i] = tolower(temp[i]) - move;
	    if (temp[i] > 'z') {
		temp[i] = temp[i] - 'z' + 'a' - 1;
	    }
	    else if (temp[i] < 'a') {
		temp[i] = temp[i] + 'z' - 'a' + 1;
	    }
	}
	else {
	    continue;
	}
    }
    return temp;	
}
