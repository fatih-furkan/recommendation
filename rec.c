#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <math.h>
#include <time.h>


//User ids start from 1 and they are numareted from 1 to USER_NUM, film ids are from 1 to MOVIE_NUM
//All the numbers between 1 and USER_NUM must be used, same for the MOVIE_NUM with film ids
//The file has MAX_LINE lines
#define MAX_LINE 1027
#define USER_NUM 99
#define MOVIE_NUM 99
#define REC_ARR_SIZE 20

//Node structure for users and movies
struct node {
    int type; //0 for user, 1 for film, 2 means it is not important
    int id;
    int point; //0 for main nodes
    struct node* next;
};

//Creates a struct node node
struct node* createNode(int id, int type, int point) {
    struct node* node = malloc(sizeof(struct node));
    node->id = id;
    node->next = NULL;
    node->type = type;
    node->point = point;
    return node;
}

//Adds new node to the linked list that have root as its root and returns the root. If root is NULL then new node becomes the root.
struct node* addNodeNode(struct node* root, int id, int point, int type) {
    struct node* new_node = createNode(id, type, point);
    if(root == NULL) {
        return new_node;
    }
    //if list given is empty, new node becomes root

    struct node* temp = root;
    while(temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = new_node;
    return root;
    //else new_node is added at the end of the list given.
}

//Searches a node who has num as id in the linked list given.
struct node* searchNode(struct node* root, int num) {
    if(root == NULL) {
        return NULL;
    }
    struct node* temp = root;

    while(temp != NULL) {

        if(temp->id == num) {
            return temp; 
        }
        temp = temp->next;
    }
    return NULL;
}

//Deletes the node with the given id
struct node* deleteNode(struct node* root, int id) {
    if(root == NULL) {
        printf("The list does not contain any element!\n");
        return NULL;
    }

    struct node* temp = root;
    struct node* pre = NULL;

    if(temp->id == id) {
        pre = temp->next;
        free(temp);
        return pre;
    }

    while(temp != NULL) {
        if(temp->id == id) {
            pre->next = temp->next;
            free(temp);
        }
        pre = temp;
        temp = temp->next;

    }
    return root;
}

//Fills the array "watched" with the movies watched by user and returns the number of elements of the array. Does not order movies.
int watchedFilms(int user, struct node* users[USER_NUM+1], int watched[MOVIE_NUM]) {
    if(user > USER_NUM || user < 1) {
        printf("User given to watchedFilms is not in the right interval!\n");
        return 0;
    }

    struct node* temp = users[user];
    if(temp->next != NULL) temp = temp->next;
    int i = 0;
    while(temp != NULL) {
        watched[i] = temp->id;
        temp = temp->next;
        i++;
    }
    return i; //number of elements
}

//Orders the linked list of struct nodes that it takes (inbuilt)
void selectionSort(struct node* root) {
    if(root == NULL) return;
    if(root->next == NULL) return;

    struct node* i = root;
    struct node* j = NULL;
    struct node* max = root;
    while(i != NULL) {
        max = i;
        j = i->next;

        while(j != NULL) {
            if(j->point > max->point) {
                max = j;
            }
            j = j->next;
        }

        int id = i->id;
        int point = i->point;
        int type = i->type;
        i->id = max->id;
        i->point = max->point;
        i->type = max->type;
        max->id = id;
        max->point = point;
        max->type = type;

        i = i->next;
    }
    return;
}

//Returns a linked list that contains all films watched by user. The films are ordered by point given by the user. (Descending order)
struct node* watchedFilms2(int user, struct node* users[USER_NUM+1]) {
    if(user > USER_NUM || user < 1) return NULL;

