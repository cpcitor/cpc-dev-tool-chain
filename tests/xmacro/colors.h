/** The elements and their values are defined in a separate file colors.def.

Some variante of the XMACRO pattern define another macro inline.

This has consequences:

* scalability: using a local macro forces to keep all elements on the same line,
or with backslash continuation.
* convenience: backslash continuation are error-prone
* values are in a code file, which is not practical if values have to be
generated.

Putting all values in a separate file solves this issue.

Remember that C's "#include" are of free use.  The common include
pattern is just one of many.  All are legal.

*/

/** Parse the external file to declare an enum with all values. */
typedef enum color
{
#define _XMACRO_ELEMENT( name ) name,
#include "colors.def"
#undef _XMACRO_ELEMENT
} color;

/** Parse the external file again to declare an array of names. */
char *color_name[] = {
#define _XMACRO_ELEMENT( name ) #name,
#include "colors.def"
#undef _XMACRO_ELEMENT
};

/** Parse the external file again to count values. */
#define XMACRO_COUNT_TYPE char
typedef struct
{
#define _XMACRO_ELEMENT( name ) XMACRO_COUNT_TYPE name;
#include "colors.def"
#undef ENTRY
} __dummy_colorname_count_t;
enum
{
    color_count =
        sizeof( __dummy_colorname_count_t ) / sizeof( XMACRO_COUNT_TYPE )
};

/* When elements have varying sizes, it make sense to repeat the
 * struct trick above with a union.  They sizeof(the_union) provides
 * the size of the largest element.
 */

/** Now it's easy to provide a conversion function. */

char *
color_to_colorname( color color_ )
{
    return color_name[color_];
}
