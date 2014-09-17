#include <defs.h>
#include <math.h>

int strcmp(char *s1, char *s2)
{
       while (*s1 == *s2++) {
          if (*s1++ == 0){ 
               return (0);
                 }
            }
        return (*(unsigned char *) s1 - *(unsigned char *) --s2);
}


int pow(int x, int y)
{
      if( y == 0)
                return 1;
          else if (y%2 == 0)
                    return pow(x, y/2)*pow(x, y/2);
              else
                        return x*pow(x, y/2)*pow(x, y/2);
               
}

int getSize(char *p) 
{
   int k = 0;
    while (*p) {
       k = (k<<3)+(k<<1)+(*p)-'0';
        p++;
         }

     int decimal=0, i=0, rem; 
      while (k!=0) 
         { 
             rem = k%10; 
               k/=10; 
                 decimal += rem*pow(8,i); 
                   ++i; 
                    } 
       return decimal; 
}
