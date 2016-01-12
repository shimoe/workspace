/*
  ============================================================================
  Name        : strings.c
  Author      : 
  Version     :
  Copyright   : Your copyright notice
  Description : Hello World in C, Ansi-style
  ============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int str_len(char s[]){
  int i;

  for(i = 0;s[i] != '\0';i++);

  return i;
  //return strlen(s);

}


int count_chars(char s[],char c){
  int i,count = 0;

  for(i = 0;s[i] != '\0';i++){
    if(s[i] == c) count++;  
  }

  return count;
}

int str_eql(char s1[], char s2[]){

  int i;
  for(i = 0;s1[i] != '\0';i++) {
    if (s1[i] != s2[i]) {
      return 0;
    }
  }
  return 1;
  //return strcmp(s1,s2);
}


int str_eql_n(char s1[],char s2[], int n)
{
  int i;
  for (i = 0; i < n; i++) {
    if (s1[i] != s2[i]) {
      return 0;
    }
  }

  if (strlen(s1) > n || strlen(s2) > n) {
    return 0;
  }

  return 1;
  //return strncmp(s1,s2,n);
}

int str_copy(char* s1,char* s2){
  int i,j;
 
  if((int)strlen(s1) > (int)strlen(s2)){
    return 0;
  }
  for(i = 0; s1[i] != '\0'; i++) {
    s2[i] = s1[i];
  }
  for(j = 0;s2[i+j] != '\0';j++){
    s2[i+j] = '\0';
  }
  //strcpy(s1,s2);
}

void string_append(char s1[],char s2[]){
  int i,j;
  char* str;

  str = s1;

  if((int)strlen(s1) + (int)strlen(s2) > (int)strlen(s1)){
    s1 =  malloc((int)strlen(s1) + (int)strlen(s2) + 1);
  }

  for (i = 0; s1[i] != '\0'; i++){
    s1[i] = str[i];
  }

  for (j = 0; s2[j] != '\0'; j++) {
    s1[i] = s2[j];
    i++;
  }
}

int str_take(char s1[], int n, int m, char s2[]){
  int i;
  int j = 0;

  for(i = n;i < n + m;i++) {
    if (s1[i] == '\0'){
      return 0;
    } else {
      s2[j] = s1[i];
      j++;
    }
  }
  s2[j] = '\0';
  return 1;
}

int str_search(char s1[],char s2[]){
  int i,j,k = 0;

  for(i = 0;s1[i] != '\0';i++){
    if(s1[i] == s2[0]){
      for(j = 0;s2[j] != '\0';j++){      
	if(s1[i+j] == s2[j]){
	  k++;
	}
      }
      printf("%d\n",k);
      if(k == strlen(s2)) return i;      
    }
  }

  return -1;    
}

void str_remove(char s1[],int n,int m){
  int i,j;
  j = 0;
  for(i = n;s1[i] != '\0';i++){
    if(n+m+j > strlen(s1)){
      s1[i] = '\0';
    }
    s1[i] = s1[n+m+j];
    j++;

  }
  printf("%s\n",s1);

}

void str_insert(char s1[],int n,char s2[]){
  int i,j;
  char s3[strlen(s1)];

  for(i = 0;s1[i+n] != '\0';i++){
    s3[i] = s1[i+n];
    s1[i+n] = '\0';
  }
  strcat(s1,s2);
  strcat(s1,s3);
  printf("%s\n",s1);
}

void str_subst(char s1[],char s2[],char s3[]){
  int i,j,n;

  n = str_search(s1,s2);
  printf("%d\n",n);
  str_remove(s1,n,strlen(s2));
  str_insert(s1,n,s3);
  printf("%s\n",s1);
}

int main(void) {

  
char s1[100]="All you need is XX.";

 str_subst(s1, "YYY", "hate");

}



