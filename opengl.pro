QT += core gui widgets opengl
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    glwidget.cpp \
    gltexturewidget.cpp

HEADERS  += \
    mainwindow.h \
    glwidget.h \
    gltexturewidget.h

DISTFILES += \
    shader.f.glsl \
    shader.v.glsl

RESOURCES += \
    sketchapp.qrc

FORMS += \
    mainwindow.ui
