set(Qt6_DIR "C:/Dev/Qt/6.8.0/mingw_64/lib/cmake/Qt6")
find_package(Qt6 REQUIRED COMPONENTS Core Gui UiTools Widgets)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

function(target_link_qt_libraries Target)
  target_link_libraries(${Target} PUBLIC Qt::Core Qt::Gui Qt::UiTools
                                         Qt::Widgets)
endfunction()

function(install_qt_executable_with_dependencies Targets)
  install(TARGETS ${Targets})
  qt_generate_deploy_app_script(TARGET Main OUTPUT_SCRIPT qt_deploy_script
                                NO_UNSUPPORTED_PLATFORM_ERROR)
  install(SCRIPT ${qt_deploy_script})
endfunction()
