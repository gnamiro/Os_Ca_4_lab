#include "types.h"
#include "stat.h"
#include "user.h"

int main()
{
  int ret1, ret2;

  barrier_init(2, 0);
  barrier_init(3, 1);


  ret1 = fork();
  if(ret1 == 0)
    {

      sleep(10);

      printf(1, "Child 1 at barrier0\n");

      barrier_check(0);
      printf(1, "Child 1 cleared barrier0\n");

      printf(1, "Child 1 at barrier1\n");
      barrier_check(1);
      printf(1, "Child 1 cleared barrier1\n");


      exit();
    }
  else
    {
      ret2 = fork();

      if(ret2 == 0)
	{
	  sleep(20);
	  printf(1, "Child 2 at barrier0\n");

	  barrier_check(0);

      printf(1, "Child 2 cleared barrier0\n");

      printf(1, "Child 2 at barrier1\n");
	  barrier_check(1);
	  printf(1, "Child 2 cleared barrier1\n");


	  exit();
	}
      else
	{
	  printf(1, "Parent at barrier0\n");

	  barrier_check(0);

      printf(1, "Parent cleared barrier0\n");

      printf(1, "Parent at barrier1\n");
      barrier_check(1);
      printf(1, "Parent cleared barrier1\n");



	  wait();
	  wait();
	  exit();

	}

    }
}
