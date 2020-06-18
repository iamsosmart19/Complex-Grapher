#include <gio/gio.h>

#if defined (__ELF__) && ( __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 6))
# define SECTION __attribute__ ((section (".gresource.glib_resources"), aligned (8)))
#else
# define SECTION
#endif

#ifdef _MSC_VER
static const SECTION union { const guint8 data[577]; const double alignment; void * const ptr;}  glib_resources_resource_data = { {
  0107, 0126, 0141, 0162, 0151, 0141, 0156, 0164, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 
  0030, 0000, 0000, 0000, 0344, 0000, 0000, 0000, 0000, 0000, 0000, 0050, 0007, 0000, 0000, 0000, 
  0000, 0000, 0000, 0000, 0001, 0000, 0000, 0000, 0004, 0000, 0000, 0000, 0005, 0000, 0000, 0000, 
  0006, 0000, 0000, 0000, 0006, 0000, 0000, 0000, 0007, 0000, 0000, 0000, 0107, 0002, 0032, 0135, 
  0003, 0000, 0000, 0000, 0344, 0000, 0000, 0000, 0006, 0000, 0114, 0000, 0354, 0000, 0000, 0000, 
  0360, 0000, 0000, 0000, 0213, 0346, 0223, 0166, 0002, 0000, 0000, 0000, 0360, 0000, 0000, 0000, 
  0010, 0000, 0114, 0000, 0370, 0000, 0000, 0000, 0000, 0001, 0000, 0000, 0366, 0325, 0124, 0153, 
  0000, 0000, 0000, 0000, 0000, 0001, 0000, 0000, 0014, 0000, 0114, 0000, 0014, 0001, 0000, 0000, 
  0020, 0001, 0000, 0000, 0173, 0242, 0170, 0174, 0004, 0000, 0000, 0000, 0020, 0001, 0000, 0000, 
  0003, 0000, 0114, 0000, 0024, 0001, 0000, 0000, 0030, 0001, 0000, 0000, 0324, 0265, 0002, 0000, 
  0377, 0377, 0377, 0377, 0030, 0001, 0000, 0000, 0001, 0000, 0114, 0000, 0034, 0001, 0000, 0000, 
  0040, 0001, 0000, 0000, 0173, 0176, 0254, 0041, 0001, 0000, 0000, 0000, 0040, 0001, 0000, 0000, 
  0015, 0000, 0166, 0000, 0060, 0001, 0000, 0000, 0303, 0001, 0000, 0000, 0131, 0253, 0135, 0230, 
  0001, 0000, 0000, 0000, 0303, 0001, 0000, 0000, 0015, 0000, 0166, 0000, 0320, 0001, 0000, 0000, 
  0100, 0002, 0000, 0000, 0163, 0061, 0155, 0067, 0165, 0057, 0000, 0000, 0002, 0000, 0000, 0000, 
  0147, 0154, 0146, 0151, 0154, 0145, 0163, 0057, 0006, 0000, 0000, 0000, 0005, 0000, 0000, 0000, 
  0143, 0160, 0154, 0170, 0147, 0162, 0141, 0160, 0150, 0145, 0162, 0057, 0001, 0000, 0000, 0000, 
  0151, 0157, 0057, 0000, 0000, 0000, 0000, 0000, 0057, 0000, 0000, 0000, 0003, 0000, 0000, 0000, 
  0166, 0145, 0162, 0164, 0123, 0150, 0141, 0144, 0056, 0166, 0145, 0162, 0164, 0000, 0000, 0000, 
  0203, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0043, 0166, 0145, 0162, 0163, 0151, 0157, 0156, 
  0040, 0061, 0065, 0060, 0012, 0012, 0151, 0156, 0040, 0166, 0145, 0143, 0064, 0040, 0160, 0157, 
  0163, 0151, 0164, 0151, 0157, 0156, 0073, 0012, 0151, 0156, 0040, 0166, 0145, 0143, 0064, 0040, 
  0143, 0157, 0154, 0157, 0162, 0073, 0012, 0157, 0165, 0164, 0040, 0166, 0145, 0143, 0064, 0040, 
  0166, 0145, 0162, 0164, 0137, 0143, 0157, 0154, 0157, 0162, 0073, 0012, 0012, 0166, 0157, 0151, 
  0144, 0040, 0155, 0141, 0151, 0156, 0050, 0051, 0040, 0173, 0012, 0011, 0147, 0154, 0137, 0120, 
  0157, 0163, 0151, 0164, 0151, 0157, 0156, 0040, 0075, 0040, 0160, 0157, 0163, 0151, 0164, 0151, 
  0157, 0156, 0073, 0012, 0011, 0166, 0145, 0162, 0164, 0137, 0143, 0157, 0154, 0157, 0162, 0040, 
  0075, 0040, 0143, 0157, 0154, 0157, 0162, 0073, 0012, 0175, 0012, 0000, 0000, 0050, 0165, 0165, 
  0141, 0171, 0051, 0146, 0162, 0141, 0147, 0123, 0150, 0141, 0144, 0056, 0146, 0162, 0141, 0147, 
  0140, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0043, 0166, 0145, 0162, 0163, 0151, 0157, 0156, 
  0040, 0061, 0065, 0060, 0012, 0012, 0151, 0156, 0040, 0166, 0145, 0143, 0064, 0040, 0166, 0145, 
  0162, 0164, 0137, 0143, 0157, 0154, 0157, 0162, 0073, 0012, 0157, 0165, 0164, 0040, 0166, 0145, 
  0143, 0064, 0040, 0157, 0165, 0164, 0137, 0143, 0157, 0154, 0157, 0162, 0073, 0012, 0012, 0166, 
  0157, 0151, 0144, 0040, 0155, 0141, 0151, 0156, 0050, 0051, 0040, 0173, 0012, 0011, 0157, 0165, 
  0164, 0137, 0143, 0157, 0154, 0157, 0162, 0040, 0075, 0040, 0166, 0145, 0162, 0164, 0137, 0143, 
  0157, 0154, 0157, 0162, 0073, 0012, 0175, 0012, 0000, 0000, 0050, 0165, 0165, 0141, 0171, 0051
} };
#else /* _MSC_VER */
static const SECTION union { const guint8 data[577]; const double alignment; void * const ptr;}  glib_resources_resource_data = {
  "\107\126\141\162\151\141\156\164\000\000\000\000\000\000\000\000"
  "\030\000\000\000\344\000\000\000\000\000\000\050\007\000\000\000"
  "\000\000\000\000\001\000\000\000\004\000\000\000\005\000\000\000"
  "\006\000\000\000\006\000\000\000\007\000\000\000\107\002\032\135"
  "\003\000\000\000\344\000\000\000\006\000\114\000\354\000\000\000"
  "\360\000\000\000\213\346\223\166\002\000\000\000\360\000\000\000"
  "\010\000\114\000\370\000\000\000\000\001\000\000\366\325\124\153"
  "\000\000\000\000\000\001\000\000\014\000\114\000\014\001\000\000"
  "\020\001\000\000\173\242\170\174\004\000\000\000\020\001\000\000"
  "\003\000\114\000\024\001\000\000\030\001\000\000\324\265\002\000"
  "\377\377\377\377\030\001\000\000\001\000\114\000\034\001\000\000"
  "\040\001\000\000\173\176\254\041\001\000\000\000\040\001\000\000"
  "\015\000\166\000\060\001\000\000\303\001\000\000\131\253\135\230"
  "\001\000\000\000\303\001\000\000\015\000\166\000\320\001\000\000"
  "\100\002\000\000\163\061\155\067\165\057\000\000\002\000\000\000"
  "\147\154\146\151\154\145\163\057\006\000\000\000\005\000\000\000"
  "\143\160\154\170\147\162\141\160\150\145\162\057\001\000\000\000"
  "\151\157\057\000\000\000\000\000\057\000\000\000\003\000\000\000"
  "\166\145\162\164\123\150\141\144\056\166\145\162\164\000\000\000"
  "\203\000\000\000\000\000\000\000\043\166\145\162\163\151\157\156"
  "\040\061\065\060\012\012\151\156\040\166\145\143\064\040\160\157"
  "\163\151\164\151\157\156\073\012\151\156\040\166\145\143\064\040"
  "\143\157\154\157\162\073\012\157\165\164\040\166\145\143\064\040"
  "\166\145\162\164\137\143\157\154\157\162\073\012\012\166\157\151"
  "\144\040\155\141\151\156\050\051\040\173\012\011\147\154\137\120"
  "\157\163\151\164\151\157\156\040\075\040\160\157\163\151\164\151"
  "\157\156\073\012\011\166\145\162\164\137\143\157\154\157\162\040"
  "\075\040\143\157\154\157\162\073\012\175\012\000\000\050\165\165"
  "\141\171\051\146\162\141\147\123\150\141\144\056\146\162\141\147"
  "\140\000\000\000\000\000\000\000\043\166\145\162\163\151\157\156"
  "\040\061\065\060\012\012\151\156\040\166\145\143\064\040\166\145"
  "\162\164\137\143\157\154\157\162\073\012\157\165\164\040\166\145"
  "\143\064\040\157\165\164\137\143\157\154\157\162\073\012\012\166"
  "\157\151\144\040\155\141\151\156\050\051\040\173\012\011\157\165"
  "\164\137\143\157\154\157\162\040\075\040\166\145\162\164\137\143"
  "\157\154\157\162\073\012\175\012\000\000\050\165\165\141\171\051"
  "" };
