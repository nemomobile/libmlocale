include(../common_top.pri)
include(../common_duisettings.pri)

# unit test and unit classes
SOURCES += \
    ut_duisettingslanguagewidgetfactory.cpp \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagewidgetfactory.cpp \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagenode.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit classes
HEADERS += \
    ut_duisettingslanguagewidgetfactory.h \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagewidgetfactory.h \
    $$DUISRCDIR/settings/settingslanguage/duisettingslanguagenode.h

include(../common_bot.pri)
