#include "all.h"
#include "itkPolyLineParametricPath.h"
#include "itkSpeedFunctionToPathFilter.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkGradientDescentOptimizer.h"
#include "itkIterateNeighborhoodOptimizer.h"
#include "itkRegularStepGradientDescentOptimizer.h"
#include "itkPathIterator.h"

void getCandidatePathByPieceWise(const FloatImagePointer &speed,
                                 const std::vector<ShortIndex> &seeds,
                                 std::vector<ShortIndex> &pathes)
{
    // 涉及到输入图像处理的，都是float类型， 涉及到结果保存的mask，都是char
    using PixelType = float;
    using OutputPixelType = char;
    using PathType = itk::PolyLineParametricPath<3>;
    using PathFilterType = itk::SpeedFunctionToPathFilter<itk::Image<PixelType, 3>, PathType>;
    using CoordRepType = PathFilterType::CostFunctionType::CoordRepType;
    using PathIteratorType = itk::PathIterator<itk::Image<OutputPixelType, 3>, PathType>;

    // 1. Create interpolator 构建插值函数
    using InterpolatorType = itk::LinearInterpolateImageFunction<itk::Image<PixelType, 3>, CoordRepType>;
    InterpolatorType::Pointer interp = InterpolatorType::New();

    // 2. Create cost function 构建损失函数
    PathFilterType::CostFunctionType::Pointer cost = PathFilterType::CostFunctionType::New();
    cost->SetInterpolator(interp);

    // 3. Create optimizer 构建优化器
    // 这个优化器容易有问题
    // using OptimizerType = itk::GradientDescentOptimizer;
    // OptimizerType::Pointer optimizer = OptimizerType::New();
    // optimizer->SetNumberOfIterations(1000);

    using OptimizerType = itk::IterateNeighborhoodOptimizer;
    OptimizerType::Pointer optimizer = OptimizerType::New();
    optimizer->MinimizeOn();
    optimizer->FullyConnectedOn();
    typename OptimizerType::NeighborhoodSizeType size(3); // Array<double>类型
    for (unsigned int i = 0; i < 3; i++)
        size[i] = speed->GetSpacing()[i] * 1.0;
    optimizer->SetNeighborhoodSize(size);

    // using OptimizerType = itk::RegularStepGradientDescentOptimizer;
    // OptimizerType::Pointer optimizer = OptimizerType::New();
    // optimizer->SetNumberOfIterations(1000);
    // optimizer->SetMaximumStepLength(0.5);
    // optimizer->SetMinimumStepLength(0.1);
    // optimizer->SetRelaxationFactor(0.5);

    // 4. Create path filter 构建路径Filter
    PathFilterType::Pointer pathFilter = PathFilterType::New();
    pathFilter->SetInput(speed);
    pathFilter->SetCostFunction(cost);
    pathFilter->SetOptimizer(optimizer);
    pathFilter->SetTerminationValue(0.1);

    // 5. Add path information
    // 在这里分段进行, 每两个点作为起始点，每段都作为一个路径, 测一下会不会默认多条路径一起算

    // int startNum = 1;
    // int endNum = 3;
    // PathFilterType::PointType start, end;
    // start[0] = seeds[startNum][0];
    // start[1] = seeds[startNum][1];
    // start[2] = seeds[startNum][2];
    // end[0] = seeds[endNum][0];
    // end[1] = seeds[endNum][1];
    // end[2] = seeds[endNum][2];
    // info->SetStartPoint(start);
    // info->SetEndPoint(end);

    using PathInformationType = PathFilterType::PathInformationType;
    PathInformationType::Pointer info = PathInformationType::New();
    PathFilterType::PointType start, end;
    for (int i = 1; i < seeds.size(); i++)
    {
        start[0] = seeds[i - 1][0];
        start[1] = seeds[i - 1][1];
        start[2] = seeds[i - 1][2];
        end[0] = seeds[i][0];
        end[1] = seeds[i][1];
        end[2] = seeds[i][2];
        // image->TransformIndexToPhysicalPoint(seeds[i], end);
        PathInformationType::Pointer info = PathInformationType::New();
        info->SetStartPoint(start);
        info->SetEndPoint(end);
        pathFilter->AddPathInformation(info);
    }

    pathFilter->Update();
    int outPathNum = pathFilter->GetNumberOfOutputs();
    std::vector<std::vector<ShortIndex>> pieceWisePathes;
    for (unsigned int path_i = 0; path_i < outPathNum; path_i++)
    {
        PathType::Pointer path = pathFilter->GetOutput(path_i);
        // 路径点转为shortindex类型， Rasterize栅格化
        const PathType::VertexListType *vertexList = path->GetVertexList();
        // 把每条路径原先就有的种子点加进来(每条路径的起始点)
        pathes.push_back({seeds[path_i][0], seeds[path_i][1], seeds[path_i][2]});
        // 再把产生的路径点加进来
        ShortIndex t;
        for (int i = vertexList->Size() - 1; i >= 0; --i)
        {
            t = {static_cast<signed long>(vertexList->GetElement(i)[0]),
                 static_cast<signed long>(vertexList->GetElement(i)[1]),
                 static_cast<signed long>(vertexList->GetElement(i)[2])};
            pathes.push_back(t);
        }
        // 把每条路径原先就有的种子点加进来(每条路径的终止点)
        pathes.push_back({seeds[path_i + 1][0], seeds[path_i + 1][1], seeds[path_i + 1][2]});
        pieceWisePathes.emplace_back(pathes);
    }
}

