#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>

#define SKIPLIST_MAX_LEVEL 32

char* negInf = "-oo";
char* posInf = "+oo";

typedef struct node {
  char* key;
  int value;
  int pos;
  struct node *right;
  struct node *left;
  struct node *up;
  struct node *down;
} node;

typedef struct skiplist{
  node *head;
  node *tail;
  int numNodes;
  int height;
  int random;
} skiplist;

skiplist *skiplist_init(skiplist *list) {
  node *p1, *p2;
  p1 = (node *) malloc(sizeof(struct node));
  p2 = (node *) malloc(sizeof(struct node));
  p1->key   = negInf;
  p2->key   = posInf;
  list->head = p1;
  list->tail = p2;

  list->numNodes = 0;
  list->height   = 0;
  list->random   = 0;
}

double r2()
{
  time_t t;
  return (double)rand() / (double)((unsigned)RAND_MAX + 1) ;
}

int size(skiplist *list)
{
  return list->numNodes;
}

bool isEmpty(skiplist *list)
{
  return (list->numNodes == 0);
}

node* findEntry(char *key, skiplist *list)
{
  node* p;

  p = list->head;

  //printf("%d\n",strcmp(p->right->key, key));
  while(true)
  {
    while( (strcmp(p->right->key, posInf) != 0) &&
    (strcmp(p->right->key, key)   <= 0) )
    {
      p = p->right;
    }
    if(p->down != NULL)
    {
      p = p->down;
    }
    else
    {
      break;
    }
  }
  return p;
}

int put(int memory, int value, skiplist *list)
{
  node *p, *q;
  int   i;

  p = findEntry(key, list);
  if( strcmp(key, p->key) == 0 )
  {
    //printf("jaskdjsj");
    int old = p->value;
    p->value = value;
    return old;
  }

  q = (node *) malloc(sizeof(struct node));
  q->key         = key;
  q->value       = value;
  q->left        = p;
  q->right       = p->right;
  q->right->left = q;
  p->right       = q;
  i = 0;
  //printf("%lf", r2());
  double random = r2();
  //printf("%lf\n", random);
  while(rand() < RAND_MAX/2 && list->height < SKIPLIST_MAX_LEVEL)
  {
    //printf("SSSSSs%lf\n", random);
    //printf("i = %d, h = %d\n",i, list->height );
    if (i >= list->height)
    {
      node *p1, *p2;
      list->height++;
      p1 = (node *) malloc(sizeof(struct node));
      p2 = (node *) malloc(sizeof(struct node));
      p1->key = negInf;
      p2->key = posInf;

      p1->right = p2;
      p1->down  = list->head;

      p2->left  = p1;
      p2->down  = list->tail;
      list->head->up  = p1;
      list->tail->up  = p2;

      list->head      = p1;
      list->tail      = p2;
    }

    while(p->up == NULL)
    {
      p = p->left;
    }
    p = p->up;

    node *e;

    e = (node *) malloc(sizeof(struct node));
    e->key   = key;
    e->left  = p;
    e->right = p->right;
    e->down  = q;

    p->right->left = e;
    p->right       = e;
    q->up          = e;

    q = e;
    i++;
    random = r2();
  }

  list->numNodes++;
  return 0;
}

char* getOneRow(node *p)
{
  char* str1;
  int a, b;

  a = 0;

  str1 = "";
  char * new_str;
  char * old_str;
  if((new_str = malloc(strlen(str1)+strlen(p->key)+1)) != NULL){
    new_str[0] = '\0';   // ensures the memory is an empty string
    strcat(new_str,str1);
    strcat(new_str,p->key);
  } else {
    printf("malloc failed!\n");
  }
  //strcat(s, p->key);
  p = p->right;

  while(p != NULL)
  {
    node *q;
    q = p;
    while(q->down != NULL)
    {
      q = q->down;
    }
    b = q->pos;
    new_str = realloc(new_str, (strlen(new_str)+strlen(" <-")));
    strcat(new_str, " <-");

    for(int i = a+1; i < b; i++)
    {
      new_str = realloc(new_str, (strlen(new_str)+strlen("--------")));
      strcat(new_str, "--------");
      //strcat("--------", s);
    }
    new_str = realloc(new_str, (strlen(new_str)+strlen("> ")));
    strcat(new_str, "> ");
    //  strcat("> ",  s);
    new_str = realloc(new_str, (strlen(new_str)+strlen(p->key)));
    strcat(new_str, p->key);
    //  strcat(p->key,s);
    a = b;
    p = p->right;
  }
  return new_str;
}



void printHorizontal(skiplist *list)
{
  char *s = "";
  int   i;
  node *p;

  p = list->head;
  while(p->down != NULL)
  {
    p = p->down;
  }
  i = 0;
  while(p != NULL)
  {
    p->pos = i++;
    p      = p->right;
  }

  p = list->head;

  while (p != NULL)
  {
    s = getOneRow(p);
    printf("%s\n",s);
    p = p->down;
  }

}

int main()
{
  srand(getpid());
  skiplist* s = (skiplist *) malloc(sizeof(struct skiplist));

  node *p1, *p2;
  p1 = (node *) malloc(sizeof(struct node));
  p2 = (node *) malloc(sizeof(struct node));
  p1->key   = negInf;
  p2->key   = posInf;
  s->head = p1;
  s->tail = p2;
  p1->right = p2;
  p2->left  = p1;

  s->numNodes = 0;
  s->height   = 0;
  s->random   = 0;


  //printHorizontal(s);
  printf("\n");
  //printf("------\n");
  //    printVertical();
  //    printf("======");

  put(1024, 123, s);
  printf("\n");
  printf("\n");

  put(2048, 123,s);
  put(4096, 123,s);
  put(8192, 123,s);
  put(16384, 123,s);
  put(32768, 123,s);

  printHorizontal(s);
  printf("%s\n", findEntry("HIK", s)->key);


  return 0;
}
