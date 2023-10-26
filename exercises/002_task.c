// includes as usual
#include <assert.h>
#include <stdio.h>

// whatever comes before the function name is the return type
// The return type can be any of the standart data types but 
// also self defined types, which we will learn about later

// we can define a function like this
// it returns nothing and is called function_2. Whatever is
// inside the curly brackets will be executed when the 
// function is called
void function_2() {
    printf("we called function 2");
}

// we can also define a function like this, with an integer as 
// return type, like the main function from the previous example had
int function_1() {
    // we can call another function with the name of the function folloed by
    // brackets
    function_2();
    // we return a value from a function like this
    return 0;
}

// ignore this, these are to test your code
#define TEST int main
TEST(void) {
    assert(function_1() == 0);
    return 0;
}
