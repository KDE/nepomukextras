project(nepomukextras)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${nepomuk_kde_SOURCE_DIR}/cmake)

# set the version to be used by NepomukExtrasConfig.cmake and the lib version
# ===============================================================================================
set(CMAKE_NEPOMUKEXTRAS_VERSION_MAJOR 0)
set(CMAKE_NEPOMUKEXTRAS_VERSION_MINOR 2)
set(CMAKE_NEPOMUKEXTRAS_VERSION_RELEASE 0)
set(CMAKE_NEPOMUKEXTRAS_VERSION "${CMAKE_NEPOMUKEXTRAS_VERSION_MAJOR}.${CMAKE_NEPOMUKEXTRAS_VERSION_MINOR}.${CMAKE_NEPOMUKEXTRAS_VERSION_RELEASE}")


# generate NepomukExtrasConfig.cmake for easy utilisation of the package by other cmake build systems
# ===============================================================================================
configure_file(NepomukExtrasConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/NepomukExtrasConfig.cmake @ONLY)
configure_file(NepomukExtrasConfigVersion.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/NepomukExtrasConfigVersion.cmake @ONLY)
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/NepomukExtrasConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/NepomukExtrasConfigVersion.cmake
  DESTINATION ${CMAKE_INSTALL_PREFIX}/share/cmake/NepomukExtras)


# search required packages
# ===============================================================================================
set(KDE_MIN_VERSION "4.5.00")
find_package(KDE4 4.5.00 REQUIRED)
include(KDE4Defaults)
find_package(KDE4Workspace)
include(MacroLibrary)

find_package(SharedDesktopOntologies 0.5)
set(SOPRANO_MIN_VERSION "2.5.0")
find_package(Soprano)
find_package(Nepomuk)

find_package(Strigi)

find_package(Kolena)

macro_log_feature(SOPRANO_FOUND "Soprano" "Support for the Nepomuk semantic desktop system" "http://soprano.sourceforge.net" TRUE "${SOPRANO_MIN_VERSION}" "")
macro_log_feature(SHAREDDESKTOPONTOLOGIES_FOUND "Shared desktop ontologies" "Desktop ontologies" "http://oscaf.sourceforge.net" TRUE ${SDO_MIN_VERSION} "Ontologies necessary for the Nepomuk semantic desktop.")
macro_log_feature(Nepomuk_FOUND "Nepomuk" "Nepomuk Libraries" "kdelibs" TRUE "" "The basis for this package.")
macro_log_feature(Strigi_FOUND "libstreamanalyzer" "Strigi file analyzer libs (streamanalyzer)" "libstreamanalyzer" TRUE "0.6.95" "Required to extract text from files.")
macro_log_feature(Kolena_FOUND "Kolena" "The Kolena image text extraction" "kolena" FALSE "" "Required to extract text from images.")


# build stuff
# ===============================================================================================
add_subdirectory(src)
add_subdirectory(test)

macro_display_feature_log()