#endif /* !_MSC_VER */

static GStaticResource static_resource = { glib_resources_resource_data.data, sizeof (glib_resources_resource_data.data) - 1 /* nul terminator */, NULL, NULL, NULL };
extern GResource *glib_resources_get_resource (void);
GResource *glib_resources_get_resource (void)
{
  return g_static_resource_get_resource (&static_resource);
}
/*
  If G_HAS_CONSTRUCTORS is true then the compiler support *both* constructors and
  destructors, in a sane way, including e.g. on library unload. If not you're on
  your own.

  Some compilers need #pragma to handle this, which does not work with macros,
  so the way you need to use this is (for constructors):

  #ifdef G_DEFINE_CONSTRUCTOR_NEEDS_PRAGMA
  #pragma G_DEFINE_CONSTRUCTOR_PRAGMA_ARGS(my_constructor)
  #endif
  G_DEFINE_CONSTRUCTOR(my_constructor)
  static void my_constructor(void) {
   ...
  }

*/

#ifndef __GTK_DOC_IGNORE__

#if  __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 7)

#define G_HAS_CONSTRUCTORS 1

#define G_DEFINE_CONSTRUCTOR(_func) static void __attribute__((constructor)) _func (void);
#define G_DEFINE_DESTRUCTOR(_func) static void __attribute__((destructor)) _func (void);

