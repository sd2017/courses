#include <stdio.h>
#include <setjmp.h>
#include <stdlib.h>
#include <signal.h>

/* output from a sample run:

   SIGFPE is defined as 8
   Two integers: 100 10
   100 / 10 == 10
   Two integers: 100 0
        Control to jumper: signal status == 8.
   Two integers: 100 4
   100 / 4 == 25
   Two integers:  ;; I entered Control-C to terminate program
*/

/* Define a jump buffer to handle error conditions.
   Data type jmp_buf defined in setjmp.h.
*/
static jmp_buf env;       /* file scope; type is defined in setjmp.h */

void jumper(int);         /* callback for SIGFPE errors */
void divisionInt(void);   /* alternative to void divisionInt() */
int guard(void);          /* guards divisonInt */

void trace_and_die(const char* msg) {
  fprintf(stderr, msg);
  exit(EXIT_FAILURE);
}

int main() {
  /* Trap signals generated by arithmetic errors such as 
     division by zero. The statement
     
     signal(SIGINT, SIG_IGN);

     would instruct the program to ignore (SIG_IGN) interrupts such
     as hitting Control-C while the program is in execution.

     In this example, we want to suppress floating-point exceptions
     from the system and instead handle the matter ourselves.

     Withtout this setup, the division of, say, 100 by 0 would generate
     a "floating-point exception" and terminate execution.
  */
  signal(SIGFPE, jumper); 
  printf("SIGFPE is defined as %i\n", SIGFPE);

  /* Loop until error condition occurs. */
  while (0 == guard())
    ;
  return 0;
}

/* Jump out of this function to the jump destination,
   which is the point after the call to setjmp. In
   our case, the jump point occurs right after the
   if-test in function guard.
*/
void jumper(int status) {
  printf("\t Control to jumper: signal status == %d.\n", status);
  longjmp(env, 1);  /* 1 indicates with setjmp would have returned.
		       If successful, longjmp forces control to return
		       to the next statement after setjmp, in this case
		       a call to divisionInt.
		     */
  /* This code is executed only if longjmp fails. Normally, longjmp
     does not return.
  */
  trace_and_die("longjmp returned: trouble\n");
}

/* Set a jump point for return from an error  condition, in this case 
   an error resulting from a floating-point operation. The function
   signal is invoked in main to trap such errors. Return 0 (ok) if the 
   floating-point operation succeeds; otherwise, the nonlocal jump
   of longjmp intervenes.
*/
int guard(void) {
  /* Set jump point, which is right after the if-test */
  setjmp(env);
  divisionInt();
  return 0;
}

/* Scan the standard input for two floats and
   divide the first by the second.
*/
void divisionInt(void) {
  int n1, n2;
  printf("Two integers: ");
  scanf("%i %i", &n1, &n2);
  printf("%i / %i == %i\n", n1, n2, n1 / n2);
}

