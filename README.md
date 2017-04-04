# CalculaTron

Basic calculator as a school project.
Compatible with Linux 64bit.

## Install

```{r, engine='bash', install}
make install
`````

## Documentation

### Class: AteamMathLib

Class for our Math library.

```cpp
namespace MathLib
{
  class AteamMathLib 
  {
  
  public:

    static unsigned long long int factorial (short int x);

    static double power (double x, int n);

    static double nroot (double x, int n);
    
  };
}
```

### Math library

* `factorial(n)` Factorial of n.
* `power(x, n)` Nth power of x.
* `nroot(x, n)` Nth root of x.

## Similar Projects

Other great watch libraries to try are:

* Default Windows calculator
* Google search calculator

## License
Copyright (c) 2017, Daniel Uhricek, Jan Sorm, Peter Uhrin, Alena Tesarova
Licensed under GNU GPL v3.0
