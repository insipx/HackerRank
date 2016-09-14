//PROGRAM: mtheall's program to solve Botclean Large on Hackerrank
//Is a GA algorithm.
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <pthread.h>

#define NUM_THREADS      4

#define MIGRATIONS       5
#define GENERATIONS      100
#define REPEAT_MAX       (GENERATIONS/5)
#define POPULATION_SIZE  1000
#define ELITE_SIZE       (POPULATION_SIZE/100)
#define MIGRATION_SIZE   (ELITE_SIZE/NUM_THREADS)
#define CROSSOVER_CHANCE (80) /* 0-100 */
#define MUTATION_CHANCE  (75) /* 0-100 */
#define MUTATION_MAX     25
#define MAX_NODES        25

typedef struct {
  int x, y;
  int dist;
} node_t;

typedef struct {
  int size;
  int target;
  node_t nodes[0];
} node_list_t;

typedef struct {
  node_t **nodes;
  int    value;
  int    elite_score;
} rank_t;

typedef enum {
  WAIT,
  CONTINUE,
  RESULT,
  QUIT,
} op_t;

typedef struct {
  node_t *nodes;
  rank_t *results;
  pthread_mutex_t mutex;
  pthread_cond_t  cond;
  op_t op;
  int num_nodes;
  int posx, posy;
} work_t;

static inline int dist(int dx, int dy) {
  return abs(dx) + abs(dy);
}

static void* worker(void* arg);
static int rank(int posx, int posy, node_t **nodes, int num_nodes);
static inline int node_dist(node_t *n1, node_t *n2);
static int rank_compare(const void *arg1, const void *arg2);
static int node_compare(const void *arg1, const void *arg2);

static inline void makeMove(int posx, int posy, int endx, int endy) {
  if     (posx < endx) printf("DOWN\n");
  else if(posy < endy) printf("RIGHT\n");
  else if(posx > endx) printf("UP\n");
  else                 printf("LEFT\n");
}

static void next_move(int posx, int posy, node_t *nodes, int num_nodes) {
  int endx, endy, fd, orig_num_nodes = num_nodes;
  pthread_t tid[NUM_THREADS];
  work_t work_package[NUM_THREADS];
  work_t *best;

  for(int i = 0; i < num_nodes; ++i)
    nodes[i].dist = dist(posx-nodes[i].x, posy-nodes[i].y);

  /* if there's more than MAX_NODES nodes, then just process that many */
  if(num_nodes > MAX_NODES) {
    qsort(nodes, num_nodes, sizeof(node_t), node_compare);
    num_nodes = MAX_NODES;
  }

  /* set up the worker threads */
  for(int t = 0; t < NUM_THREADS; ++t) {
    work_package[t].nodes     = nodes;
    work_package[t].num_nodes = num_nodes;
    work_package[t].posx = posx;
    work_package[t].posy = posy;
    work_package[t].op = WAIT;
    pthread_mutex_init(&work_package[t].mutex, NULL);
    pthread_cond_init(&work_package[t].cond, NULL);
    pthread_mutex_lock(&work_package[t].mutex);
    pthread_create(&tid[t], NULL, worker, &work_package[t]);
  }

  /* perform generations around migrations */
  for(int m = 0; m < MIGRATIONS; ++m) {
    for(int t = 0; t < NUM_THREADS; ++t) {
      work_package[t].op = CONTINUE;
      pthread_mutex_unlock(&work_package[t].mutex);
    }

    for(int t = 0; t < NUM_THREADS; ++t) {
      pthread_mutex_lock(&work_package[t].mutex);
      while(work_package[t].op != RESULT) {
        pthread_cond_signal(&work_package[t].cond);
        pthread_cond_wait(&work_package[t].cond, &work_package[t].mutex);
      }
    }

    /* migrate the best candidates among the pools */
    if(NUM_THREADS > 1) {
      for(int i = 0; i < NUM_THREADS; ++i) {
        for(int j = 0; j < NUM_THREADS; ++j) {
          if(i != j) {
            for(int k = 0; k < MIGRATION_SIZE; ++k) {
              /* overwrite the worst from m2 with the best from m1 */
              node_t **m1 = work_package[i].results[k].nodes;
              node_t **m2 = work_package[j].results[POPULATION_SIZE-k-1].nodes;
              memcpy(m2, m1, sizeof(node_t*) * num_nodes);
            }
            /* resort the new ranking */
            qsort(work_package[j].results, POPULATION_SIZE, sizeof(rank_t), rank_compare);
          }
        }
      }
    }
  }

  /* signal for the final results */
  for(int t = 0; t < NUM_THREADS; ++t) {
    while(work_package[t].op != RESULT) {
      pthread_cond_signal(&work_package[t].cond);
      pthread_cond_wait(&work_package[t].cond, &work_package[t].mutex);
    }
  }

  /* find the best candidate from all pools */
  best = &work_package[0];
  for(int t = 1; t < NUM_THREADS; ++t) {
    if(work_package[t].results[0].value < best->results[0].value)
      best = &work_package[t];
  }

  /* output node listing to file */
  fd = open("state", O_WRONLY | O_TRUNC | O_CREAT, 0600);
  if(fd >= 0) {
    if(orig_num_nodes == num_nodes) {
      write(fd, &num_nodes, sizeof(num_nodes));
      write(fd, "\0\0\0\0\0\0\0\0", sizeof(int));
      for(int i = 0; i < num_nodes; ++i)
        write(fd, best->results[0].nodes[i], sizeof(node_t));
    }
    else {
      num_nodes = 1;
      write(fd, &num_nodes, sizeof(int));
      write(fd, "\0\0\0\0\0\0\0\0", sizeof(int));
      write(fd, best->results[0].nodes[0], sizeof(node_t));
    }
    close(fd);
  }

  endx = best->results[0].nodes[0]->x;
  endy = best->results[0].nodes[0]->y;

  for(int t = 0; t < NUM_THREADS; ++t) {
    work_package[t].op = QUIT;
    pthread_cond_signal(&work_package[t].cond);
    pthread_mutex_unlock(&work_package[t].mutex);
    pthread_join(tid[t], NULL);
  }

  makeMove(posx, posy, endx, endy);
}