    struct node* temp = users[user];
    if(temp->next == NULL) {
        printf("Il n'a pas regarde aucun film.\n");
        return NULL;
    }
    else temp = temp->next;
    int i = 0;
    struct node* root = NULL;
    while(temp != NULL) {
        root = addNodeNode(root, temp->id, temp->point, temp->type);
        temp = temp->next;
        i++;
    }
    selectionSort(root);
    return root;
}

//Returns random films that the user didn't watch.
void randomRec(int user, struct node* users[USER_NUM+1], int recs[REC_ARR_SIZE], int rec_num) {
    if(user > USER_NUM || user < 1) {
        printf("User is not in the right interval!\n");
        return;
    }

    int watched[MOVIE_NUM];
    int element_num = watchedFilms(user, users, watched);

    srand(time(0));
    int j = 0;
    while(1) {
        if(j > rec_num-1) {
            return;
        }
        int found = 0;
        int num = abs(10000*(rand())) % MOVIE_NUM+1;
        for(int j = 0; j < element_num; j++) {
            if(num == watched[j]) {
                found = 1;
                break;
            }
        }
        if(found == 0) {
            recs[j] = num;
            j++;
        }
    }
}


//a simple swap operation between two integers
void swapQuick(int* a, int* b) {
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

//quickSort helper
int partition(int* distances, int* ids, int l, int r) {
    int pivot = r;
    int kac_ileri = 0;
    for(int i = l; i < r; i++) {
        if(distances[i] < distances[pivot]) {
            swapQuick(&distances[i], &distances[l + kac_ileri]);
            swapQuick(&ids[i], &ids[l + kac_ileri]);
            kac_ileri++;
        }
    }
    swapQuick(&distances[pivot], &distances[l+kac_ileri]);
    swapQuick(&ids[pivot], &ids[l+kac_ileri]);

    return(l+kac_ileri);
}

//Takes two arrays of integer and while sorting distances, makes same operations for ids
//for the purpose of being able to maintain connections of ids and distances.
void quickSort(int* distances, int* ids, int l, int r) {
    if(l < r) {
        int pivot = partition(distances, ids, l, r);
        quickSort(distances, ids, l , pivot - 1);
        quickSort(distances, ids, pivot + 1, r);
    }
}



//finds number of viewers of a film given
int watchedBy(int movie, struct node* movies[MOVIE_NUM+1]) {
    if(movie > MOVIE_NUM || movie < 1) {
        printf("Error, movie is not in the right interval\n");
        return -1;
    }
    struct node* temp = movies[movie]->next;
    int i = 0;
    while(temp != NULL) {
        i++;
        temp = temp->next;
    }
    return i;
}

//Fills empty most_popular array with most watched films that user didn't watch already.
//most_popular must contain more elements than rec_num.
void mostWatchedRec(int user, struct node* movies[MOVIE_NUM+1], struct node* users[USER_NUM+1], int most_popular[REC_ARR_SIZE], int rec_num) {
    if(user > USER_NUM || user < 1) {
        printf("User is not in the right interval!\n");
        return;
    }

    int views[MOVIE_NUM+1];
    int ids[MOVIE_NUM+1];

    //ids array is initialized
    for(int i = 0; i<MOVIE_NUM+1; i++) {
        ids[i] = i;
    }

    int watched[MOVIE_NUM];
    int elm_num = watchedFilms(user, users, watched);

    //views array is filled with viewer numbers
    for(int i = 1 ; i< MOVIE_NUM+1; i++) {
        views[i] = watchedBy(i, movies);
    }

    //lists are sorted from big to small
    quickSort(views, ids, 1, MOVIE_NUM);
    
    int k = 0;
    for(int i = MOVIE_NUM; i>0; i--) {
        if(k > rec_num-1) break;
        for(int j = 0; j<elm_num; j++) {
            if(ids[i] == watched[j]) {
                goto end;
            }
        }
        most_popular[k] = ids[i];
        k++;
        end: ;
    }
}


//----------------------------

//A simple struct for calculating similarities between users
struct Similarity {
    int similarity;
    int id;
    struct Similarity* next;
};

//similarity value start from 1
struct Similarity* createSimilarityNode(int id) {
    struct Similarity* new_node = malloc(sizeof(struct Similarity));
    new_node->id = id;
    new_node->similarity = 1;
    new_node->next = NULL;
    return new_node;
}

//returns root
struct Similarity* addSimNode(struct Similarity* root, int id) {
    struct Similarity* new_node = createSimilarityNode(id);
    if(root == NULL) {
        return new_node;
    }
    //if list is empty, new_node is the root.

    struct Similarity* temp = root;
    while(temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = new_node;
    return root;
    //else add root to and of the linked list.
}

//returns root
struct Similarity* searchSimNode(struct Similarity* root, int num) {
    if(root == NULL) {
        return NULL;
    }
    struct Similarity* temp = root;

    while(temp != NULL) {

        if(temp->id == num) {
            return temp; 
        }
        temp = temp->next;
    }
    return NULL;
}

//returns root
struct Similarity* deleteSimNode(struct Similarity* root, int id) {
    if(root == NULL) return NULL;

    struct Similarity* temp = root;
    struct Similarity* pre = NULL;
    

    int a = temp->id;

    if(a == id) {
        pre = temp->next;
        free(temp);
        return pre;
    }

    struct Similarity* temp2 = root;

    while(temp2 != NULL) {
        if(temp2->id == id) {
            pre->next = temp2->next;
            free(temp2);
            break;
        }
        pre = temp2;
        temp2 = temp2->next;

    }
    return root;
}

//Creates a linked-list that contains all users who have at least 1 movie common with user given. Returns root of that linked list.
struct Similarity* similarUser(int user, struct node* users[USER_NUM+1], struct node* movies[MOVIE_NUM+1]) {
    if(user > USER_NUM || user < 1) {
        printf("There is no such user.\n");
        return NULL;
    }

    if(users[user]->next == 0) {
        printf("The user didn't watch any movie!\n");
        return NULL;
    }
    
    struct node* temp = users[user]->next;

    struct Similarity* root = NULL;

    while(temp != NULL) {

        if(temp->id > MOVIE_NUM) {
            printf("ATTENTION\n");
            temp = temp->next;
            continue;
        }

        struct node* temp2 = movies[temp->id];
        if(temp2->next == NULL) {
            printf("ATTENTION2\n");
        }

        temp2 = temp2->next;

        while(temp2 != NULL) {

            struct Similarity* result = searchSimNode(root, temp2->id);

            if(result == NULL) {
                root = addSimNode(root, temp2->id);
            }
            else {
                result->similarity += 1;
            }
            temp2 = temp2->next;
        }
        temp = temp->next;
    }
    return root;
}

// Returns the most similar user to the user
struct Similarity* mostSimilar(int user, struct node* users[USER_NUM+1], struct node* movies[MOVIE_NUM+1]) {
    struct Similarity* root = similarUser(user, users, movies);
    struct Similarity* greatest;
    for(int i = 0; i<2; i++) {

        struct Similarity* temp = root;
        if(root == NULL) {
            printf("Warning\n");
            return NULL;
        }

        greatest = temp;

        while(temp != NULL) {
            if((temp->similarity > greatest->similarity) || temp->id == user) greatest = temp;
            temp = temp->next;
        }
        if(i == 0) {
            root = deleteSimNode(root, greatest->id); //  user is being deleted
        }
    }
    //we will free greatest so before that we copy him
    struct Similarity* new_node = createSimilarityNode(greatest->id);
    new_node->similarity = greatest->similarity;

    //free
    struct Similarity* temp = root;
    while(temp != NULL) {
        struct Similarity* temp2 = temp;
        temp = temp->next;
        free(temp2);
    }
    return new_node;
}

//Fills the recs array with best rated films of the most similar user. Does not recommend movies watched by the user.
//recs must contain more elements than rec_num.
void similarUserRecom(int user, struct node* users[USER_NUM+1], struct node* movies[MOVIE_NUM+1], int recs[REC_ARR_SIZE], int rec_num) {

    struct Similarity* most_similar = mostSimilar(user, users, movies);
    if(most_similar == NULL) {
        for(int i = 0; i<rec_num; i++) {
            recs[i] = 0;
        }
        return;
    }

    if(most_similar->id > USER_NUM || most_similar->id < 1) {
        for(int i = 0; i<rec_num; i++) {
            recs[i] = 0;
        }
        printf("Err!\n");
        return;
    }

    struct node* root_user = watchedFilms2(user, users);
    struct node* root_similar = watchedFilms2(most_similar->id, users);
    struct node* temp1 = root_similar;
    
    int i = 0;

    while(temp1 != NULL) {
        if(i>rec_num-1) break;
        struct node* temp2 = root_user;
        int different = 1;

        while(temp2 != NULL) {
            if(temp1->id == temp2->id) {
                different = 0;
                break;
            }
            temp2 = temp2->next;
        }

        if(different == 1) {
            recs[i] = temp1->id;
            i++;
        }
        temp1 = temp1->next;
    }

    //memory cleaning
    struct node* temp = root_user;
    while(temp != NULL) {
        struct node* tempf = temp;
        temp = temp->next;
        free(tempf);
    }
    temp = root_similar;
    while(temp != NULL) {
        struct node* tempf = temp;
        temp = temp->next;
        free(tempf);
    }
}


//---------QUEUE STRUCTURE-------------

//a node struct for a doubly linked queue
struct q_node {
    int type; //0 for user, 1 for film, 2 means it is not important
    int id;
    int dist;
    struct q_node* next;
    struct q_node* prev;
};

//min_prior is for deleting it when necessary. So we have a priority queue
struct queue {
    struct q_node* head;
    struct q_node* min_prior;
};

struct q_node* createQNode(int id, int type, int dist) {
    struct q_node* node = malloc(sizeof(struct q_node));
    node->id = id;
    node->next = NULL;
    node->prev = NULL;
    node->type = type;
    node->dist = dist;
    return node;
}

struct queue* addQnodeNode(struct queue* queue, int id, int type, int dist) {
    struct q_node* new_node = createQNode(id, type, dist);
    if(queue->head == NULL) {
        queue->head = new_node;
        queue->min_prior = new_node;
        return queue;
    }

    struct q_node* temp = queue->head;
    while(temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = new_node;
    new_node->prev = temp;
    if(new_node->dist < queue->min_prior->dist) {
        queue->min_prior = new_node;
    }
    return queue;
}

void assignMinPrior(struct queue* queue) {
    if(queue == NULL) return;

    struct q_node* temp = queue->head;
    int min = INT_MAX;
    queue->min_prior = NULL;

    while(temp != NULL) {
        if(temp->dist < min) {
            min = temp->dist;
            queue->min_prior = temp;
        }
        temp = temp->next;
    }
}

//Deletes the node indicated by min_prior, fills id_type array with id and type of the node deleted. id_type[0]=id, id_type[1] = type
void deleteQnode(struct queue* queue, int id_type[2]) {
    if(queue == NULL) return;
    if(queue->min_prior == NULL) return;

    if(queue->min_prior->prev == NULL) {
        if(queue->min_prior->next == NULL) {
            queue->head = NULL;
        }
        else {
            queue->min_prior->next->prev = NULL;
            queue->head = queue->min_prior->next;
        }
        
    }

    else if(queue->min_prior->next == NULL) {
        queue->min_prior->prev->next = NULL;
    }

    

    else {
        queue->min_prior->prev->next = queue->min_prior->next;
        queue->min_prior->next->prev = queue->min_prior->prev;
        
    }
    id_type[0] = queue->min_prior->id;
    id_type[1] = queue->min_prior->type;
    free(queue->min_prior);

    assignMinPrior(queue);
}

struct q_node* findQnode(struct queue* queue, int id, int type) {
    if(queue == NULL) return NULL;
    struct q_node* temp = queue->head;
    while(temp != NULL) {
        if(temp->id == id && temp->type == type) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

//Finds the node with id and type and makes his new distance equal to dist
void changeDist(struct queue* queue, int id, int type, int dist) {
    if(queue == NULL) return;
    struct q_node* q_node = findQnode(queue, id, type);
    if(q_node != NULL) {
        q_node->dist = dist;
    }   
    assignMinPrior(queue);
}
//----------END OF QUEUE STRUCTURE---------------


//Fills dist_u list with distance of all users to the source, and dist_m with films' distances. (we have index id equality)
void dijksta(int source, struct node* users[USER_NUM+1], struct node* movies[MOVIE_NUM+1], int dist_u[USER_NUM+1], int dist_m[MOVIE_NUM+1]) {

    //create queue
    struct queue* queue = malloc(sizeof(struct queue));
    queue->head = NULL;
    queue->min_prior = NULL;

    //initialises dist_u and adds a node to the queue for each user 
    for(int i = 1; i <USER_NUM+1; i++) {
        dist_u[i] = INT_MAX;
        queue = addQnodeNode(queue, users[i]->id, users[i]->type, INT_MAX);
    }

    //initialises dist_m and adds a node to the queue for each film
    for(int i = 1; i <MOVIE_NUM+1; i++) {
        dist_m[i] = INT_MAX;
        queue = addQnodeNode(queue, movies[i]->id, movies[i]->type, INT_MAX);
    }
    //makes source's distance equal to 0 so we will start by it (shortest distance)
    dist_u[source] = 0;
    changeDist(queue, source, 0, 0);

    while(queue->head != NULL) {

        int id_type[2];
        deleteQnode(queue, id_type);
        //id_type[0] is id of the node deleted
        //id_type[1] is type of the node deleted. rappel: 0->user, 1->film

        if(id_type[1] == 0) {
            struct node* temp = users[id_type[0]]->next;
            int alt = 0;

            //i decided to have (6-point) for weight because when we chose point as weight directly, what i will recommend to users
            //is going to be primarily the films rated 1 by users who also rated 1 a film that the main user rated 1.
            while(temp != NULL) {
                alt = dist_u[id_type[0]] + 6-(temp->point);
                if(alt < dist_m[temp->id]) {
                    dist_m[temp->id] = alt;
                    changeDist(queue, temp->id, 1, alt); 
                }
                temp = temp->next;
            }
        } 

        if(id_type[1] == 1) {
            struct node* temp = movies[id_type[0]]->next;
            int alt = 0;
            while(temp != NULL) {
                alt = dist_m[id_type[0]]+ 6-(temp->point);
                if(alt < dist_u[temp->id]) {
                    dist_u[temp->id] = alt;
                    changeDist(queue, temp->id, 0, alt); 
                }
                temp = temp->next;
            }
        }
    }
    
}

//Makes recommendations based on shortest path between nodes.
//recs must contain more elements than rec_num.
void ShortestWeighPathRec(int source, struct node* users[USER_NUM+1], struct node* movies[MOVIE_NUM+1], int recs[REC_ARR_SIZE], int rec_num) {

    int dist_u[USER_NUM+1];
    int dist_m[MOVIE_NUM+1];

    if(source > USER_NUM || source < 1) {
        printf("Source does not exist!\n");
        return;
    }

    //ids are initialized
    int ids[MOVIE_NUM+1];
    for(int i = 0; i<MOVIE_NUM+1; i++) {
        ids[i] = i;
    }

    //distances are finded
    dijksta(source, users, movies, dist_u, dist_m);

    //distances are sorted
    quickSort(dist_m, ids, 1, MOVIE_NUM);

    struct node* head = watchedFilms2(source, users);

    //films didn't watch by user are added to the recs
    int index = 0;
    for(int i = 1; i < MOVIE_NUM+1; i++) {  
        if(index > rec_num-1) break;
        if(dist_m[i] > 2) { //1 means user already watched the film and gave 5, 2 means s.he gave a 4
            if(searchNode(head, ids[i])== NULL) {
                recs[index] = ids[i];
                index++;
            }
        }
    }

    //free the nodes
    struct node* temp = head;
    while(temp != NULL) {
        struct node* temp2 = temp;
        temp = temp->next;
        free(temp2);
    }

    return;
}


//--------------------------------


//Takes an array of integer of lenght 5 and fill it with most popular films watched by the user.
void popularWatchedFilms(int user, struct node* users[USER_NUM+1], struct node* movies[MOVIE_NUM+1], int films[5]) {
    if(user > USER_NUM || user < 1) return;
    int watched[MOVIE_NUM];
    int elm_num = watchedFilms(user, users, watched);
    int views[USER_NUM];
    
    for(int i = 0; i<elm_num; i++) {
        views[i] = watchedBy(watched[i], movies);
    }
    quickSort(views, watched, 0, elm_num-1);

    for(int i = 0; i<5; i++) {
        films[i] = watched[elm_num-1-i];
    }
}


//-------A structure to hold users who have watched certain films that we decided -----------
struct SameFilms {
    int id;
    float average;
    struct SameFilms* next;
};

struct SameFilms* createSameFilmsNode(int id, float average) {
    struct SameFilms* node = malloc(sizeof(struct SameFilms));
    node->id = id;
    node->next = NULL;
    node->average = average;
    return node;
}

struct SameFilms* addSameFilmsNode(struct SameFilms* root, int id, float average) {
    struct SameFilms* new_node = createSameFilmsNode(id, average);
    if(root == NULL) {
        return new_node;
    }

    struct SameFilms* temp = root;
    while(temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = new_node;
    return root;
}

struct SameFilms* deleteSameFilmsNode(struct SameFilms* root, int id) {

    if(root == NULL) return NULL;

    struct SameFilms* temp = root;
    struct SameFilms* pre = NULL;
    

    int a = temp->id;

    if(a == id) {
        pre = temp->next;
        free(temp);
        return pre;
    }

    struct SameFilms* temp2 = root;

    while(temp2 != NULL) {
        if(temp2->id == id) {
            pre->next = temp2->next;
            free(temp2);
            break;
        }
        pre = temp2;
        temp2 = temp2->next;

    }
    return root;
}

//----------------------------


//Finds all users who watched 5 films decided by popularWatchedFilms function that it calls.
//If there aren't any user watched all that films other than the user given then the function searches for users who watched 4 films.
//It goes the same way until 1.
struct SameFilms* UsersWatchedThoseFilms(int user, struct node* users[USER_NUM+1], struct node* movies[MOVIE_NUM+1]) {
    if(user > USER_NUM || user < 1) {
        printf("User is not in the right interval for UsersWatchedThoseFilms function!\n");
        return NULL;
    }

    int films[5];
    popularWatchedFilms(user, users, movies, films);
    struct SameFilms* root = NULL;
    int limit = 5;

    do {
        
        if(limit < 0) break;

        if(limit != 5) {
            free(root);
            root = NULL;
        }

        for(int i = 1; i<USER_NUM+1; i++) {
            float total = 0;
            for(int j = 0; j<limit; j++) {
                struct node* temp = searchNode(users[i], films[j]);
                if(temp == NULL) {
                    goto end;
                }
                total += temp->point;
            }
            float average = total/limit;
            root = addSameFilmsNode(root, i, average);
        end: ;
        }
        
        limit--;

        if(root == NULL) {
            printf("These films are watched by no one. Be sure that id is between 1 and %d and \n", USER_NUM);
            return root;
        }
    }
    while(root->next == NULL);

    if(limit < 3) {
        printf("Attention, this may not be an approprite recommendation for this user.\n");
    }
    return root;
}

//Finds the closest user to user given with comparing notes given at 5 films chosen
struct SameFilms* closestAverage(int user, struct node* users[USER_NUM+1], struct node* movies[MOVIE_NUM+1]) {

    struct SameFilms* root = UsersWatchedThoseFilms(user, users, movies);
    if(root == NULL) {
        printf("Can't find closest user (closestAverage function)!\n");
        return NULL;
    }

    struct SameFilms* temp = root;
    float users_avg = 0;

    //user's average is finded
    while(temp != NULL) {
        if(temp->id == user) {
            users_avg = temp->average;
            break;
        }
        temp = temp->next;
    }

    if(temp == NULL) {
        printf("\nError! user is not in the list!\n");
        return NULL;
    }

    //user is being deleted from list
    root = deleteSameFilmsNode(root, user);

    temp = root;
    struct SameFilms* closest = root;
    while(temp != NULL) {

        if(fabs(closest->average - users_avg) > fabs(temp->average - users_avg)) {
            closest = temp;
        }
        temp = temp->next;
    }

    struct SameFilms* cls = createSameFilmsNode(closest->id, closest->average);

    //free
    temp = root;
    while(temp != NULL) {
        struct SameFilms* temp2 = temp;
        temp = temp->next;
        free(temp2);
    }

    return cls;
}

//Fills the empty list that it takes with best films of the most similar user finded by point average
//recs must contain more elements than rec_num.
struct SameFilms* closeAverageRec(int user, struct node* users[USER_NUM+1], struct node* movies[MOVIE_NUM+1], int recs[REC_ARR_SIZE], int rec_num) {
    
    //most similar user is being found
    struct SameFilms* similar = closestAverage(user, users, movies);

    if(similar == NULL) {
        for(int i = 0; i<rec_num; i++) {
            recs[i] = 0;
        }
        return NULL;
    } 

    struct node* root_user = watchedFilms2(user, users);
    struct node* root_similar = watchedFilms2(similar->id, users);
    struct node* temp1 = root_similar;
    
    int i = 0;

    while(temp1 != NULL) {

        if(i>rec_num-1) break;

        struct node* temp2 = root_user;
        int different = 1;

        while(temp2 != NULL) {

            if(temp1->id == temp2->id) {
                different = 0;
                break;
            }
            temp2 = temp2->next;
        }

        if(different == 1) {
            recs[i] = temp1->id;
            i++;
        }
        temp1 = temp1->next;
    }

    //free
    struct node* temp = root_user;
    while(temp != NULL) {
        struct node* tempf = temp;
        temp = temp->next;
        free(tempf);
    }
    //free
    temp = root_similar;
    while(temp != NULL) {
        struct node* tempf = temp;
        temp = temp->next;
        free(tempf);
    }

    return similar;
}

//-----------------------

//Calculates average point of the film given
float pointCalcul(int movie, struct node* movies[MOVIE_NUM+1]) {
    if(movie > MOVIE_NUM || movie < 1) {
        printf("Movie is not in the right interval!\n");
        return -1.0;
    }
    struct node* temp = movies[movie];
    if(temp == NULL) return -1.0;

    float total = 0;
    float users = 0;

    while(temp->next != NULL) {
        temp = temp->next;
        total += temp->point;
        users++;
    }
    return total/users; 
}

//A basic structure for holding the point and the id of a movie toghether
struct Movie {
    float point;
    int id;
};

//Makes swap operation for insertion sort
void swap(struct Movie* a, struct Movie* b) {
    struct Movie temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

//Sorts the films in descending order according to their points
void insertionSort(struct Movie* liste, int eleman_sayisi) {
    for(int i = 1; i<eleman_sayisi; i++) {
        for(int j = i; j>0; j--) {
            int swapped = 0;
            
            if((liste[j]).point > (liste[j-1]).point) {
                swap(&liste[j], &liste[j-1]);
                swapped = 1;
            }
            if (swapped == 0) break;
        }
    }
}

//Takes an empty array of struct Movie and fill it with ordered films (according their point in descending order)
void movieList(struct node* movies[MOVIE_NUM+1], struct Movie ordered[MOVIE_NUM]) {

    for(int i = 1; i<= MOVIE_NUM; i++) {
        (ordered[i-1]).id = i;
        (ordered[i-1]).point = pointCalcul(i, movies);
    }
    insertionSort(ordered, MOVIE_NUM);
}

//Returns best 10 movies that the user didn't watch. Eliminates automatically films watched by few than 10 users.
//recs must contain more elements than rec_num.
void bestRatingRec(int user, struct node* users[USER_NUM+1], struct node* movies[MOVIE_NUM+1], struct Movie recs[REC_ARR_SIZE], int rec_num) {
    struct Movie ordered[MOVIE_NUM];
    struct node* root = watchedFilms2(user, users);
    movieList(movies, ordered);
    int j = 0;
    
    for(int i = 0; i<MOVIE_NUM; i++) {
        if(j>rec_num-1) break;

        struct node* temp = root;
        while(temp != NULL) {
            if(ordered[i].id == temp->id || watchedBy(ordered[i].id, movies) < 10) {
                goto end;
            }
            temp = temp->next;
        }
        recs[j] = ordered[i];
        j++;
    end: ;
    }

    //free
    struct node* temp = root;
    while(temp!= NULL) {
        struct node* temp2 = temp;
        temp = temp->next;
        free(temp2);
    }
    return;
}

int main() {
    struct node* users[USER_NUM+1]; //all ids are same with the indexes
    struct node* movies[MOVIE_NUM+1]; //all ids are same with indexes

    //for using the id index equality it is decided to have 2 arrays.

    //adding users to the array
    for(int i = 1; i <= USER_NUM; i++) {
        struct node* new_node = createNode(i, 0, 0);
        users[i] = new_node;
    }
    //adding movies to the array
    for(int i = 1 ; i <= MOVIE_NUM; i++) {
        struct node* new_node = createNode(i, 1, 0);
        movies[i] = new_node;
    }

    FILE* file;
    int user;
    int film;
    int point;

    file = fopen("example.txt", "r");
    int i = 0;
    struct node* temp = NULL;
    
    while(i < MAX_LINE) {
        
        if (fscanf(file, "%d %d %d %*d", &user, &film, &point) != 3) {
            printf("File couldn't transfered.\n");
            fclose(file);
            return 1; 
        }

        //We add the film connections to the array of users
        temp = users[user];
        if(temp == NULL) {
            printf("User is not in the array!\n");
            i++;
            continue;
        }
        
        while(temp->next != NULL) {
                temp = temp->next;
            }
        temp->next = createNode(film, 2, point);

        //We add user connections to the array of films
        temp = movies[film];
        if(temp == NULL) {
            printf("Film is not in the array!");
            i++;
            continue;
        }
        
        while(temp->next != NULL) {
                temp = temp->next;
            }
        temp->next = createNode(user, 2, point);

        i++;
    }
    fclose(file);



    printf("Enter the user (1-%d): ", USER_NUM);
    int the_user = 1;
    scanf(" %d", &the_user);
    printf("How many films should i recommend per algorithm?\nEnter a number between 1 and %d -> ", REC_ARR_SIZE);
    int rec_num = 1;
    scanf(" %d", &rec_num);

    if(the_user < 1) the_user = 1;
    if(the_user > USER_NUM) the_user = USER_NUM;
    if(rec_num < 1) rec_num = 1;
    if(rec_num > REC_ARR_SIZE) rec_num = REC_ARR_SIZE;

    clock_t start, end;
    double cpu_time_used;
    

    int arr[REC_ARR_SIZE];
    printf("\nUser %d\n--------\n\n", the_user);

    printf("Random recommendation:\n");

    start = clock();

    randomRec(the_user, users, arr, rec_num);

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    for(int j = 0; j<rec_num; j++) {
        printf("%d ", arr[j]);
    }
    printf("\n");
    printf("Time consumed: %.3f seconds\n", cpu_time_used);
    

//--

    printf("\nPopular film recommendation:\n");
    start = clock();

    mostWatchedRec(the_user, movies, users, arr, rec_num);

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    for(int j = 0; j<rec_num; j++) {
        printf("Film id: %d, Watched by %d users\n", arr[j], watchedBy(arr[j], movies));
    }
    printf("Time consumed: %.3f seconds\n", cpu_time_used);

//--

    printf("\nMost common film recommendation:\n");

    start = clock();

    similarUserRecom(the_user, users, movies, arr, rec_num);

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Most similar user is %d with %d films in common\n", mostSimilar(the_user, users, movies)->id, mostSimilar(the_user, users, movies)->similarity);

    for(int j = 0; j<rec_num; j++) {
        printf("%d ", arr[j]);
    }
    printf("\n");
    printf("Time consumed: %.3f seconds\n", cpu_time_used);

//--

    printf("\nShortest weighted path recommendation:\n");

    start = clock();

    ShortestWeighPathRec(the_user, users, movies, arr, rec_num);

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    for(int j = 0; j<rec_num; j++) {
        printf("%d ", arr[j]);
    }
    printf("\n");
    printf("Time consumed: %.3f seconds\n", cpu_time_used);

//--

    printf("\nSimilar ratings recommendation:\n");
    start = clock();

    closeAverageRec(the_user, users, movies, arr, rec_num);

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Most similar user is %d\n", closestAverage(the_user, users, movies)->id);
    for(int j = 0; j<rec_num; j++) {
        printf("%d ", arr[j]);
    }
    printf("\n");
    printf("Time consumed: %.3f seconds\n", cpu_time_used);

//--

    printf("\nBest rating recommendation:\n");
    struct Movie recoms[REC_ARR_SIZE];
    start = clock();

    bestRatingRec(the_user, users, movies, recoms, rec_num);

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    for(int j = 0; j<rec_num; j++) {
        printf("Film id:%d, rating of the film: %.2f\n", recoms[j].id, recoms[j].point);
    }
    printf("Time consumed: %.3f seconds\n", cpu_time_used);

    return 0;
}