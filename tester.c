#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "the2.h"

Environment *init_environment2(char *user_file_name);
FILE *fp;

char *rstrip(char *string);
void print_con_begin(User *user) {
    Connection *connection;
    int i;
    for (i = 0; i < BUCKET_SIZE; i++) {
        printf("Bucket %d: ", i);
        fprintf(fp,"Bucket %d: ", i);
        if (user->connections[i] != NULL) {
            connection = user->connections[i];
            do {
              printf("[with:%s time:%ld] -> ", connection->user2->name,
                       connection->connection_time);
              fprintf(fp,"[with:%s time:%ld] -> ", connection->user2->name,
                       connection->connection_time);

            } while ((connection = connection->next_connection) != NULL);
        }
        printf("NULL\n");
        fprintf(fp,"NULL\n");


    }
}

void print_con_end(User *user) {
    Connection *connection;
    int i;
    for (i = 0; i < BUCKET_SIZE; i++) {
        printf("Reverse %d: ", i);
        fprintf(fp,"Reverse %d: ", i);
        if (user->connections[i] != NULL) {
            connection = user->connections[i];
            printf("NULL <- ");
            fprintf(fp,"NULL <- ");
            while(connection->next_connection!=NULL){
              connection=connection->next_connection;
            }
            do {
                printf("[with:%s time:%ld] <- ", connection->user2->name,
                       connection->connection_time);
                fprintf(fp,"[with:%s time:%ld] <- ", connection->user2->name,
                       connection->connection_time);

            } while ((connection = connection->prev_connection) != NULL);
            printf("(START)\n" );
            fprintf(fp,"(START)\n" );
        }else
        printf("NULL\n");
        fprintf(fp,"NULL\n");


    }
}
void print_bucket(User *user){
  printf("Connection list for %s:\n",user->name);
  fprintf(fp,"Connection list for %s:\n",user->name);
  print_con_begin(user);
  print_con_end(user);
  printf("\n");
  fprintf(fp,"\n");
}
void print_common_connections(Environment environment,int id1, int id2){
  int i=0;
  User **arr;
  User * user1;
  User * user2;
  printf("----\n");
  fprintf(fp,"----\n");
  arr=get_common_connections(environment,user1=get_user(environment,id1),user2=get_user(environment,id2));
  if(!arr[i]){
    printf(" [No Common]\n [between id %d(%s) and id %d(%s)]\n",id1,user1->name,id2,user2->name);
    fprintf(fp," [No Common]\n [between id %d(%s) and id %d(%s)]\n",id1,user1->name,id2,user2->name);
  }else{
    printf("Common connections of %d and %d:\n",id1,id2);
    printf("  --%s-- && --%s--\n",user1->name,user2->name);
    fprintf(fp,"Common connections of %d and %d:\n",id1,id2);
    fprintf(fp,"  --%s-- && --%s--\n",user1->name,user2->name);

  }
  while(arr[i]){
    printf("  name:  %-10s id: %-3d\n",arr[i]->name,arr[i]->id);
    fprintf(fp,"  name:  %-10s id: %-3d\n",arr[i]->name,arr[i]->id);
    i++;
  }
}
void print_user(Environment environment ,int id1){
  User *temp_user;
  temp_user = get_user(environment, id1);
  printf("User-> id: %d name: %s hash: %lu", temp_user->id, temp_user->name, hash_code(temp_user));
  fprintf(fp,"User-> id: %d name: %s hash: %lu", temp_user->id, temp_user->name, hash_code(temp_user));
  printf("\n");
  fprintf(fp,"\n");
}
int * count_connections(User * user){
  int * arr;
  int i;
  arr=(int *)malloc(sizeof(int)*BUCKET_SIZE);
  for(i=0;i<BUCKET_SIZE;i++){
    arr[i]=count_con(user->connections[i]);
  }
  return arr;
}
void print_con_num(Environment environment,int id){
  int i;
  int *arr;
  User *temp_user;
  arr=count_connections(temp_user=get_user(environment,id));
  printf(" [%s,%d]: ",temp_user->name,temp_user->id);
  fprintf(fp," [%s,%d]: ",temp_user->name,temp_user->id);
  for(i=0;i<BUCKET_SIZE;i++){
    printf("%d ",arr[i]);
    fprintf(fp,"%d ",arr[i]);
  }
  printf("\n");
  fprintf(fp,"\n");
}
void print_environment(Environment * environment){
  int i=0;
  printf("\n");
  fprintf(fp,"\n");
  while(environment->users[i]){
    print_user(*environment,environment->users[i++]->id);
  }
  printf("\n");
  fprintf(fp,"\n");
  i=0;
  while(environment->users[i]){
    print_con_num(*environment,environment->users[i++]->id);
  }
  fprintf(fp,"\n");
}
int count_con(Connection * connection){
  int i=0;
  while(connection){
    i++;
    connection=connection->next_connection;
  }
  return i;
}
void print_all_buckets(Environment * environment){
  int i=0;
  printf("\n");
  fprintf(fp,"\n");
  while(environment->users[i]){
    print_bucket(get_user(*environment,environment->users[i++]->id));
  }
}
int ** get_combination(int * arr,int size){
  int * dyn;
  int ** dynarr=NULL;
  int i,j;
  int counter=1;
  for(i=0;i<size;i++){
    for(j=i+1;j<size;j++){
      dyn=calloc(2,sizeof(int));
      dyn[0]=arr[i];
      dyn[1]=arr[j];
      dynarr=realloc(dynarr,sizeof(int*)*(counter+1));
      dynarr[counter-1]=dyn;
      dynarr[counter]=NULL;
      counter++;
    }
  }
  return dynarr;
}
void print_all_commons(Environment *environment){
  int **arr;
  int *ids=NULL;
  int i;
  int counter=0;
  for(i=0;environment->users[i];i++){
    ids=realloc(ids,sizeof(int)*(counter+1));
    ids[counter++]=environment->users[i]->id;
  }

  for(i=0,arr=get_combination(ids,counter);arr[i];i++){
    print_common_connections(*environment,arr[i][0],arr[i][1]);
  }
}






