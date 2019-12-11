#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "the2.h"

/*
 *  returns the order of the letter in the alphabet
 *  if given char is not an ascii letter, returns 0
 */
int letter_order(char c){
    if (c < 91 && c > 64)
    {
        return c - 64;
    }
    else if (c < 123 && c > 96)
    {
        return c - 96;
    }
    return 0;
}
/*
 * Prints the connections of given user.
 */
void print_connections(User *user){
    Connection *connection;
    int i;
    for (i = 0; i < BUCKET_SIZE; i++)
    {
        printf("Bucket %d: ", i);
        if (user->connections[i] != NULL)
        {
            connection = user->connections[i];
            do
            {
                printf("[user1: %s, user2: %s, connection_time: %ld] -> ", connection->user1->name,
                       connection->user2->name, connection->connection_time);

            } while ((connection = connection->next_connection) != NULL);
        }
        printf("NULL\n");
    }
}

/*
 * Returns the hash code of the user.
 */
unsigned long hash_code(User *user){
    unsigned long hash = 0;
    int i = 0;

    while ((*user).name[i] != 0)
    {
        hash += letter_order((*user).name[i]) * pow(HASH_KEY, i);
        i++;
    }

    return hash;
}

/*
 * Returns the User with given id from environment.
 */
User *get_user(Environment environment, int id){
    int i;
    for (i = 0; environment.users[i] != NULL; i++)
    {
        if (environment.users[i]->id == id)
        {
            return environment.users[i];
        }
    }
    return 0;
}

/*
 * Connects two user and registers the related connection objects to both users' connection hash tables.
 */
void connect_users(Environment environment, int id1, int id2, long connection_time){
    if( (get_connection(environment, id1, id2)) == NULL ){
    User *new_user1, *new_user2;
    int k = 0, j = 0;
    unsigned long hash1 = hash_code(get_user(environment, id1));
    unsigned long hash2 = hash_code(get_user(environment, id2));
    int index1 = hash2 % BUCKET_SIZE;
    int index2 = hash1 % BUCKET_SIZE;
    Connection *user1_cn, *user2_cn;
    user1_cn = (Connection*)malloc(sizeof(Connection));
    user2_cn = (Connection*)malloc(sizeof(Connection));
    new_user1 = get_user(environment, id1);
    new_user2 = get_user(environment, id2);

    user1_cn->prev_connection = NULL;
    user1_cn->next_connection = NULL;
    user1_cn->connection_time = connection_time;
    user1_cn->user1 = new_user1;
    user1_cn->user2 = new_user2;
    user2_cn->prev_connection = NULL;
    user2_cn->next_connection = NULL;
    user2_cn->connection_time = connection_time;
    user2_cn->user1 = new_user2;
    user2_cn->user2 = new_user1;
    if (new_user1->connections[index1] != NULL){
        k++;
    }
    if (new_user2->connections[index2] != NULL){
        Connection * iter2 = new_user2->connections[index1];
        j++;
    }
    if (k == 0 && j == 0){
        new_user1->connections[index1] = user1_cn;
        new_user2->connections[index2] = user2_cn;
    }

    if (k != 0 && j == 0){
        Connection * iter1 = new_user1->connections[index1];
        if (new_user1->connections[index1]->connection_time > connection_time){
            user1_cn->next_connection = new_user1->connections[index1];
            new_user1->connections[index1]->prev_connection = user1_cn;
            new_user1->connections[index1] = user1_cn;
        }
        if (new_user1->connections[index1]->connection_time < connection_time){
            for(; iter1->next_connection != NULL && iter1->connection_time < connection_time ;){
                iter1 = iter1->next_connection;
            }
        
            if(iter1->next_connection == NULL && iter1->connection_time < connection_time){
                iter1->next_connection= user1_cn;
                user1_cn->prev_connection = iter1;
            }
            else{
            iter1->prev_connection->next_connection = user1_cn;
            user1_cn->prev_connection = iter1->prev_connection;
            user1_cn->next_connection = iter1;
            iter1->prev_connection = user1_cn;
            }
        }
        new_user2->connections[index2] = user2_cn;
    }

    if (k == 0 && j != 0){
        Connection * iter2 = new_user2->connections[index2];
        if (new_user2->connections[index2]->connection_time > connection_time){
            user2_cn->next_connection = new_user2->connections[index2];
            new_user2->connections[index2]->prev_connection = user2_cn;
            new_user2->connections[index2] = user2_cn;
        }
        if (new_user2->connections[index2]->connection_time < connection_time){
            for(; iter2->next_connection != NULL && iter2->connection_time < connection_time ;){
                iter2 = iter2->next_connection;
            }
        
            if(iter2->next_connection == NULL && iter2->connection_time < connection_time){
                iter2->next_connection= user2_cn;
                user2_cn->prev_connection = iter2;
            }
            else{
            iter2->prev_connection->next_connection = user2_cn;
            user2_cn->prev_connection = iter2->prev_connection;
            user2_cn->next_connection = iter2;
            iter2->prev_connection = user2_cn;
            }
        }
        new_user1->connections[index1] = user1_cn;
    }

    if (k != 0 && j != 0){
        Connection * iter1 = new_user1->connections[index1];
        Connection * iter2 = new_user2->connections[index2];
        if (new_user1->connections[index1]->connection_time > connection_time){
            user1_cn->next_connection = new_user1->connections[index1];
            new_user1->connections[index1]->prev_connection = user1_cn;
            new_user1->connections[index1] = user1_cn;
        }
        if (new_user1->connections[index1]->connection_time < connection_time){
            for(; iter1->next_connection != NULL && iter1->connection_time < connection_time ;){
                iter1 = iter1->next_connection;
            }
        
            if(iter1->next_connection == NULL && iter1->connection_time < connection_time){
                iter1->next_connection= user1_cn;
                user1_cn->prev_connection = iter1;
            }
            else{
            iter1->prev_connection->next_connection = user1_cn;
            user1_cn->prev_connection = iter1->prev_connection;
            user1_cn->next_connection = iter1;
            iter1->prev_connection = user1_cn;
            }
        }
        if (new_user2->connections[index2]->connection_time > connection_time){
            user2_cn->next_connection = new_user2->connections[index2];
            new_user2->connections[index2]->prev_connection = user2_cn;
            new_user2->connections[index2] = user2_cn;
        }
        if (new_user2->connections[index2]->connection_time < connection_time){
            for(; iter2->next_connection != NULL && iter2->connection_time < connection_time ;){
                iter2 = iter2->next_connection;
            }
        
            if(iter2->next_connection == NULL && iter2->connection_time < connection_time){
                iter2->next_connection= user2_cn;
                user2_cn->prev_connection = iter2;
            }
            else{
            iter2->prev_connection->next_connection = user2_cn;
            user2_cn->prev_connection = iter2->prev_connection;
            user2_cn->next_connection = iter2;
            iter2->prev_connection = user2_cn;
            }
        }
    }
    }
}

