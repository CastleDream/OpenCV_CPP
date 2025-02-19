#include "all.h"
#include "itkPolyLineParametricPath.h"
#include "itkSpeedFunctionToPathFilter.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkGradientDescentOptimizer.h"
#include "itkIterateNeighborhoodOptimizer.h"
#include "itkRegularStepGradientDescentOptimizer.h"
#include "itkPathIterator.h"

std::vector<std::vector<ShortIndex>> getCandidatePathByPieceWise(const FloatImagePointer &speed,
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
    pathFilter->SetTerminationValue(0.2);

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
    std::vector<ShortIndex> tempPath;
    for (unsigned int path_i = 0; path_i < outPathNum; path_i++)
    {
        PathType::Pointer path = pathFilter->GetOutput(path_i);
        // 路径点转为shortindex类型， Rasterize栅格化
        const PathType::VertexListType *vertexList = path->GetVertexList();
        // 把每条路径原先就有的种子点加进来(每条路径的起始点), 只包含第一条的就可以，因为后续的起点都是上一条的终点
        // 合并的时候，就不需要去重了
        if (path_i == 0)
        {
            tempPath.push_back({seeds[path_i][0], seeds[path_i][1], seeds[path_i][2]});
        }
        std::cout << "path_i: " << path_i << std::endl;

        // 再把产生的路径点加进来
        // 1.路径点是不包含seeds点的(PathFilter得到的路径不包含起始点，但是包含AddWay的中间点)
        // 2.路径点按照顺序从第一个点到最后一个点，是从终点逆向到起点的，这与算法有关，算法是根据梯度逆向搜索的
        // 3.因此后续要进行多条路径的拼接，那么要保证每条路径中，起点，中间的路径点，终点， 这三者是可以构成顺序的
        //   则就需要让路径点，倒着来索引
        ShortIndex t;
        for (int i = vertexList->Size() - 1; i >= 0; i--)
        {
            t = {static_cast<signed long>(vertexList->GetElement(i)[0]),
                 static_cast<signed long>(vertexList->GetElement(i)[1]),
                 static_cast<signed long>(vertexList->GetElement(i)[2])};
            tempPath.push_back(t);
        }
        // 把每条路径原先就有的种子点加进来(每条路径的终止点)
        tempPath.push_back({seeds[path_i + 1][0], seeds[path_i + 1][1], seeds[path_i + 1][2]});
        int pathLastPointIndex = tempPath.size() - 1;

        pieceWisePathes.emplace_back(tempPath);
        pathes.insert(pathes.end(), tempPath.begin(), tempPath.end());

        if (path_i == 0)
        { // 只有第一条，需要输出： 起点，起点后的10， 终点前的10个点，终点
            // 后面的路径，都以上一条的终点作为起点，所以没有加入到path中，防止合并的时候重复
            // 后面的路径，都只会: (起点就是上一条的终点) 前10个点，终点前的10个点，终点
            for (int i = 0; i < tempPath.size(); i++)
            {
                if (i == 0)
                {
                    std::cout << "Start seeds: ";
                    std ::cout << "[ {" << tempPath[i][0] << "}, {" << tempPath[i][1] << "}, {" << tempPath[i][2] << "} ]" << std::endl;
                }
                else if (i == (tempPath.size() - 1))
                {
                    std::cout << "End seeds: ";
                    std ::cout << "[ {" << tempPath[i][0] << "}, {" << tempPath[i][1] << "}, {" << tempPath[i][2] << "} ]" << std::endl;
                }
                else if (i < 10 || i > (tempPath.size() - 10))
                {
                    std ::cout << "point " << i << " : [ {" << tempPath[i][0] << "}, {" << tempPath[i][1] << "}, {" << tempPath[i][2] << "} ]" << std::endl;
                }
            }
        }
        else
        {
            for (int i = 0; i < tempPath.size(); i++)
            {
                if (i == (tempPath.size() - 1))
                {
                    std::cout << "End seeds: ";
                    std ::cout << "[ {" << tempPath[i][0] << "}, {" << tempPath[i][1] << "}, {" << tempPath[i][2] << "} ]" << std::endl;
                }
                else if (i < 10 || i > (tempPath.size() - 10))
                {
                    std ::cout << "point " << i << " : [ {" << tempPath[i][0] << "}, {" << tempPath[i][1] << "}, {" << tempPath[i][2] << "} ]" << std::endl;
                }
            }
        }
        tempPath.clear();
    }

    return pieceWisePathes;
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
    pathFilter->SetTerminationValue(0.2);

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

void savePathMask(const std::vector<ShortIndex> &seeds,
                  const std::vector<ShortIndex> &pathes,
                  const FloatImagePointer &image,
                  CharImagePointer &mask)
{ // 初始化mask
    itk::Image<char, 3>::RegionType region;
    region = image->GetLargestPossibleRegion();
    itk::Image<char, 3>::IndexType start;
    start.Fill(0);
    region.SetIndex(start);

    mask->SetRegions(region);
    mask->Allocate();
    mask->FillBuffer(0);
    mask->SetDirection(image->GetDirection());
    mask->SetSpacing(image->GetSpacing());
    mask->SetOrigin(image->GetOrigin());

    // 赋值 先赋值路径，再赋值seeds，不然会被覆盖
    itk::Image<char, 3>::IndexType maskIndex;
    for (auto point : pathes)
    {
        maskIndex = {{point[0], point[1], point[2]}};
        mask->SetPixel(maskIndex, 6);
    }
    for (auto seed : seeds)
    {
        maskIndex = {{seed[0], seed[1], seed[2]}};
        mask->SetPixel(maskIndex, 5);
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
bool testPieceWisePathOrWholeOnePath()
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

    // 测试分段路径处理， 注释掉这部分就是正常的比较速度了
    // std::vector<std::vector<ShortIndex>> PieceWisePathes;
    // m_timer.Start("pathesByPieceWise");
    // PieceWisePathes = getCandidatePathByPieceWise(inputImage, seeds, pathesByPieceWise);
    // m_timer.Stop("pathesByPieceWise");
    // m_timer.Report(std::cout);
    // CharImagePointer pathSeedsMask = itk::Image<char, 3>::New();
    // savePathMask(seeds, pathesByPieceWise, inputImage, pathSeedsMask);
    // saveImage<char>(pathSeedsMask, "/Users/huangshan/Documents/DailyStudy/cpp/ITK/data/pathSeedsMaskPieceWise.nii.gz");
    // return true;

    std::vector<ShortIndex> pathesByWhole;
    for (int i = 0; i < 10; i++)
    {
        pathesByPieceWise.clear();
        m_timer.Start("pathesByPieceWise");
        getCandidatePathByPieceWise(inputImage, seeds, pathesByPieceWise);
        m_timer.Stop("pathesByPieceWise");

        pathesByWhole.clear();
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
    for (int i = pathesByWhole.size() - 1; i >= pathesByWhole.size() - 10; i--)
    {
        std::cout << "point " << i << " : [ {" << pathesByWhole[i][0] << "}, {" << pathesByWhole[i][1] << "}, {" << pathesByWhole[i][2] << "} ]" << std::endl;
    }

    m_timer.Report(std::cout);
    return true;
}

/**这是第二次pathesByPieceWise中修改最终合并路径， pathesByWhole倒着索引的结果
pathesByPieceWise
point 0 : [ {94}, {30}, {226} ]
point 1 : [ {94}, {30}, {225} ]
point 2 : [ {94}, {30}, {224} ]
point 3 : [ {94}, {30}, {223} ]
point 4 : [ {94}, {30}, {222} ]
point 5 : [ {94}, {30}, {221} ]
point 6 : [ {94}, {30}, {220} ]
point 7 : [ {94}, {30}, {219} ]
point 8 : [ {94}, {30}, {218} ]
point 9 : [ {94}, {30}, {217} ]
pathesByWhole(很明显，PathFiler输出的路径点是不包含 起点和终点的，需要自己手动加上去)
point 198 : [ {94}, {30}, {225} ]
point 197 : [ {94}, {30}, {224} ]
point 196 : [ {94}, {30}, {223} ]
point 195 : [ {94}, {30}, {222} ]
point 194 : [ {94}, {30}, {221} ]
point 193 : [ {94}, {30}, {220} ]
point 192 : [ {94}, {30}, {219} ]
point 191 : [ {94}, {30}, {218} ]
point 190 : [ {94}, {30}, {217} ]
point 189 : [ {94}, {30}, {216} ]
.. 这里少索引了一行，哈哈哈

System:              huangshandeMacBook-Pro.local
Processor:           Intel(R) Core(TM) i5-8257U CPU @ 1.40GHz
    Cache:           32768
    Clock:           1400
    Physical CPUs:   4
    Logical CPUs:    8
    Virtual Memory:  Total: 3072            Available: 1116
    Physical Memory: Total: 8192            Available: 2532
OSName:              macOS
    Release:         13.2.1
    Version:         22D68
    Platform:        x86_64
    Operating System is 64 bit
ITK Version: 5.2.1
Name Of Probe (Time)          Iterations     Total (s)      Min (s)        Mean (s)       Max (s)        StdDev (s)
pathesByPieceWise             10             62.5963        5.40107        6.25963        7.21937        0.668102
pathesByWhole                 10             151.03         12.8025        15.103         16.467         1.35185

依然还是分段速度快，但是依然还是很慢，哪怕把图变小了，很奇怪，198个点，5s？？

和 b1_interactiveTubeSeg/deploy中的log信息差不多，就是5s左右
 *
 */

/**
这是一开始pathesByPieceWise中没有修改最终合并路径， pathesByWhole没有倒着索引的结果， 代码可以在github的提交记录看到
结果：
pathesByPieceWise
point 0 : [ {94}, {30}, {226} ]
point 1 : [ {94}, {30}, {225} ]
point 2 : [ {94}, {30}, {224} ]
point 3 : [ {94}, {30}, {223} ]
point 4 : [ {94}, {30}, {222} ]
point 5 : [ {94}, {30}, {221} ]
point 6 : [ {94}, {30}, {220} ]
point 7 : [ {94}, {30}, {219} ]
point 8 : [ {94}, {30}, {218} ]
point 9 : [ {94}, {30}, {217} ]
pathesByWhole
point 0 : [ {31}, {53}, {30} ]
point 1 : [ {31}, {52}, {30} ]
point 2 : [ {31}, {51}, {30} ]
point 3 : [ {32}, {50}, {30} ]
point 4 : [ {33}, {49}, {30} ]
point 5 : [ {33}, {48}, {30} ]
point 6 : [ {34}, {46}, {31} ]
point 7 : [ {35}, {45}, {32} ]
point 8 : [ {36}, {44}, {33} ]
point 9 : [ {37}, {43}, {34} ]
System:              huangshandeMacBook-Pro.local
Processor:           Intel(R) Core(TM) i5-8257U CPU @ 1.40GHz
    Cache:           32768
    Clock:           1400
    Physical CPUs:   4
    Logical CPUs:    8
    Virtual Memory:  Total: 3072            Available: 1450
    Physical Memory: Total: 8192            Available: 2239
OSName:              macOS
    Release:         13.2.1
    Version:         22D68
    Platform:        x86_64
    Operating System is 64 bit
ITK Version: 5.2.1
Name Of Probe (Time)          Iterations     Total (s)      Min (s)        Mean (s)       Max (s)        StdDev (s)
pathesByPieceWise             10             67.7279        6.09444        6.77279        7.55736        0.441216
pathesByWhole                 10             154.107        13.5495        15.4107        17.0222        1.24417

所以分段路径确实会提速
 */
