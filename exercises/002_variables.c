// to store data in c you can assign it to a variable with a name
// which you can reuse later
//
// you define variables by entering a type, followed by the variables name
// and an otionl assignment, like this:
//
//     int number = 1;
// 
// now we defined an integer with the name of number, which we can resuse later
//
//     printf("%d", number);
//
// here we are printing it to the standart output
//
// There are many different types in c, but here are some of the more important ones:
//
//     int               -> 16 bit signed integer [-32,767, +32,767]
//     unsigned int      -> 16 but unsigned integer [0, +65,535]
//     long              -> 32 bit signed integer [−2,147,483,647, +2,147,483,647]
//     unsigned long     -> 32 but unsigned integer [0, 4,294,967,295]
//     char              -> a single character
//     char*             -> a string type in c
//     float             -> a 32 bit decimal
//     double            -> a 64 bit decimal
//
// Now let's define some variables!
//

#include <stdio.h>

int main() {
    // TODO: Fix this program
    //
    // ??? number1 = 100;
    // ??? number2 = number1 / 2;
    //
    // printf("number 1 + number 2 = %d", number1 + ???)
    //
    int number1 = 100;
    int number2 = number1 / 2;

    printf("number 1 + number 2 = %d", number1 + number2);
}

