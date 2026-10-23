#ifndef itkMRMLIDIOPlugin_h
#define itkMRMLIDIOPlugin_h

#include "itkObjectFactoryBase.h"

#ifdef _WIN32
# ifdef MRMLIDIOPlugin_EXPORTS
#  define MRMLIDIOPlugin_EXPORT __declspec(dllexport)
# else
#  define MRMLIDIOPlugin_EXPORT __declspec(dllimport)
# endif
#else
# define MRMLIDIOPlugin_EXPORT
#endif

// itkConfigure.h defines ITK_LOAD_FUNCTION_NAME from ITK 5.4.8 and 6.0.0 on.
#if ITK_VERSION_MAJOR > 5 || (ITK_VERSION_MAJOR == 5 && ITK_VERSION_MINOR > 4) || (ITK_VERSION_MAJOR == 5 && ITK_VERSION_MINOR == 4 && ITK_VERSION_PATCH >= 8)
# ifndef ITK_LOAD_FUNCTION_NAME
#  error "ITK_LOAD_FUNCTION_NAME is expected from itkConfigure.h"
# endif
#elif !defined(ITK_LOAD_FUNCTION_NAME)
// release-5.4 commits after v5.4.7 carry the macro while still reporting 5.4.7.
# define ITK_LOAD_FUNCTION_NAME itkLoad
#endif

/**
 * Routine that is called when the shared library is loaded by
 * itk::ObjectFactoryBase::LoadDynamicFactories().
 *
 * This entry point is a C (not C++) function.
 */
extern "C"
{
  MRMLIDIOPlugin_EXPORT itk::ObjectFactoryBase* ITK_LOAD_FUNCTION_NAME();
}
#endif
