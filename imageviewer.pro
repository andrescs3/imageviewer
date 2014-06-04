QT += widgets
qtHaveModule(printsupport): QT += printsupport

HEADERS       = imageviewer.h \
    thinning.h \
    lineReconstruction.h \
    commonFilters.h \
    limpieza/limpieza.h \
    cuerpos/Cuerpo.h \
    cuerpos/cuerpos.h
SOURCES       = imageviewer.cpp \
                main.cpp \
    thinning.cpp \
    lineReconstruction.cpp \
    commonFilters.cpp \
    limpieza/limpieza.cpp \
    cuerpos/Cuerpo.cpp \
    cuerpos/cuerpos.cpp \
    lineReconstruction - copia.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/widgets/imageviewer
INSTALLS += target


wince*: {
   DEPLOYMENT_PLUGIN += qjpeg qgif
}


win32 {
    message("* Using settings for Windows.")

    INCLUDEPATH += "C:\\opencv\\build\\include" \
                   "C:\\opencv\\build\\include\\opencv" \
                   "C:\\opencv\\build\\include\\opencv2"

    LIBS += -L"C:\\opencv\\release\\lib" \
        -lopencv_core249 \
        -lopencv_highgui249 \
        -lopencv_imgproc249
}
