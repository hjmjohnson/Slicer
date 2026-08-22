
set(Slicer_PYTHON_SCRIPTS
  slicer/__init__
  slicer/cli
  slicer/i18n
  slicer/logic
  slicer/parameterNodeWrapper/__init__
  slicer/parameterNodeWrapper/default
  slicer/parameterNodeWrapper/guiCreation
  slicer/parameterNodeWrapper/guiConnectors
  slicer/parameterNodeWrapper/parameterInfo
  slicer/parameterNodeWrapper/parameterPack
  slicer/parameterNodeWrapper/serializers
  slicer/parameterNodeWrapper/types
  slicer/parameterNodeWrapper/util
  slicer/parameterNodeWrapper/validators
  slicer/parameterNodeWrapper/wrapper
  slicer/packaging
  slicer/ScriptedLoadableModule
  slicer/testing
  slicer/util
  slicerqt
  mrml
  vtkAddon
  vtkITK
  vtkSegmentationCore
  vtkTeem
  )

set(Slicer_PYTHON_MODULES_CONFIG "
# slicer libs
'mrml',
'vtkAddon',
'vtkSegmentationCore',
'qMRMLWidgetsPythonQt',
# slicer base libs
'logic',
'qSlicerBaseQTCorePythonQt',
'qSlicerBaseQTCLIPythonQt',
'qSlicerBaseQTGUIPythonQt',
'qSlicerBaseQTAppPythonQt'"
  )

configure_file(
  slicer/kits.py.in
  ${CMAKE_CURRENT_BINARY_DIR}/slicer/kits.py
  @ONLY
  )

list(APPEND Slicer_PYTHON_SCRIPTS ${CMAKE_CURRENT_BINARY_DIR}/slicer/kits.py)

set(Slicer_PYTHON_RESOURCES
  )

if(Slicer_USE_SimpleITK)
  list(APPEND Slicer_PYTHON_SCRIPTS
    sitkUtils
    )
endif()

ctkMacroCompilePythonScript(
  TARGET_NAME SlicerBase
  SCRIPTS "${Slicer_PYTHON_SCRIPTS}"
  RESOURCES "${Slicer_PYTHON_RESOURCES}"
  DESTINATION_DIR ${Slicer_BINARY_DIR}/bin/Python
  INSTALL_DIR ${Slicer_INSTALL_BIN_DIR}
  )

# Generate a lazily-loading replacement for the eager "vtk" module distributed
# with VTK. It is placed in ${Slicer_BINARY_DIR}/bin/Python, which precedes the
# VTK site-packages directory on the Python path, so "import vtk" resolves to
# the generated module and each attribute access imports only the vtkmodules
# submodule that provides it. This saves several hundred milliseconds at every
# application startup. See Utilities/Scripts/GenerateLazyVtkModule.py.
set(_lazy_vtk_module ${Slicer_BINARY_DIR}/bin/Python/vtk.py)
# The VTK Python package location is passed explicitly: during a fresh
# superbuild the launcher settings of ${PYTHON_EXECUTABLE} may not
# reference the VTK build tree yet when this rule runs.
set(_lazy_vtk_pythonpath "")
if(DEFINED VTK_PYTHONPATH_LAUNCHER_BUILD)
  set(_lazy_vtk_pythonpath ${VTK_PYTHONPATH_LAUNCHER_BUILD})
endif()
# On Windows, Python >= 3.8 resolves an extension module's dependent DLLs only
# from directories registered with os.add_dll_directory(); PATH is ignored.
# VTK's vtkmodules/__init__.py registers VTK's own bin/ directory, but not the
# directories of the libraries VTK links against, so "import vtkmodules.all"
# fails on vtkCommonCore (vtkCommon -> tbb12.dll) and then on vtkRenderingQt
# (-> Qt6). Name those directories explicitly for the generator script.
set(_lazy_vtk_dll_directories "")
if(WIN32)
  foreach(_lazy_vtk_dll_dir IN LISTS tbb_LIBRARY_PATHS_LAUNCHER_BUILD)
    list(APPEND _lazy_vtk_dll_directories --dll-directory ${_lazy_vtk_dll_dir})
  endforeach()
  foreach(_lazy_vtk_qt_core IN ITEMS Qt6::Core Qt5::Core)
    if(TARGET ${_lazy_vtk_qt_core})
      list(APPEND _lazy_vtk_dll_directories
        --dll-directory $<TARGET_FILE_DIR:${_lazy_vtk_qt_core}>)
      break()
    endif()
  endforeach()
endif()
add_custom_command(
  OUTPUT ${_lazy_vtk_module}
  COMMAND ${CMAKE_COMMAND} -E env "PYTHONPATH=${_lazy_vtk_pythonpath}"
    ${PYTHON_EXECUTABLE}
    ${Slicer_SOURCE_DIR}/Utilities/Scripts/GenerateLazyVtkModule.py
    --output ${_lazy_vtk_module}
    ${_lazy_vtk_dll_directories}
  DEPENDS ${Slicer_SOURCE_DIR}/Utilities/Scripts/GenerateLazyVtkModule.py
  COMMENT "Generating lazily-loading vtk module"
  VERBATIM
  )
add_custom_target(SlicerGenerateLazyVtkModule ALL
  DEPENDS ${_lazy_vtk_module}
  )
install(FILES ${_lazy_vtk_module}
  DESTINATION ${Slicer_INSTALL_BIN_DIR}/Python
  COMPONENT Runtime
  )