int main() {
    Environment* environment;
    Environment* environmentAfter;
    User *temp_user;
    fp = fopen("testdata", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);
    fp = fopen("testoutput.txt","w");
    environment = init_environment2("testdata");
    return 0;
}








void print_all(Environment* environment){
  print_environment(environment);
  print_all_commons(environment);
  print_all_buckets(environment);
}
Environment *init_environment2(char *user_file_name) {
    Environment *environment;
    User *user;
    FILE *fp;
    char *line = NULL;
    char *iter;
    char *sep = " ";
    size_t len = 0;
    size_t read;
    int id, id1, id2;
    long timestamp;
    char *name;
    int ff=0;
    char *strtemp;
    int u_count = 0;

    environment = malloc(sizeof(Environment));
    environment->users = malloc(sizeof(User*));
    environment->users[0] = NULL;

    fp = fopen(user_file_name, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, fp)) != -1) {
        line = rstrip(line);
        if(line[0]=='\0')continue;
        if (strcmp(line, "#Users#") == 0) {
            continue;
        } else if (strcmp(line, "#Connections#") == 0) {
            break;
        }
        user = malloc(sizeof(User));
        iter = strtok(line, sep);
        id = atoi(iter);
        iter = strtok(NULL, sep);
        name = malloc(sizeof(char)*strlen(iter));
        strcpy(name, iter);
        user->id=id;
        user->name=name;
        environment->users = realloc(environment->users, sizeof(User*)*(u_count+2));
        environment->users[u_count] = user;
        environment->users[u_count+1] = NULL;
        u_count++;
    }
    while ((read = getline(&line, &len, fp)) != -1) {
        line = rstrip(line);
        if(strncmp(line,"#",1)==0)continue;
        if(line[0]=='\0')continue;
        if (strncmp("Rem", line, 3) == 0) {
          strtok(line, sep);
          iter = strtok(NULL, sep);
          id1 = atoi(iter);
          iter = strtok(NULL, sep);
          id2 = atoi(iter);
          remove_connection(*environment,id1,id2);
        }else if(strncmp("Com", line, 3) == 0){
          strtok(line, sep);
          iter = strtok(NULL, sep);
          id1 = atoi(iter);
          iter = strtok(NULL, sep);
          id2 = atoi(iter);
          print_common_connections(*environment,id1,id2);
        }else if(strncmp("Print", line, 5) == 0){
          strtok(line, sep);
          strtemp = strtok(NULL, sep);
          if(strcmp(strtemp,"environment")==0){
            print_environment(environment);
          }
          if(strcmp(strtemp,"user")==0){
            iter = strtok(NULL, sep);
            id1 = atoi(iter);
            print_bucket(get_user(*environment,id1));
          }
          if(strcmp(strtemp,"commons")==0){
            print_all_commons(environment);
          }
          if(strcmp(strtemp,"everything")==0){
            print_all(environment);
          }
        }else{
        iter = strtok(line, sep);
        id1 = atoi(iter);
        iter = strtok(NULL, sep);
        id2 = atoi(iter);
        iter = strtok(NULL, sep);
        timestamp = atol(iter);
        connect_users(*environment, id1, id2, timestamp);
      }
    }

    return environment;

}