/*
 * Rstrip the given string.
 */

char *rstrip(char *string){
    char *end;

    while (isspace((unsigned char)*string))
    {
        string++;
    }

    if (*string == 0)
    {
        return string;
    }

    end = string + strlen(string) - 1;
    while (end > string && isspace((unsigned char)*end))
    {
        end--;
    }

    end[1] = '\0';

    return string;
}

/*
 * Creates a new environment with the information in the given file that contains users
 * with connections and returns it.
 */
Environment *init_environment(char *user_file_name){
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
    User *user1, *user2;

    int u_count = 0;

    environment = malloc(sizeof(Environment));
    environment->users = malloc(sizeof(User *));
    environment->users[0] = NULL;

    fp = fopen(user_file_name, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, fp)) != -1)
    {
        line = rstrip(line);
        if (strcmp(line, "#Users#") == 0)
        {
            continue;
        }
        else if (strcmp(line, "#Connections#") == 0)
        {
            break;
        }
        user = malloc(sizeof(User));
        iter = strtok(line, sep);
        id = atoi(iter);
        iter = strtok(NULL, sep);
        name = malloc(sizeof(char) * strlen(iter));
        strcpy(name, iter);
        user->id = id;
        user->name = name;
        environment->users = realloc(environment->users, sizeof(User *) * (u_count + 2));
        environment->users[u_count] = user;
        environment->users[u_count + 1] = NULL;
        u_count++;
    }
    /* Uncomment after connect_users function implemented. */
    while ((read = getline(&line, &len, fp)) != -1) {
        line = rstrip(line);
        iter = strtok(line, sep);
        id1 = atoi(iter);
        iter = strtok(NULL, sep);
        id2 = atoi(iter);
        iter = strtok(NULL, sep);
        timestamp = atol(iter);
        connect_users(*environment, id1, id2, timestamp);
    }
    return environment;
}

/*
 * Returns the connection between users with id1 and id2. The connection
 * is the one stored in user1's hash table. If there is no such a connection returns
 * NULL.
 */
Connection *get_connection(Environment environment, int id1, int id2){
    User *new_user1, *new_user2;
    unsigned long hash2 = hash_code(get_user(environment, id2));
    int index1 = hash2 % BUCKET_SIZE;
    new_user1 = get_user(environment, id1);
    new_user2 = get_user(environment, id2);

    if(new_user1->connections[index1]==NULL){ 
        return NULL;
    }
    else{
        Connection * iter1 = new_user1->connections[index1];
        for(;iter1->next_connection!=NULL;){
        if(iter1->user2->id == new_user2->id ){
            return iter1;
        }
        else{
            iter1 = iter1->next_connection;
        }
        }
        if(iter1->next_connection==NULL){
            if(iter1->user2->id == new_user2->id){
                return iter1;
            }
        }  
    }
    return NULL;
}
/*
 * Remove connection from the given user. Additionally, finds the other user and removes related connection
 * from her/his table also.
 */
