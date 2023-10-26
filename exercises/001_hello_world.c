// +----------------------------------------------------+
// |                welcome to C-LINGS                  |
// | So you want to learn the c language. Great choice! |
// | C is a very old language  but there is a reason it |
// | is  still in  use and it is  also a  language that |
// | teaches a  lot of low  level concepts  like memory |
// | management  and many  other things  which we  will |
// | one after  another in  this course,  starting with |
// | displaying something on the terminal.              |
// |                                                    |
// | This program is inspired by projects like ziglings |
// | and rustlings                                      |
// +----------------------------------------------------+

// this is an import of extern code which we can use in our 
// code but we will learn more about this later.
#include <stdio.h>

// This is the main function. It is the entry point of the 
// program
int main(void) {
    // This function is from the stdio library we just importet
    // which enables us to print something in the terminal,
    // but there is something missing.
    //
    // TODO: Make this program print "hello world" on the screen
    printf("hello world");

    // we return 0 to signal that the program completet with no errors
    return 0;
}
