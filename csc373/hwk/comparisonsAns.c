#include <stdio.h>
#include <limits.h>

int main() {

  int n1 = 2;
  int n2 = 3;

  unsigned int u1 = (unsigned int) n1;  
  unsigned int u2 = (unsigned int) n2;

  /* Is result always 1? Explain. 

     Answer: No
     Explanation (treating leftmost as sign bit and rightmost as 
                  the least significant bit):

        Set one of n1 and n2 to INT_MIN and the other to any other value, e.g., 
        n1 is INT_MIN and  n2 is 0. If both are INT_MIN, the equivalence
        is true. In this example, n2 could be any value other than INT_MIN; for
        example, n2 is 0. Then 

            n1 == 1000...000   ;; 32 bits
            n2 == 0000...000   ;; 32 bits

        Then n1 < n2 because sign-bit in n1 is 1 (negative) and sign-bit in n2
        is 0 (non-negative). Hence, n1 < n2 == 1 (true).

        Now consider -n1 > -n2, where reversing the sign is equivlanet
        to taking the 2's complement.

           -n1 == ~(1000...000) + 1 == 0111...111 + 1 == 1000...000 == n1
           -n2 == ~(0000...000) + 1 == 1111...111 + 1 == 0000...000 == n2

        Then it's not the case that -n1 > -n2 because, again, -n1's sign is
        1 (negative) and -n2's sign is 0 (non-negative). 
        Hence, -n1 > -n2 == 0 (false). 

        The underlying problem is that INT_MIN == -INT_MIN. That's true only
        for INT_MIN among the 2's complement, 32-bit integers.
  */
  n1 = INT_MIN, n2 = 0;
  int result = (n1 < n2) == (-n1 > -n2);
  printf("(%i < %i) == (-%i > -%i) evaluates to %d\n", 
	 n1, n2, n1, n2, result);

  /****************************************************************************/

  /* Is result always 0? Explain. 

     Answer: Yes, result is always zero.

     Explanation: 

     There are various ways to tackle this one. For starters, let's try an
     analytical approach. Let n1 = 2 and n2 = 3. Then

                   n1 == 2        ;; n1
                  ~n1 == -3       ;; 1's complement of n1
                  ~n1 + 1 == -2   ;; 2's complement of n1

     Let's summarize this by saying that the 1's complement of n1 is "off by 1"
     from the 2's complement because -3 (1's complement) differs by 1 from 
     -2 (2's complement). The same holds, of course, for ~n2, which is off by 1
     from the 2's complement of n2. Now consider that

                     n1 + n2  ==  5    
                    ~n1 + ~n2 == -7
        (~n1 + 1) + (~n2 + 1) == -5

     So the addition of two "off by 1" integers results in a sum that is 
     "off by 2" from the addition using the 2's complements. Finally, note that

                   ~(n1 + n2) == -6  ;; "off by 1"
 
     is "off by 1" rather than "off by 2." In general, then, ~n1 + ~n2 will be off
     by 2, whereas ~(n1 + n2) will be off by 1. So the two expressions cannot be equal.
 
     The same point can be illustrated in binary by using 2-bit rather than 32-bit
     integers:

          n1   n2  ~n1  ~n2  (~n1 + ~n2)  ~(n1 + n2)
          ------------------------------------------
          00   00   11   11       00          11
          00   01   11   10       01          10
          00   10   11   01       00          01
          00   11   11   00       11          00
          01   01   10   10       00          01
          01   10   10   01       11          00
          01   11   10   00       10          11
          10   10   01   01       10          11
          10   11   01   00       01          10
          11   11   00   00       00          11

     Finally, there's a (very, very, very, very, very,...,very) long experiment to
     settle the matter. Here's the code (run only if you're not in hurry for the
     program to terminate):

         #include <stdio.h>
         #include <limits.h>

         int main() {
            int i, j;
            for (i = INT_MIN; i <= INT_MAX; i++) {
               for (j = i; j <= INT_MAX; j++) {
                  int result = (~i + ~j) == ~(i + j);
                  if (result) {
	            printf("Counter example!\n");
	            printf("i == %d and j == %d\n", i, j);
	            exit(0);
                  }
               }
            }
            printf("In no case is (~n1 + ~n2) == ~(n1 + n2)\n");
            return 0;
         }
  */
  n1 = -1, n2 = -1;
  result = ~n1 + ~n2 == ~(n1 + n2);
  printf("\t\t%d %d %d %d %d\n", n1, ~n1, (~n1 + ~n2), (n1 + n2), ~(n1 + n2));
  printf("(~%x + ~%x) == ~(%x + %x) evaluates to %d\n", 
	 n1, n2, n1, n2, result);

  /***************************************************************************/

  /* Is result always 1? Explain. 
     
     Answer: Yes

     Explanation: Because the shifts are 1 bit in length, there are only two
     bit positions that could impact the result: a 1 in the leftmost position 
     the sign position) and a 1 in the rightmost postion. Here are the cases:

               1?????...????

     Whether the 1-bit right shift is arithemtic or logical

               01????...????   ;; logical
               11????...????   ;; arithmetic

     the resulting left shift restores a 1 to the sign position; hence, n1
     remains negative. (If n1 were non-negative to begin, the shifting leaves
     n1 non-negative.)

     The other important condition is the rightmost bit. Assume a 1.

               ?????...???1

     After the right shift we have

               ?????...????

     and after the following left shift, with a fill bit of 0, we have

               ?????...???0

     So the magnitude has decreased by 1. In the case where n1 = 1, 
     ((n1 >> 1) << 1) equals 0; but note that the comparison
     is <= rather than ==. So

               n1 = 1;                  
               ((n1 >> 1) << 1) <= n1; 

    evaluates to 1. In summary, the shifting leaves an even signed int unchanged
    but decrements an odd signed int by 1.
  */
  result = ((n1 >> 1) << 1) <= n1;
  printf("((%d >> 1) << 1) <= %d evaluates to %d\n",
	 n1, n1, result);

  /* Is result always 1? Explain. 

     Answer: Yes

     Explanation: By assumption, the same bit patterns are stored in
     n1 and u1

                  int n1 = <some expression>;
         unsigned int u1 = (unsigned) n1;

     and, in similar fashion, the same bit strings are stored in n2 and u2.
     Further, the applicable arithmetic operations (subtraction and unary -, in 
     this case) work exactly the same. For example, in the code segment

                  unsigned int x1 = 7, x2 = 5, ans;
                  x2 = ~x2;
                  x2 += 1;
                  ans = x1 + x2;
     
     ans is 2. In other words, subtraction is done via (a) 2's complement and 
     (b) addition, just as with signed values. Now suppose that we subtract
     the bigger from the smaller:

                  unsigned int x1 = 5, x2 = 7, ans;
                  x2 = ~x2;
                  x2 += 1;
                  ans = x1 + x2;

     In this case, ans is 4,294,967,294 (11111111111111111111111111111110 in binary)
     because the bit string is _interpreted_ as unsigned and the leftmost 1 therefore
     represents the most significant magnitude bit rather than the sign bit. However,
     if this bit string is interpreted as signed, its value is -2. Recall that, to 
     get the absolute value of 11111111111111111111111111111110 as a signed integer
     we compute its 2's complement:

                  00000...0001     ;; 1's complement
                 +           1     ;; 2's complement
                  ------------
                  00000...0010     ;; 2

     In summary, at the bit level n1 == u1 and n2 == u2 and the underlying
     arithmetic operations work the same. So the only difference is in the
     interpretation, signed versus unsigned. In the example, however, the
     value (u1 - u2) is cast to an int. In effect, then, the expression 
     
                ((int) u1 - u2)) == -(n2 - n1)

     is equivalent to

                (n1 - n2) == -(n2 - n1)

     If n1 is larger, both sides evaluate to non-negative integers.
     If n1 is smaller, both sides evaluate to negative integers.
     If n1 and n2 are the same, both sides evaluate to zero. (In 2's complement,
     +0 == -0.)
  */
  result = ((int) (u1 - u2)) == -(n2 - n1);
  printf("((int) (%x - %x)) == -(%x - %x) evaluates to %d\n",
	 u1, u2, n2, n1, result);

  return 0;
}
