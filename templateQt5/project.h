// ******************************************************
// * copyright 2009-2017: (C) by Reinhardt Behm rbehm@hushmail.com
// * All Rights reserved
// ******************************************************

#ifndef PROJECT_H
#define PROJECT_H

#include <unistd.h>
#include <math.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <iostream>

using namespace std;

#ifdef QT_CORE_LIB
#include <QtCore>
#endif

#ifdef QT_GUI_LIB
#include <QtGui>
#endif

#ifdef QT_WIDGETS_LIB
#include <QtWidgets>
#endif

#ifdef QT_NETWORK_LIB
#include <QtNetwork>
#endif

#ifdef QT_SERIALPORT_LIB
#include <QtSerialPort>
#endif

#ifdef QT_OPENGL_LIB
#include <QtOpenGL>
#endif

#ifdef QT_XML_LIB
#include <QtXml>
#endif

#ifdef QT_SQL_LIB
#include <QtSql>
#endif

#ifdef QT_DBUS_LIB
#include <QtDBus>
#endif

#ifdef QT_QT3SUPPORT_LIB
#include <Qt3Support>
#endif

#ifdef QT_MULTIMEDIAWIDGETS_LIB
#include <QtMultimediaWidgets>
#endif

#ifdef QT_PRINTSUPPORT_LIB
#include <QtPrintSupport>
#endif

#ifdef QT_POSITIONING_LIB
#include <QtPositioning>
#endif

#ifdef QT_SENSORS_LIB
#include <QtXmlPatterns>
#endif

#ifdef QT_WEBKIT_LIB
#include <QtWebKit>
#endif

#ifdef QT_XMLPATTERNS_LIB
#include <QtXmlPatterns>
#endif

#ifdef QT_SVG_LIB
#include <QtSvg>
#endif

#ifdef QT_WEBSOCKETS_LIB
#include <QtWebSockets>
#endif

#ifdef QT_MULTIMEDIA_LIB
#include <QtMultimedia>
#endif

#ifdef QT_X11EXTRAS_LIB
#include <QtX11Extras>
#endif

#ifdef QT_UITOOLS_LIB
#include <QtUiTools>
#endif

#ifdef QT_BLUETOOTH_LIB
#include <QtBluetooth>
#endif

#ifdef QCUSTOM_PLOT
#include "qcustomplot.h"
#endif


#define dimof(x) (sizeof(x)/sizeof(x[0]))

#endif