void remove_connection(Environment environment, int id1, int id2){
    User *new_user1, *new_user2;
    unsigned long hash1 = hash_code(get_user(environment, id1));
    unsigned long hash2 = hash_code(get_user(environment, id2));
    int index1 = hash2 % BUCKET_SIZE;
    int index2 = hash1 % BUCKET_SIZE;
    new_user1 = get_user(environment, id1);
    new_user2 = get_user(environment, id2);


    if(get_connection(environment,id1,id2)==NULL){
        return;
    }
    if(get_connection(environment,id1,id2)!=NULL && get_connection(environment,id1,id2)->prev_connection==NULL){
        new_user1->connections[index1] = get_connection(environment,id1,id2)->next_connection;
        free(get_connection(environment,id1,id2));
    }
    if(get_connection(environment,id1,id2)!=NULL && get_connection(environment,id1,id2)->next_connection==NULL && get_connection(environment,id1,id2)->prev_connection!=NULL){
        get_connection(environment,id1,id2)->prev_connection->next_connection = NULL;
        free(get_connection(environment,id1,id2));
    }
    if(get_connection(environment,id1,id2)!=NULL && get_connection(environment,id1,id2)->next_connection!=NULL && get_connection(environment,id1,id2)->prev_connection!=NULL){
        get_connection(environment,id1,id2)->prev_connection->next_connection = get_connection(environment,id1,id2)->next_connection;
        get_connection(environment,id1,id2)->next_connection->prev_connection = get_connection(environment,id1,id2)->prev_connection;
        free(get_connection(environment,id1,id2));
    }


    if(get_connection(environment,id2,id1)==NULL){
        return;
    }
    if(get_connection(environment,id2,id1)!=NULL && get_connection(environment,id2,id1)->prev_connection==NULL){
        new_user2->connections[index2] = get_connection(environment,id2,id1)->next_connection;
        free(get_connection(environment,id2,id1));
    }
    if(get_connection(environment,id2,id1)!=NULL && get_connection(environment,id2,id1)->next_connection==NULL && get_connection(environment,id2,id1)->prev_connection!=NULL){
        get_connection(environment,id2,id1)->prev_connection->next_connection = NULL;
        free(get_connection(environment,id2,id1));
    }
    if(get_connection(environment,id2,id1)!=NULL && get_connection(environment,id1,id2)->next_connection!=NULL && get_connection(environment,id2,id1)->prev_connection!=NULL){
        get_connection(environment,id2,id1)->prev_connection->next_connection = get_connection(environment,id2,id1)->next_connection;
        get_connection(environment,id2,id1)->next_connection->prev_connection = get_connection(environment,id2,id1)->prev_connection;
        free(get_connection(environment,id2,id1));
    }

}

/*
 * Returns dynamic User array which contains common connections. The last element
 * of the array is NULL, which is mandatory for detecting the end.
 */
User **get_common_connections(Environment environment, User *user1, User *user2){
    User ** my_users;
    Connection *iter1;
    Connection *iter2;
    int i,j,k=1,l=1;
    my_users = (User**)malloc(sizeof(User *) * (k+1));
    my_users[k-1] = (User*)malloc(sizeof(User));
    my_users[k] = NULL;
    
    for(i=0;i<BUCKET_SIZE;i++){
        if(user1->connections[i]==NULL){
            continue;
        }
        else if(user1->connections[i]!=NULL){
            iter1 = user1->connections[i];
            for(;iter1!=NULL;){
                for(j=0;j<BUCKET_SIZE;j++){
                    if(user2->connections[j]==NULL){
                        continue;
                    }
                    else if(user2->connections[j]!=NULL){
                        iter2 = user2->connections[j];
                        for(;iter2!=NULL;){
                            if(iter1->user2->id == iter2->user2->id){
                                my_users = realloc(my_users, sizeof(User*) * (k+1));
                                my_users[k-1] = (User*)malloc(sizeof(User));
                                my_users[k-1] = iter1->user2;
                                my_users[k] = NULL;
                                k++;
                            }
                            iter2 = iter2->next_connection;
                        }
                    }
                }
                iter1 = iter1->next_connection;
            }    
        }
    }
    if(k==1){
            my_users = (User**)malloc(sizeof(User *));
            my_users[k-1] = (User*)malloc(sizeof(User));
            my_users[0] = NULL;
        }
    return my_users;
}
