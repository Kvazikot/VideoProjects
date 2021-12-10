#ifndef APP6DYN_GLOBAL_H
#define APP6DYN_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(APP6DYN_LIBRARY)
#  define APP6DYNSHARED_EXPORT Q_DECL_EXPORT
#else
#  define APP6DYNSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // APP6DYN_GLOBAL_H
