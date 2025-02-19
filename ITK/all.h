#pragma once
#include "itkTimeProbesCollectorBase.h"
#include "itkImage.h"
#include "itkNiftiImageIO.h"
#include "itkImageFileWriter.h"
#include "itkImageFileReader.h"

typedef itk::Image<short, 3>::Pointer ShortImagePointer;
typedef itk::Image<short, 3>::IndexType ShortIndex;
typedef itk::Image<float, 3>::Pointer FloatImagePointer;

// 辅助函数
template <typename T>
void saveImage(typename itk::Image<T, 3>::Pointer &image, std::string savePath)
{
    using OutputImageType = itk::Image<T, 3>;
    typedef itk::ImageFileWriter<OutputImageType> WriterType;
    typename WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(savePath);
    writer->SetImageIO(itk::NiftiImageIO::New());
    writer->SetInput(image);
    writer->Update();
    std::cout << "Result has been Writen in " << savePath << std::endl;
}

template <typename T>
typename itk::Image<T, 3>::Pointer readImage(std::string inputImagePath)
{
    using ImageType = itk::Image<T, 3>;
    typedef itk::ImageFileReader<ImageType> ImageReaderType;
    typename ImageReaderType::Pointer readerSpeed = ImageReaderType::New();
    readerSpeed->SetImageIO(itk::NiftiImageIO::New());
    readerSpeed->SetFileName(inputImagePath);
    readerSpeed->Update();
    typename ImageType::Pointer Image = readerSpeed->GetOutput();
    return Image;
}

void convert2SpeedByRawFilter(const ShortImagePointer &image, FloatImagePointer &speed);
void convert2SpeedByIterators(const ShortImagePointer &image, FloatImagePointer &speed);
void testRawFilterWithIterators();

void getCandidatePathByPieceWise(const FloatImagePointer &speed,
                                 const std::vector<ShortIndex> &seeds,
                                 std::vector<ShortIndex> &pathes);
void getCandidatePathByWhole(const FloatImagePointer &speed,
                             const std::vector<ShortIndex> &seeds,
                             std::vector<ShortIndex> &pathes);
void testPieceWisePathOrWholeOnePath();