void getCandidatePathByWhole(const FloatImagePointer &speed,
                             const std::vector<ShortIndex> &seeds,
                             std::vector<ShortIndex> &pathes)
{
    // 涉及到输入图像处理的，都是float类型， 涉及到结果保存的mask，都是char
    using PixelType = float;
    using OutputPixelType = char;
    using PathType = itk::PolyLineParametricPath<3>;
    using PathFilterType = itk::SpeedFunctionToPathFilter<itk::Image<PixelType, 3>, PathType>;
    using CoordRepType = PathFilterType::CostFunctionType::CoordRepType;
    using PathIteratorType = itk::PathIterator<itk::Image<OutputPixelType, 3>, PathType>;

    // 1. Create interpolator 构建插值函数
    using InterpolatorType = itk::LinearInterpolateImageFunction<itk::Image<PixelType, 3>, CoordRepType>;
    InterpolatorType::Pointer interp = InterpolatorType::New();

    // 2. Create cost function 构建损失函数
    PathFilterType::CostFunctionType::Pointer cost = PathFilterType::CostFunctionType::New();
    cost->SetInterpolator(interp);

    // 3. Create optimizer 构建优化器
    using OptimizerType = itk::IterateNeighborhoodOptimizer;
    typename OptimizerType::Pointer optimizer = OptimizerType::New();
    optimizer->MinimizeOn();
    optimizer->FullyConnectedOn();
    typename OptimizerType::NeighborhoodSizeType size(3);
    for (unsigned int i = 0; i < 3; i++)
        size[i] = speed->GetSpacing()[i] * 1.0;
    optimizer->SetNeighborhoodSize(size);

    // 4. Create path filter 构建路径Filter
    PathFilterType::Pointer pathFilter = PathFilterType::New();
    pathFilter->SetInput(speed);
    pathFilter->SetCostFunction(cost);
    pathFilter->SetOptimizer(optimizer);
    pathFilter->SetTerminationValue(0.1);

    // 5. Add path information
    using PathInformationType = PathFilterType::PathInformationType;
    PathInformationType::Pointer info = PathInformationType::New();

    int seedsSize = seeds.size();
    PathFilterType::PointType start, end, way;
    for (int i = 0; i < seedsSize; i++)
    {
        if (i == 0)
        {
            // info->SetStartPoint((seeds[i][0], seeds[i][1], seeds[i][2]));
            // 不可以用这种写法，无效，而且会得到不想要的结果
            start[0] = seeds[i][0];
            start[1] = seeds[i][1];
            start[2] = seeds[i][2];
            info->SetStartPoint(start);
        }
        else if (i == (seedsSize - 1))
        {
            end[0] = seeds[i][0];
            end[1] = seeds[i][1];
            end[2] = seeds[i][2];
            info->SetEndPoint(end);
        }
        else
        {
            way[0] = seeds[i][0];
            way[1] = seeds[i][1];
            way[2] = seeds[i][2];
            info->AddWayPoint(way);
        }
    }
    pathFilter->AddPathInformation(info);
    pathFilter->Update();

    PathType::Pointer path = pathFilter->GetOutput(0);
    const PathType::VertexListType *vertexList = path->GetVertexList();
    // 路径点类型转换为shortindex
    ShortIndex t;
    for (unsigned int i = 0; i < vertexList->Size(); ++i)
    {
        // std::cout << vertexList->GetElement(i) << std::endl;
        t = {static_cast<signed long>(vertexList->GetElement(i)[0]),
             static_cast<signed long>(vertexList->GetElement(i)[1]),
             static_cast<signed long>(vertexList->GetElement(i)[2])};
        pathes.emplace_back(t);
    }
}

