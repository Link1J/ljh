#include <version>

#if _MSC_VER >= 1932 && !defined(__cpp_explicit_this_parameter)
#define __cpp_explicit_this_parameter 202110L
#endif

static_assert(__cpp_explicit_this_parameter >= 202110L);

int main()
{}