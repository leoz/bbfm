TEMPLATE = lib
TARGET = bbfm

CONFIG += qt warn_on debug_and_release cascades

INCLUDEPATH += ../src
INCLUDEPATH += ../../ImageMagick ../../ImageMagick/Magick++/lib
SOURCES += ../src/*.cpp
HEADERS += ../src/*.hpp ../src/*.h
RESOURCES += bbfm.qrc
DEPENDPATH += assets

lupdate_inclusion {
    SOURCES += ../assets/*.qml
}

IM_FLAGS = -DMAGICKCORE_HDRI_ENABLE=0 -DMAGICKCORE_QUANTUM_DEPTH=16

device {
    CONFIG(release, debug|release) {
        DESTDIR = o.le-v7
        TEMPLATE = lib
        QMAKE_CXXFLAGS_RELEASE += -fvisibility=hidden -mthumb $${IM_FLAGS}
    }
    CONFIG(debug, debug|release) {
        DESTDIR = o.le-v7-g
		QMAKE_CXXFLAGS_DEBUG += $${IM_FLAGS}
    }
}

simulator {
    CONFIG(release, debug|release) {
        DESTDIR = o
        QMAKE_CXXFLAGS_RELEASE += $${IM_FLAGS}
    }
    CONFIG(debug, debug|release) {
        DESTDIR = o-g
		QMAKE_CXXFLAGS_DEBUG += $${IM_FLAGS}
    }
}

OBJECTS_DIR = $${DESTDIR}/.obj
MOC_DIR = $${DESTDIR}/.moc
RCC_DIR = $${DESTDIR}/.rcc
UI_DIR = $${DESTDIR}/.ui

suredelete.target = sureclean
suredelete.commands = $(DEL_FILE) $${MOC_DIR}/*; $(DEL_FILE) $${RCC_DIR}/*; $(DEL_FILE) $${UI_DIR}/*
suredelete.depends = distclean

QMAKE_EXTRA_TARGETS += suredelete