/**
 * @brief
 * 测试是使用分段路径，即每两个点作为start，end构成一段路径，由多段路径组成一条路径，整体是分段路径
 * 还是直接所有点的构成一条路径，只有一个start，一个end，中间的都是way
 * 这两种方式哪种快
 * 主要是想知道using PathFilterType = itk::SpeedFunctionToPathFilter<itk::Image<PixelType, 3>, PathType>;
 * 这个PathFilter会不会自动看到多条路径，就自动用多线程的方式
 */
void testPieceWisePathOrWholeOnePath()
{
    itk::TimeProbesCollectorBase m_timer;
    std::string imagePath = "/Users/huangshan/Documents/All_Projects/b1_interactiveTubeSeg/deploy/data/speed.nii.gz";
    FloatImagePointer inputImage = readImage<float>(imagePath);

    std::vector<ShortIndex> seeds = {
        {94, 30, 226},
        {70, 39, 99},
        {51, 42, 53},
        {33, 47, 30},
        {30, 54, 31}};

    std::vector<ShortIndex> pathesByPieceWise;
    std::vector<ShortIndex> pathesByWhole;
    for (int i = 0; i < 10; i++)
    {
        m_timer.Start("pathesByPieceWise");
        getCandidatePathByPieceWise(inputImage, seeds, pathesByPieceWise);
        m_timer.Stop("pathesByPieceWise");

        m_timer.Start("pathesByWhole");
        getCandidatePathByWhole(inputImage, seeds, pathesByWhole);
        m_timer.Stop("pathesByWhole");
    }

    std::cout << "pathesByPieceWise" << std::endl;
    for (int i = 0; i < 10; i++)
    {
        std::cout << "point " << i << " : [ {" << pathesByPieceWise[i][0] << "}, {" << pathesByPieceWise[i][1] << "}, {" << pathesByPieceWise[i][2] << "} ]" << std::endl;
    }

    std::cout << "pathesByWhole" << std::endl;
    for (int i = 0; i < 10; i++)
    {
        std::cout << "point " << i << " : [ {" << pathesByWhole[i][0] << "}, {" << pathesByWhole[i][1] << "}, {" << pathesByWhole[i][2] << "} ]" << std::endl;
    }

    m_timer.Report(std::cout);
}

/**
结果：
Name Of Probe (Time)          Iterations     Total (s)      Min (s)        Mean (s)       Max (s)        StdDev (s)
pathesByPieceWise             10             67.7279        6.09444        6.77279        7.55736        0.441216
pathesByWhole                 10             154.107        13.5495        15.4107        17.0222        1.24417

所以分段路径确实会提速
 */