include_directories(
  ${QT_INCLUDES}
  ${KDE4_INCLUDES}
  ${NEPOMUK_INCLUDE_DIR}
  ${NEPOMUK_INCLUDE_DIR}/nepomuk
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/src/
)

kde4_add_executable(propertycompletertest propertycompletertest.cpp)
target_link_libraries(propertycompletertest
  nepomukextras
  ${NEPOMUK_LIBRARIES}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTDBUS_LIBRARY}
  ${KDE4_KDECORE_LIBS}
  ${KDE4_KDEUI_LIBS}
)
