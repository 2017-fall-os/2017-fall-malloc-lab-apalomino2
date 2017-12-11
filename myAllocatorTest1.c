#include "stdio.h"
#include "stdlib.h"
#include "myAllocator.h"
#include "sys/time.h"
#include <sys/resource.h>
#include <unistd.h>

double diffTimeval(struct timeval *t1, struct timeval *t2) {
  double d = (t1->tv_sec - t2->tv_sec) + (1.0e-6 * (t1->tv_usec - t2->tv_usec));
  return d;
}

void getutime(struct timeval *t)
{
  struct rusage usage;
  getrusage(RUSAGE_SELF, &usage);
  *t = usage.ru_utime;
}

int main() 
{
  void *p1, *p2, *p3, *p4, *p5;
  arenaCheck();
  p1 = bestFitAllocRegion(32);
  arenaCheck();
  p2 = bestFitAllocRegion(48);
  arenaCheck();
  p3 = bestFitAllocRegion(20);
  arenaCheck();
  p4 = bestFitAllocRegion(40);
  arenaCheck();
  printf("%8zx %8zx %8zx\n", p1, p2, p3);
  printf("Initial\n");
  arenaCheck();
  freeRegion(p1);
  printf("Free p1\n");
  arenaCheck();
  freeRegion(p3);
  printf("Free p3\n");
  arenaCheck();  
  p1 = bestFitAllocRegion(24);
  printf("Best fit for 24\n");
  arenaCheck();
  freeRegion(p3);
  printf("Free p3\n");
  arenaCheck();  
  resizeRegion(p2, 64);
  printf("Resize p2 to 64\n");
  arenaCheck();
  {				/* measure time for 10000 mallocs */
    struct timeval t1, t2;
    int i;
    getutime(&t1);
    for(i = 0; i < 10000; i++)
      if (firstFitAllocRegion(4) == 0) 
	break;
    getutime(&t2);
    printf("%d firstFitAllocRegion(4) required %f seconds\n", i, diffTimeval(&t2, &t1));
  }
  return 0;
}



