/*=========================================================================

  Program:   Slicer
  Language:  C++
  Module:    $HeadURL$
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Brigham and Women's Hospital (BWH) All Rights Reserved.

  See License.txt or http://www.slicer.org/copyright/copyright.txt for details.

==========================================================================*/

#include "itkConfidenceConnectedImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkCurvatureFlowImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkPluginFilterWatcher.h"

#include "SimpleRegionGrowingSegmentationCLP.h"

// Use an anonymous namespace to keep class types and function names
// from colliding when module is used as shared object module.  Every
// thing should be in an anonymous namespace except for the module
// entry point, e.g. main()
//
namespace
{

} // end of anonymous namespace

int main( int argc, char *argv[] )
{
  PARSE_ARGS;

  using InternalPixelType = float;
  const     unsigned int Dimension = 3;
  using InternalImageType = itk::Image<InternalPixelType, Dimension>;

  using OutputPixelType = unsigned short;
  using OutputImageType = itk::Image<OutputPixelType, Dimension>;

  using CastingFilterType = itk::CastImageFilter<InternalImageType, OutputImageType>;
  CastingFilterType::Pointer caster = CastingFilterType::New();

  using ReaderType = itk::ImageFileReader<InternalImageType>;
  using WriterType = itk::ImageFileWriter<OutputImageType>;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName( inputVolume.c_str() );
  reader->Update();

  writer->SetFileName( outputVolume.c_str() );

  using CurvatureFlowImageFilterType = itk::CurvatureFlowImageFilter<InternalImageType, InternalImageType>;
  CurvatureFlowImageFilterType::Pointer smoothing =
    CurvatureFlowImageFilterType::New();
  itk::PluginFilterWatcher watcher1(smoothing, "Smoothing",
                                    CLPProcessInformation, 0.5, 0.0);

  using ConnectedFilterType = itk::ConfidenceConnectedImageFilter<InternalImageType, InternalImageType>;
  ConnectedFilterType::Pointer confidenceConnected = ConnectedFilterType::New();
  itk::PluginFilterWatcher     watcher2(confidenceConnected, "Segmenting",
                                        CLPProcessInformation, 0.5, 0.5);

  smoothing->SetInput( reader->GetOutput() );
  confidenceConnected->SetInput( smoothing->GetOutput() );
  caster->SetInput( confidenceConnected->GetOutput() );
  writer->SetInput( caster->GetOutput() );
  writer->SetUseCompression(true);

  smoothing->SetNumberOfIterations( smoothingIterations );
  smoothing->SetTimeStep( timestep );

  confidenceConnected->SetMultiplier( multiplier );
  confidenceConnected->SetNumberOfIterations( iterations );
  confidenceConnected->SetReplaceValue( labelvalue );
  confidenceConnected->SetInitialNeighborhoodRadius( neighborhood );

  if( seed.size() > 0 )
    {
    InternalImageType::PointType lpsPoint;
    InternalImageType::IndexType index;
    for( ::size_t i = 0; i < seed.size(); ++i )
      {
      // seeds come in ras, convert to lps
      lpsPoint[0] = -seed[i][0];
      lpsPoint[1] = -seed[i][1];
      lpsPoint[2] = seed[i][2];

      reader->GetOutput()->TransformPhysicalPointToIndex(lpsPoint, index);
      confidenceConnected->AddSeed(index);

//       std::cout << "LPS: " << lpsPoint << std::endl;
//       std::cout << "IJK: " << index << std::endl;
      }
    }
  else
    {
    std::cerr << "No seeds specified." << std::endl;
    return -1;
    }

  try
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & excep )
    {
    std::cerr << "Exception caught !" << std::endl;
    std::cerr << excep << std::endl;
    }

  return 0;
}
