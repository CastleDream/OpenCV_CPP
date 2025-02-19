#include "all.h"
#include "itkMultiplyImageFilter.h"
#include "itkIntensityWindowingImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkTimeProbesCollectorBase.h"
#include "itkChangeLabelImageFilter.h"
#include "itkAddImageFilter.h"

void convert2SpeedByRawFilter(const ShortImagePointer &image, FloatImagePointer &speed)
{
    using IntensityFilterType = itk::IntensityWindowingImageFilter<itk::Image<short, 3>, itk::Image<float, 3>>;
    using FilterType = itk::MultiplyImageFilter<itk::Image<short, 3>, itk::Image<short, 3>, itk::Image<short, 3>>;

    // 对图像乘以-1来进行反向
    auto filter = FilterType::New();
    filter->SetInput(image);
    filter->SetConstant(-1);

    auto intensityWindowing = IntensityFilterType::New();
    // 设置输入图像的可见范围, 相当于只要黑色的部分
    intensityWindowing->SetWindowMinimum(0);
    intensityWindowing->SetWindowMaximum(1024);
    // 设置输出图像的输出范围，线性变换
    intensityWindowing->SetOutputMinimum(0.0);
    intensityWindowing->SetOutputMaximum(1.0);
    intensityWindowing->SetInput(filter->GetOutput());

    // intensityWindowing->Update();
    // speed = intensityWindowing->GetOutput();

    // MinimalPath Extraction算法要求speed是[0,1]之间，靠近路径的地方越接近1越好，其他地方都是0最好，这样计算最快
    // 因此不需要和以前荣昊写的完全一致，不是范围在[0.1,1.1]之间
    // 需要将黑色背景的0加上一个小数， https://docs.itk.org/projects/doxygen/en/stable/classitk_1_1ChangeLabelImageFilter.html
    // using ChangeLabelFilterType = itk::ChangeLabelImageFilter<itk::Image<float, 3>, itk::Image<float, 3>>;
    // ChangeLabelFilterType::Pointer changeFilter = ChangeLabelFilterType::New();
    // changeFilter->SetInput(intensityWindowing->GetOutput());
    // changeFilter->SetChange(0.0f, 0.1f); // 将 0 替换为 0.1

    // changeFilter->Update();
    // speed = changeFilter->GetOutput();

    // 图像整体加上一个数字 https://examples.itk.org/src/filtering/imageintensity/addconstanttoeverypixel/documentation
    using AddImageFilterType = itk::AddImageFilter<itk::Image<float, 3>, itk::Image<float, 3>, itk::Image<float, 3>>;
    auto addImageFilter = AddImageFilterType::New();
    addImageFilter->SetInput(intensityWindowing->GetOutput());
    addImageFilter->SetConstant2(0.1);
    addImageFilter->Update();
    speed = addImageFilter->GetOutput();

    // speed需要设置origin是(0,0,0) spacing是(1,1,1)
    // https://itk.org/ITKSoftwareGuide/html/Book1/ITKSoftwareGuide-Book1ch4.html#x45-490004
    const itk::SpacePrecisionType spacing[3] = {1.0, 1.0, 1.0};
    const itk::SpacePrecisionType origin[3] = {0.0, 0.0, 0.0};
    speed->SetOrigin(origin);
    speed->SetSpacing(spacing);
}

void convert2SpeedByIterators(const ShortImagePointer &image, FloatImagePointer &speed)
{
    const itk::Image<short, 3>::SizeType inputSize = image->GetLargestPossibleRegion().GetSize();
    itk::Image<float, 3>::RegionType region{{0, 0, 0}, inputSize};
    speed->SetRegions(region);
    speed->SetDirection(image->GetDirection());
    speed->Allocate();
    speed->FillBuffer(0.0f);

    itk::ImageRegionIterator<itk::Image<short, 3>> imageIter(image, image->GetLargestPossibleRegion());
    itk::ImageRegionIterator<itk::Image<float, 3>> speedIter(speed, speed->GetLargestPossibleRegion());
    float replaceDiv2Multi = 1 / 1024.0;
    for (imageIter.GoToBegin(), speedIter.GoToBegin(); !imageIter.IsAtEnd(); ++imageIter, ++speedIter)
    {
        short t = std::min(short(0), std::max(short(-1024), imageIter.Value()));
        speedIter.Value() = (1024.0f - t) * replaceDiv2Multi - 1.0f + 0.1f;
    }
}

/**
 * @brief
 * 测试相同功能，使用ITK原生的Filter快，还是采用手动Iterators的方式快
 * https://blog.csdn.net/Castlehe/article/details/127491634
 */
void testRawFilterWithIterators()
{
    itk::TimeProbesCollectorBase m_timer;

    // std::string imagePath = "/Users/huangshan/Documents/DailyStudy/brain/image.nii.gz";
    std::string imagePath = "/Users/huangshan/Documents/All_Projects/b1_interactiveTubeSeg/deploy/data/region.nii.gz";
    ShortImagePointer inputImage = readImage<short>(imagePath);

    FloatImagePointer speedByRawFilter = itk::Image<float, 3U>::New();
    FloatImagePointer speedByIterators = itk::Image<float, 3U>::New();

    for (int i = 0; i < 10; i++)
    {
        m_timer.Start("RawFilterTime");
        convert2SpeedByRawFilter(inputImage, speedByRawFilter);
        m_timer.Stop("RawFilterTime");

        m_timer.Start("IteratorsTime");
        convert2SpeedByIterators(inputImage, speedByIterators);
        m_timer.Stop("IteratorsTime");
    }
    saveImage<float>(speedByRawFilter, "/Users/huangshan/Documents/DailyStudy/cpp/ITK/data/speedByRawFilter.nii.gz");
    saveImage<float>(speedByIterators, "/Users/huangshan/Documents/DailyStudy/cpp/ITK/data/speedByIterators.nii.gz");
    m_timer.Report(std::cout);
}
/**
对于 512*512*713 214.8MB的大图，
System:              huangshandeMacBook-Pro.local
Processor:           Intel(R) Core(TM) i5-8257U CPU @ 1.40GHz
    Cache:           32768
    Clock:           1400
    Physical CPUs:   4
    Logical CPUs:    8
    Virtual Memory:  Total: 3072            Available: 1681
    Physical Memory: Total: 8192            Available: 2499
OSName:              macOS
    Release:         13.2.1
    Version:         22D68
    Platform:        x86_64
    Operating System is 64 bit
ITK Version: 5.2.1

Name Of Probe (Time)          Iterations     Total (s)      Min (s)        Mean (s)       Max (s)        StdDev (s)
IteratorsTime                 10             64.144         5.79833        6.4144         9.1834         1.09771
RawFilterTime                 10             48.9801        3.75677        4.89801        7.67313        1.39564
 */

/**
对于 125*85*257 4.3MB的小图

Name Of Probe (Time)          Iterations     Total (s)      Min (s)        Mean (s)       Max (s)        StdDev (s)
IteratorsTime                 10             0.884679       0.0842559      0.0884679      0.097604       0.00398879
RawFilterTime                 10             0.60102        0.050828       0.060102       0.0746241      0.00708452

综上，
- 用原生的Filters速度更快，图越大，速度差距越明显
- 虽然用Iterators写看起来更直观

*/