#elif defined (_MSC_VER) && (_MSC_VER >= 1500)
/* Visual studio 2008 and later has _Pragma */

#include <stdlib.h>

#define G_HAS_CONSTRUCTORS 1

/* We do some weird things to avoid the constructors being optimized
 * away on VS2015 if WholeProgramOptimization is enabled. First we
 * make a reference to the array from the wrapper to make sure its
 * references. Then we use a pragma to make sure the wrapper function
 * symbol is always included at the link stage. Also, the symbols
 * need to be extern (but not dllexport), even though they are not
 * really used from another object file.
 */

/* We need to account for differences between the mangling of symbols
 * for Win32 (x86) and x64 programs, as symbols on Win32 are prefixed
 * with an underscore but symbols on x64 are not.
 */
#ifdef _WIN64
#define G_MSVC_SYMBOL_PREFIX ""
#else
#define G_MSVC_SYMBOL_PREFIX "_"
#endif

#define G_DEFINE_CONSTRUCTOR(_func) G_MSVC_CTOR (_func, G_MSVC_SYMBOL_PREFIX)
#define G_DEFINE_DESTRUCTOR(_func) G_MSVC_DTOR (_func, G_MSVC_SYMBOL_PREFIX)

#define G_MSVC_CTOR(_func,_sym_prefix) \
  static void _func(void); \
  extern int (* _array ## _func)(void);              \
  int _func ## _wrapper(void) { _func(); g_slist_find (NULL,  _array ## _func); return 0; } \
  __pragma(comment(linker,"/include:" _sym_prefix # _func "_wrapper")) \
  __pragma(section(".CRT$XCU",read)) \
  __declspec(allocate(".CRT$XCU")) int (* _array ## _func)(void) = _func ## _wrapper;

#define G_MSVC_DTOR(_func,_sym_prefix) \
  static void _func(void); \
  extern int (* _array ## _func)(void);              \
  int _func ## _constructor(void) { atexit (_func); g_slist_find (NULL,  _array ## _func); return 0; } \
   __pragma(comment(linker,"/include:" _sym_prefix # _func "_constructor")) \
  __pragma(section(".CRT$XCU",read)) \
  __declspec(allocate(".CRT$XCU")) int (* _array ## _func)(void) = _func ## _constructor;

#elif defined (_MSC_VER)

#define G_HAS_CONSTRUCTORS 1

/* Pre Visual studio 2008 must use #pragma section */
#define G_DEFINE_CONSTRUCTOR_NEEDS_PRAGMA 1
#define G_DEFINE_DESTRUCTOR_NEEDS_PRAGMA 1

#define G_DEFINE_CONSTRUCTOR_PRAGMA_ARGS(_func) \
  section(".CRT$XCU",read)
#define G_DEFINE_CONSTRUCTOR(_func) \
  static void _func(void); \
  static int _func ## _wrapper(void) { _func(); return 0; } \
  __declspec(allocate(".CRT$XCU")) static int (*p)(void) = _func ## _wrapper;

#define G_DEFINE_DESTRUCTOR_PRAGMA_ARGS(_func) \
  section(".CRT$XCU",read)
#define G_DEFINE_DESTRUCTOR(_func) \
  static void _func(void); \
  static int _func ## _constructor(void) { atexit (_func); return 0; } \
  __declspec(allocate(".CRT$XCU")) static int (* _array ## _func)(void) = _func ## _constructor;

#elif defined(__SUNPRO_C)

/* This is not tested, but i believe it should work, based on:
 * http://opensource.apple.com/source/OpenSSL098/OpenSSL098-35/src/fips/fips_premain.c
 */

#define G_HAS_CONSTRUCTORS 1

#define G_DEFINE_CONSTRUCTOR_NEEDS_PRAGMA 1
#define G_DEFINE_DESTRUCTOR_NEEDS_PRAGMA 1

#define G_DEFINE_CONSTRUCTOR_PRAGMA_ARGS(_func) \
  init(_func)
#define G_DEFINE_CONSTRUCTOR(_func) \
  static void _func(void);

#define G_DEFINE_DESTRUCTOR_PRAGMA_ARGS(_func) \
  fini(_func)
#define G_DEFINE_DESTRUCTOR(_func) \
  static void _func(void);

#else

/* constructors not supported for this compiler */

#endif

#endif /* __GTK_DOC_IGNORE__ */

#ifdef G_HAS_CONSTRUCTORS

#ifdef G_DEFINE_CONSTRUCTOR_NEEDS_PRAGMA
#pragma G_DEFINE_CONSTRUCTOR_PRAGMA_ARGS(resource_constructor)
#endif
G_DEFINE_CONSTRUCTOR(resource_constructor)
#ifdef G_DEFINE_DESTRUCTOR_NEEDS_PRAGMA
#pragma G_DEFINE_DESTRUCTOR_PRAGMA_ARGS(resource_destructor)
#endif
G_DEFINE_DESTRUCTOR(resource_destructor)

#else
#warning "Constructor not supported on this compiler, linking in resources will not work"
#endif

static void resource_constructor (void)
{
  g_static_resource_init (&static_resource);
}

static void resource_destructor (void)
{
  g_static_resource_fini (&static_resource);
}
