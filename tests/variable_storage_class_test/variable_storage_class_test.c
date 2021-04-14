#include <stdio.h>

// This file is used to test and clarifies what variables become.

// ## Symbols ?

// ## Symbols and global/static variables
// As expected, sdcc create a global symbol for global variables, not for static
// variable. 	.globl _int_initialized 	.globl _int_uninitialized
// .globl _const_int_initialized

// ## Symbols and local variables
// Local variables are allocated on the stack.
// No symbol (even a local one) is generated for a local variable.

// sdcc puts a (global or static) int uninitialized to DATA area
static int static_int_uninitialized;
int int_uninitialized;
// becomes :
// ;--------------------------------------------------------
// ; ram data
// ;--------------------------------------------------------
// 	.area _DATA
// _static_int_uninitialized:
// 	.ds 2
// _int_uninitialized::
// 	.ds 2

// sdcc puts a (global or static) int initialized to INITIALIZED area
static int static_int_initialized = 42;
int int_initialized = 42;
// becomes :
// ;--------------------------------------------------------
// ; ram data
// ;--------------------------------------------------------
// 	.area _INITIALIZED
// _static_int_initialized:
// 	.ds 2
// _int_initialized::
// 	.ds 2

// 	.area _CODE
// 	.area _INITIALIZER
// __xinit__static_int_initialized:
// 	.dw #0x002A
// __xinit__int_initialized:
// 	.dw #0x002A
// 	.area _CABS (ABS)

// So, for global variables to be correctly initialized, crt0 has to copy
// _INITIALIZER section to _INITIALIZED.

// sdcc puts a (global or static) const int initialized to CODE area and, if
// non-static, create a global symbol.
static const int static_const_int_initialized = 42;
const int const_int_initialized = 42;

//	.globl _const_int_initialized
// 	.area _CODE
// _static_const_int_initialized:
// 	.dw #0x002A
// _const_int_initialized:
// 	.dw #0x002A

// SDCC refuses to compile uninitialized const int. It does not make great sense
// but gcc accepts.
// static const int static_const_int_uninitialized;
// const int const_int_uninitialized;

// What about unused const ?
// SDCC treats them just like used const: put it in code and, if non-static,
// create a global symbol. Non-static const might be used by other modules,
// which totally makes sense. Not sure about unused static const.  Either SDCC
// author just didn't think about it, or it might be useful with some linking
// hacks?

static const int static_const_int_initialized_unused = 686;
const int const_int_initialized_unused = 686;

//	.globl _const_int_initialized_unused
// 	.area _CODE
// _static_const_int_initialized_unused:
// 	.dw #0x02ae
// _const_int_initialized_unused:
// 	.dw #0x02ae

typedef struct test_t
{
    char field_a;
    char field_b;
} test_t;

test_t global_testinstance_initialized = { 8, 13 };
const test_t const_global_testinstance_initialized = { 8, 13 };

int
main()
{

    int local_int_uninitialized;
    int local_int_initialized = 42;

    const int const_local_int_uninitialized;
    const int const_local_int_initialized = 42;

    test_t local_testinstance_initialized = { 42, 64 };

#define PRINT_VAR( x ) printf( "%d\t= " #x "\r\n", x );
#define PRINT_PTR( x ) printf( "%p\t<- " #x "\r\n", &x );

#define PRINT_BOTH( x )                                                        \
    PRINT_VAR( x );                                                            \
    PRINT_PTR( x );

    PRINT_VAR( static_int_uninitialized );
    PRINT_VAR( int_uninitialized );

    PRINT_VAR( static_int_initialized );
    PRINT_VAR( int_initialized );

    PRINT_VAR( static_const_int_initialized );
    PRINT_VAR( const_int_initialized );

    // PRINT_VAR(static_const_int_uninitialized);
    // PRINT_VAR(const_int_uninitialized);

    PRINT_VAR( local_int_uninitialized );
    PRINT_VAR( local_int_initialized );
    PRINT_VAR( const_local_int_uninitialized );
    PRINT_VAR( const_local_int_initialized );

    PRINT_PTR( static_int_uninitialized );
    PRINT_PTR( int_uninitialized );

    PRINT_PTR( static_int_initialized );
    PRINT_PTR( int_initialized );

    PRINT_PTR( static_const_int_initialized );
    PRINT_PTR( const_int_initialized );

    // PRINT_PTR(static_const_int_uninitialized);
    // PRINT_PTR(const_int_uninitialized);

    PRINT_PTR( local_int_uninitialized );
    PRINT_PTR( local_int_initialized );
    PRINT_PTR( const_local_int_uninitialized );
    PRINT_PTR( const_local_int_initialized );

    return 0;
}
