
#
# Detect itk::ObjectFactoryBase::SetAutoloadPath() (ITK PR #6985).
#
# When available, qSlicerCoreApplication registers Slicer's ITK factory plugin
# directory in-process and the launcher no longer exports ITK_AUTOLOAD_PATH, so
# another ITK build in the same process (e.g. a pip installed itk package in the
# embedded Python) never scans Slicer's plugins.
#
# Sets the cache variable Slicer_ITK_HAS_OBJECTFACTORY_AUTOLOAD_PATH.
#

function(_slicer_check_itk_objectfactory_autoload_path)
  find_package(ITK COMPONENTS ITKCommon REQUIRED)
  include(CheckCXXSourceCompiles)
  set(CMAKE_REQUIRED_INCLUDES ${ITK_INCLUDE_DIRS})
  # decltype keeps the probe unevaluated, so no ITK library is needed to link.
  check_cxx_source_compiles("
#include <itkObjectFactoryBase.h>
#include <string>
int main()
{
  using SetType = decltype(itk::ObjectFactoryBase::SetAutoloadPath(std::string()));
  using GetType = decltype(itk::ObjectFactoryBase::GetAutoloadPath());
  static_assert(sizeof(GetType) > 0);
  return 0;
}
" Slicer_ITK_HAS_OBJECTFACTORY_AUTOLOAD_PATH)
endfunction()
_slicer_check_itk_objectfactory_autoload_path()
