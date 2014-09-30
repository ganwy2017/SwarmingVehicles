#ifndef FLIGHTMODE_HANDLER_GLOBAL_H
#define FLIGHTMODE_HANDLER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(FLIGHTMODE_HANDLER_LIBRARY)
#  define FLIGHTMODE_HANDLERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define FLIGHTMODE_HANDLERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // FLIGHTMODE_HANDLER_GLOBAL_H