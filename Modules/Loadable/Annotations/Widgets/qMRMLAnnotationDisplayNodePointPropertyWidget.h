
#ifndef __qMRMLAnnotationDisplayNodePointPropertyWidget_h
#define __qMRMLAnnotationDisplayNodePointPropertyWidget_h

// CTK includes
#include <ctkPimpl.h>

// Annotations includes
#include "qMRMLAnnotationDisplayNodePropertyWidget.h"


#include "qSlicerAnnotationsModuleWidgetsExport.h"

class qMRMLAnnotationDisplayNodePointPropertyWidgetPrivate;
class vtkMRMLVolumeNode;

/// \ingroup Slicer_QtModules_Annotations
class Q_SLICER_MODULE_ANNOTATIONS_WIDGETS_EXPORT qMRMLAnnotationDisplayNodePointPropertyWidget :
    public qMRMLAnnotationDisplayNodePropertyWidget
{
  Q_OBJECT

public:

  using Superclass = qMRMLAnnotationDisplayNodePropertyWidget;
  explicit qMRMLAnnotationDisplayNodePointPropertyWidget(QWidget *newParent = 0);
  virtual ~qMRMLAnnotationDisplayNodePointPropertyWidget();

public slots:

  void updateWidgetFromMRML();

  void updateMRMLFromWidget();

private slots:



protected:
  QScopedPointer<qMRMLAnnotationDisplayNodePointPropertyWidgetPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qMRMLAnnotationDisplayNodePointPropertyWidget);
  Q_DISABLE_COPY(qMRMLAnnotationDisplayNodePointPropertyWidget);
  using ctkPimpl = qMRMLAnnotationDisplayNodePointPropertyWidgetPrivate;
  bool updating;

};

#endif // __qMRMLAnnotationDisplayNodePointPropertyWidget_h
