include(../common_top.pri)

TARGET = ut_duilistviewseparators

INCLUDEPATH += ../ut_duilistviewcommon \
    ../duilistviewcommon
HEADERS += ut_duilistviewseparators.h \
        ../duilistviewcommon/myindexedmodel.h

SOURCES += ut_duilistviewseparators.cpp  \
        ../duilistviewcommon/myindexedmodel.cpp \
        ../../src/views/duilistview_p.cpp \
        ../../src/views/.moc/moc_duilistview_p.cpp

include(../common_bot.pri)
