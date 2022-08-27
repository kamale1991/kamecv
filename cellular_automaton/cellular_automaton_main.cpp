#include <iostream>
#include "include/template.h"
#include "include/pattern_sample.h"

int main()
{
    std::cout << "template_main" << std::endl;
    std::cout << "Fibonacci(10) = " << fibonacci(10) << std::endl;
    Pattern::AbstructFactory::run();
    std::cout << std::endl;

    Pattern::Builder::run();
    std::cout << std::endl;

    Pattern::FactoryMethod::run();
    std::cout << std::endl;

    Pattern::Prototype::run();
    std::cout << std::endl;
}