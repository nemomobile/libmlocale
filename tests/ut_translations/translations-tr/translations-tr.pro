QMAKE_EXTRA_TARGETS += check
check.depends = .
check.commands = echo "nothing to do here for “make check”"

QMAKE_EXTRA_TARGETS += check-xml
check-xml.depends = .
check-xml.commands = echo "nothing to do here for “make check-xml”"

LANGUAGES = en_GB fi_FI
DISABLE_QTTRID_ENGINEERING_ENGLISH = yes
CATALOGNAME = ut_translations-tr
SOURCEDIR = $$PWD/..
TRANSLATIONDIR = $$PWD
equals(QT_MAJOR_VERSION, 4): TRANSLATION_INSTALLDIR = $$[QT_INSTALL_LIBS]/libmlocale-tests/translations-tr
equals(QT_MAJOR_VERSION, 5): TRANSLATION_INSTALLDIR = $$[QT_INSTALL_LIBS]/libmlocale-tests5/translations-tr
# add “-verbose” to force without “-idbased” which is the default in translations.pri
LRELEASE_OPTIONS = "-verbose"

# these include files are installed to $$[QT_INSTALL_DATA]/mkspecs/features
# and included in the "libmlocale-dev" package:
include($${M_BUILD_TREE}/mkspecs/features/mlocale_defines.prf)
include($${M_SOURCE_TREE}/mkspecs/features/mlocale_translations.prf)
