project(libnepomukextras)

include_directories(
  ${QT_INCLUDES}
  ${KDE4_INCLUDES}
  ${NEPOMUK_INCLUDE_DIR}
  ${NEPOMUK_INCLUDE_DIR}/nepomuk
  ${CMAKE_SOURCE_DIR}
  ${STRIGI_INCLUDE_DIR}
  ${KOLENA_INCLUDE_DIR}
)

configure_file(nepomukextras_version.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/nepomukextras_version.h)

set(nepomukextras_SRC
  kcompleter.cpp
  kcompletionitem.cpp
  kcompletionmodel.cpp
  kcompletiondelegate.cpp
  typecompletion.cpp
  resourcecompletion.cpp
  propertycompleter.cpp
  pimomodel.cpp
  daterangeselectionwidget.cpp
  daterange.cpp
  newresourcedialog.cpp
  resourceselectiondialog.cpp
  searchmenu.cpp
  nepomukmenu.cpp
  resourceactions.cpp
  statementeditor.cpp
  classmodel.cpp
  textextractionjob.cpp
  strigitextextractionjob.cpp
  nepomukextras_version.cpp
)

if(Kolena_FOUND)
  add_definitions(-DHAVE_KOLENA)
endif()

configure_file(nepomukextras_config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/nepomukextras_config.h)

kde4_add_ui_files(nepomukextras_SRC daterangeselectionwidget.ui newresourcedialog.ui)

kde4_add_library(nepomukextras SHARED ${nepomukextras_SRC})

target_link_libraries(
  nepomukextras
  ${QT_QTGUI_LIBRARY}
  ${KDE4_KDEUI_LIBS}
  ${KDE4_KIO_LIBS}
  ${SOPRANO_LIBRARIES}
  ${NEPOMUK_LIBRARIES}
  ${NEPOMUK_QUERY_LIBRARIES}
  ${STRIGI_STREAMANALYZER_LIBRARY}
  ${STRIGI_STREAMS_LIBRARY}
  nepomukutils
)

if(Kolena_FOUND)
  target_link_libraries(nepomukextras kolena)
endif()

set_target_properties(nepomukextras PROPERTIES VERSION ${CMAKE_NEPOMUKEXTRAS_VERSION} SOVERSION ${CMAKE_NEPOMUKEXTRAS_VERSION_MAJOR})

install(TARGETS nepomukextras DESTINATION ${LIB_INSTALL_DIR})
install(FILES
  kcompleter.h
  kcompletionitem.h
  typecompletion.h
  resourcecompletion.h
  propertycompleter.h
  nepomukextras_export.h
  pimomodel.h
  daterange.h
  daterangeselectionwidget.h
  newresourcedialog.h
  resourceselectiondialog.h
  searchmenu.h
  nepomukmenu.h
  resourceactions.h
  newresourcedialog.h
  statementeditor.h
  classmodel.h
  textextractionjob.h
  ${CMAKE_CURRENT_BINARY_DIR}/nepomukextras_version.h
  DESTINATION ${INCLUDE_INSTALL_DIR}/nepomuk)