int main(void) {
  int posx, posy, dimx, dimy, endx, endy;
  int num_dirty = 0, fd;
  static char board[50][50];
  node_t *nodes, *node;

  setlinebuf(stdout);
  setlinebuf(stderr);

  scanf("%d", &posx);
  scanf("%d", &posy);
  scanf("%d", &dimx);
  scanf("%d", &dimy);
  for(int i = 0; i < dimx; ++i) {
    scanf("%s[^\\n]%*c", board[i]);
  }

  if(board[posx][posy] == 'd') {
    printf("CLEAN\n");
    return 0;
  }

  fd = open("state", O_RDWR);
  if(fd >= 0) {
    node_list_t *n;
    struct stat st;

    fstat(fd, &st);
    n = (node_list_t*)mmap(NULL, st.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

    if(n->size == 1) {
      if(posx == n->nodes[n->target].x && posy == n->nodes[n->target].y)
        unlink("state");
      else {
        makeMove(posx, posy, n->nodes[n->target].x, n->nodes[n->target].y);
        munmap(n, st.st_size);
        return 0;
      }
      munmap(n, st.st_size);
    }
    else {
      if(posx == n->nodes[n->target].x && posy == n->nodes[n->target].y) {
        for(int i = 0; i < n->size-1; ++i) {
          if(n->nodes[i].x == posx && n->nodes[i].y == posy)
            n->target = i+1;
        }
      }
      makeMove(posx, posy, n->nodes[n->target].x, n->nodes[n->target].y);
      munmap(n, st.st_size);
      return 0;
    }
  }

  unlink("state");
  for(int i = 0; i < dimx; ++i) {
    for(int j = 0; j < dimy; ++j) {
      if(board[i][j] == 'd') {
        num_dirty++;
        endx = i;
        endy = j;
      }
    }
  }


  if(num_dirty == 1) {
    makeMove(posx, posy, endx, endy);
    return 0;
  }

  nodes = (node_t*)malloc(num_dirty * sizeof(node_t));
  if(nodes == NULL)
    return 1;

  node = nodes;
  for(int i = 0; i < dimx; ++i) {
    for(int j = 0; j < dimy; ++j) {
      if(board[i][j] == 'd') {
        node->x = i;
        node->y = j;
        ++node;
      }
    }
  }

  next_move(posx, posy, nodes, num_dirty);

  free(nodes);
  return 0;
}

static int rank(int posx, int posy, node_t **nodes, int num_nodes) {
  int distance = nodes[0]->dist + 1;
  for(int i = 0; i < num_nodes-1; ++i)
    distance += node_dist(nodes[i], nodes[i+1]) + 1;
  return distance;
}

static int rank_compare(const void *arg1, const void *arg2) {
  rank_t *r1 = (rank_t*)arg1;
  rank_t *r2 = (rank_t*)arg2;

  return r1->value - r2->value;
}

static int node_compare(const void *arg1, const void *arg2) {
  node_t *n1 = (node_t*)arg1;
  node_t *n2 = (node_t*)arg2;

  return n1->dist - n2->dist;
}

static inline void node_swap(node_t **n1, node_t **n2) {
  node_t *tmp;
  if(*n1 == NULL || *n2 == NULL)
    abort();
  tmp = *n1;
  *n1 = *n2;
  *n2 = tmp;
}

static inline int node_dist(node_t *n1, node_t *n2) {
  return abs(n1->x - n2->x) + abs(n1->y - n2->y);
}

static inline int node_index(node_t *n, node_t **tour, int num_nodes) {
  for(int i = 0; i < num_nodes; ++i)
    if(n == tour[i]) return i;
  return -1;
}

static inline void node_remove(node_t *n, node_t **tour, int *num_nodes) {
  for(int i = 0; i < *num_nodes; ++i) {
    if(n == tour[i]) {
      *num_nodes -= 1;
      tour[i] = tour[*num_nodes];
      tour[*num_nodes] = NULL;
      return;
    }
  }
  abort();
}

static inline void tour_validate(node_t **nodes, int num_nodes) {
#ifdef VALIDATE
  for(int i = 0; i < num_nodes-1; ++i) {
    for(int j = i+1; j < num_nodes; ++j) {
      if(nodes[i]->x == nodes[j]->x && nodes[i]->y == nodes[j]->y)
        abort();
    }
  }
#endif
}

static void chromosome_crossover(node_t **p1, node_t **p2, node_t **child, int num_nodes) {
  node_t **left  = (node_t**)malloc(sizeof(node_t*) * num_nodes);
  node_t *n, *s1, *s2;
  int    idx, num_left = num_nodes;

  if(left == NULL)
    return;

  memcpy(left, p1, sizeof(node_t*) * num_nodes);

  n = child[0] = rand()%2 ? p1[0] : p2[0];
  node_remove(n, left, &num_left);

  for(int i = 1; i < num_nodes; ++i) {
    s1 = s2 = NULL;

    if((idx = node_index(n, p1, num_nodes-1)) >= 0)
      s1 = p1[idx+1];
    if(node_index(s1, child, i) >= 0)
      s1 = NULL;
    if((idx = node_index(n, p2, num_nodes-1)) >= 0)
      s2 = p2[idx+1];
    if(node_index(s2, child, i) >= 0)
      s2 = NULL;

    if(s1 != NULL && s2 != NULL) {
      if(node_dist(n, s1) < node_dist(n, s2))
        n = s1;
      else
        n = s2;
    }
    else if(s1 == NULL && s2 != NULL)
      n = s2;
    else if(s1 != NULL && s2 == NULL)
      n = s1;
    else {
      int distance = 999, d;
      for(int j = 0; j < num_left; ++j) {
        if((d = node_dist(n, left[j])) < distance)
          n = left[j];
      }
    }

    tour_validate(child, i);
    child[i] = n;
    tour_validate(child, i+1);
    node_remove(n, left, &num_left);
  }

  free(left);
}

static void chromosome_mutate(int posx, int posy, node_t **nodes, int num_nodes) {
  for(int j = 0; j < MUTATION_MAX && rand() % 100 < MUTATION_CHANCE; ++j) {
    /* try to swap, but only accept swaps that give better rank */
    int r = rand()%(num_nodes-1);
    int score = rank(posx, posy, nodes, num_nodes);
    node_swap(&nodes[r], &nodes[r+1]);
    if(rank(posx, posy, nodes, num_nodes) > score)
      node_swap(&nodes[r], &nodes[r+1]);
  }
}

static void mutate_twins(int posx, int posy, rank_t *ranks, int num_nodes) {
  /* only mutate twins who are elite in the interest of time */
  for(int i = 0; i < ELITE_SIZE-1; ++i) {
    for(int j = i+1; j < ELITE_SIZE; ++j) {
      if(ranks[i].value == ranks[j].value
      && memcmp(ranks[i].nodes, ranks[j].nodes, sizeof(node_t*) * num_nodes) == 0) {
        chromosome_mutate(posx, posy, ranks[j].nodes, num_nodes);
        tour_validate(ranks[j].nodes, num_nodes);
        ranks[j].value = rank(posx, posy, ranks[j].nodes, num_nodes);
      }
    }
  }
}

static node_t** choose_elite(int v, rank_t *ranks) {
  for(int i = 0; i < ELITE_SIZE; ++i)
    if(ranks[i].elite_score >= v)
      return ranks[i].nodes;
  abort();
}

static void* worker(void* arg) {
  work_t *work = (work_t*)arg;
  node_t ***nodes;
  rank_t *ranks;
  int    elite_total, elite_max;
  int    repeats, last_elite_total;

  nodes = (node_t***)malloc(sizeof(node_t**) * POPULATION_SIZE);
  if(nodes == NULL)
    abort();
  for(int i = 0; i < POPULATION_SIZE; ++i) {
    nodes[i] = (node_t**)malloc(work->num_nodes * sizeof(node_t*));
    if(nodes[i] == NULL)
      abort();
    for(int j = 0; j < work->num_nodes; ++j)
      nodes[i][j] = &work->nodes[j];
    for(int j = 0; j < work->num_nodes * 4; ++j)
      node_swap(&nodes[i][rand()%work->num_nodes], &nodes[i][rand()%work->num_nodes]);
  }

  ranks = (rank_t*)malloc(sizeof(rank_t) * POPULATION_SIZE);
  if(ranks == NULL)
    abort();
  for(int i = 0; i < POPULATION_SIZE; ++i) {
    ranks[i].nodes = nodes[i];
    ranks[i].value = rank(work->posx, work->posy, nodes[i], work->num_nodes);
  }
  qsort(ranks, POPULATION_SIZE, sizeof(rank_t), rank_compare);
  work->results = ranks;

  pthread_mutex_lock(&work->mutex);
  while(1) {
    while(work->op == WAIT || work->op == RESULT) {
      pthread_cond_signal(&work->cond);
      pthread_cond_wait(&work->cond, &work->mutex);
    }

    if(work->op == QUIT) {
      pthread_mutex_unlock(&work->mutex);
      for(int i = 0; i < POPULATION_SIZE; ++i)
        free(nodes[i]);
      free(nodes);
      return NULL;
    }

    pthread_mutex_unlock(&work->mutex);

    /* prepare elite values and repeat state for first generation */
    elite_total = elite_max = 0;
    repeats = last_elite_total = 0;
    for(int i = 0; i < ELITE_SIZE; ++i)
      if(ranks[i].value > elite_max)
        elite_max = ranks[i].value;
    for(int i = 0; i < ELITE_SIZE; ++i) {
      elite_total += elite_max+1 - ranks[i].value;
      ranks[i].elite_score = elite_total;
    }

    for(int g = 0; g < GENERATIONS && repeats < REPEAT_MAX; ++g) {
      for(int i = ELITE_SIZE; i < POPULATION_SIZE; ++i) {
        node_t **p1;
        node_t **p2;

        /* make sure we get two different parents, but they are both elite */
        p1 = choose_elite(rand() % elite_total, ranks);
        while((p2 = choose_elite(rand() % elite_total, ranks)) == p1)
          ;

        tour_validate(p1, work->num_nodes);
        tour_validate(p2, work->num_nodes);

        /* perform crossover */
        if(rand() % 100 < CROSSOVER_CHANCE)
          chromosome_crossover(p1, p2, ranks[i].nodes, work->num_nodes);
        tour_validate(ranks[i].nodes, work->num_nodes);

        /* perform mutation */
        chromosome_mutate(work->posx, work->posy, ranks[i].nodes, work->num_nodes);
        tour_validate(ranks[i].nodes, work->num_nodes);

        /* update rank info */
        ranks[i].value = rank(work->posx, work->posy, ranks[i].nodes, work->num_nodes);
      }

      /* try to eliminate twins by mutating them; this will update ranks if a mutation occurs */
      mutate_twins(work->posx, work->posy, ranks, work->num_nodes);

      /* sort the population by rank (lower score is better */
      qsort(ranks, POPULATION_SIZE, sizeof(rank_t), rank_compare);

      /* prepare elite values for next generation */
      elite_total = elite_max = 0;
      for(int i = 0; i < ELITE_SIZE; ++i)
        if(ranks[i].value > elite_max)
          elite_max = ranks[i].value;
      for(int i = 0; i < ELITE_SIZE; ++i) {
        elite_total += elite_max+1 - ranks[i].value;
        ranks[i].elite_score = elite_total;
      }

      /* update repeat count */
      if(elite_total == last_elite_total)
        repeats++;
      else
        last_elite_total = elite_total;
    }

    pthread_mutex_lock(&work->mutex);
    work->op = RESULT;
    pthread_cond_signal(&work->cond);
  }
}
