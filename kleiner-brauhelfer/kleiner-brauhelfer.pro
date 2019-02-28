QT += core sql gui widgets svg webenginewidgets

ORGANIZATION = kleiner-brauhelfer
TARGET = kleiner-brauhelfer

VER_MAJ = 2
VER_MIN = 0
VER_PAT = 0
VERSION = $$sprintf("%1.%2.%3", $$VER_MAJ, $$VER_MIN, $$VER_PAT)
DEFINES += VER_MAJ=\"$$VER_MAJ\" VER_MIN=\"$$VER_MIN\" VER_PAT=\"$$VER_PAT\"

TEMPLATE = app
DEFINES += ORGANIZATION=\\\"$$ORGANIZATION\\\" TARGET=\\\"$$TARGET\\\" VERSION=\\\"$$VERSION\\\"

CONFIG += c++11
DEFINES += QT_DEPRECATED_WARNINGS
CONFIG += warn_on

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../kleiner-brauhelfer-core/release/ -lkleiner-brauhelfer-core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../kleiner-brauhelfer-core/debug/ -lkleiner-brauhelfer-core
else:unix: LIBS += -L$$OUT_PWD/../kleiner-brauhelfer-core/ -lkleiner-brauhelfer-core

INCLUDEPATH += $$PWD/../kleiner-brauhelfer-core
DEPENDPATH += $$PWD/../kleiner-brauhelfer-core

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    settings.cpp \
    tababfuellen.cpp \
    tabausruestung.cpp \
    tabbewertung.cpp \
    tabbraudaten.cpp \
    tabbrauuebersicht.cpp \
    tabdatenbank.cpp \
    tabetikette.cpp \
    tabgaerverlauf.cpp \
    tabrezept.cpp \
    tabrohstoffe.cpp \
    tabsudauswahl.cpp \
    tabsudauswahl_sudinfo.cpp \
    tabzusammenfassung.cpp \
    dialogs/dlgabout.cpp \
    dialogs/dlgeinmaischtemp.cpp \
    dialogs/dlgrestextrakt.cpp \
    dialogs/dlgrohstoffaustausch.cpp \
    dialogs/dlgrohstoffvorlage.cpp \
    dialogs/dlgsudteilen.cpp \
    dialogs/dlgverdampfung.cpp \
    dialogs/dlgvolumen.cpp \
    helper/htmlhighlighter.cpp \
    helper/mustache.cpp \
    model/checkboxdelegate.cpp \
    model/comboboxdelegate.cpp \
    model/datedelegate.cpp \
    model/datetimedelegate.cpp \
    model/doublespinboxdelegate.cpp \
    model/dsvtablemodel.cpp \
    model/ebcdelegate.cpp \
    model/ingredientnamedelegate.cpp \
    model/linklabeldelegate.cpp \
    model/proxymodelsudcolored.cpp \
    model/ratingdelegate.cpp \
    model/readonlydelegate.cpp \
    model/rohstoffauswahlproxymodel.cpp \
    model/spinboxdelegate.cpp \
    widgets/combobox.cpp \
    widgets/dateedit.cpp \
    widgets/datetimeedit.cpp \
    widgets/doublespinbox.cpp \
    widgets/spinbox.cpp \
    widgets/svgview.cpp \
    widgets/wdganhang.cpp \
    widgets/wdgbewertung.cpp \
    widgets/wdgdiagramview.cpp \
    widgets/wdghefegabe.cpp \
    widgets/wdghopfengabe.cpp \
    widgets/wdgmalzgabe.cpp \
    widgets/wdgrast.cpp \
    widgets/wdgrating.cpp \
    widgets/wdgweiterezutatgabe.cpp \
    widgets/webview.cpp

HEADERS += \
    definitionen.h \
    mainwindow.h \
    settings.h \
    tababfuellen.h \
    tabausruestung.h \
    tabbewertung.h \
    tabbraudaten.h \
    tabbrauuebersicht.h \
    tabdatenbank.h \
    tabetikette.h \
    tabgaerverlauf.h \
    tabrezept.h \
    tabrohstoffe.h \
    tabsudauswahl.h \
    tabzusammenfassung.h \
    dialogs/dlgabout.h \
    dialogs/dlgeinmaischtemp.h \
    dialogs/dlgrestextrakt.h \
    dialogs/dlgrohstoffaustausch.h \
    dialogs/dlgrohstoffvorlage.h \
    dialogs/dlgsudteilen.h \
    dialogs/dlgverdampfung.h \
    dialogs/dlgvolumen.h \
    helper/htmlhighlighter.h \
    helper/mustache.h \
    model/checkboxdelegate.h \
    model/comboboxdelegate.h \
    model/datedelegate.h \
    model/datetimedelegate.h \
    model/doublespinboxdelegate.h \
    model/dsvtablemodel.h \
    model/ebcdelegate.h \
    model/ingredientnamedelegate.h \
    model/linklabeldelegate.h \
    model/proxymodelsudcolored.h \
    model/ratingdelegate.h \
    model/readonlydelegate.h \
    model/rohstoffauswahlproxymodel.h \
    model/spinboxdelegate.h \
    widgets/combobox.h \
    widgets/dateedit.h \
    widgets/datetimeedit.h \
    widgets/doublespinbox.h \
    widgets/spinbox.h \
    widgets/svgview.h \
    widgets/wdganhang.h \
    widgets/wdgbewertung.h \
    widgets/wdgdiagramview.h \
    widgets/wdghefegabe.h \
    widgets/wdghopfengabe.h \
    widgets/wdgmalzgabe.h \
    widgets/wdgrast.h \
    widgets/wdgrating.h \
    widgets/wdgweiterezutatgabe.h \
    widgets/webview.h

FORMS += \
    mainwindow.ui \
    tababfuellen.ui \
    tabausruestung.ui \
    tabbewertung.ui \
    tabbraudaten.ui \
    tabbrauuebersicht.ui \
    tabdatenbank.ui \
    tabetikette.ui \
    tabgaerverlauf.ui \
    tabrezept.ui \
    tabrohstoffe.ui \
    tabsudauswahl.ui \
    tabzusammenfassung.ui \
    dialogs/dlgabout.ui \
    dialogs/dlgeinmaischtemp.ui \
    dialogs/dlgrestextrakt.ui \
    dialogs/dlgrohstoffaustausch.ui \
    dialogs/dlgrohstoffvorlage.ui \
    dialogs/dlgsudteilen.ui \
    dialogs/dlgverdampfung.ui \
    dialogs/dlgvolumen.ui \
    widgets/wdganhang.ui \
    widgets/wdgbewertung.ui \
    widgets/wdghefegabe.ui \
    widgets/wdghopfengabe.ui \
    widgets/wdgmalzgabe.ui \
    widgets/wdgrast.ui \
    widgets/wdgrating.ui \
    widgets/wdgweiterezutatgabe.ui

RESOURCES += \
    data.qrc \
    images.qrc