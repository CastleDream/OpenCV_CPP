#include <string>
#include <iostream>
#include <sstream>

/**
 * @brief 对图像路径添加额外的序号和字符
 * 如果需要返回值是 char* 类型，且调用者需要长期保存返回值，可以动态分配内存 (new 或 malloc)，并返回该分配的 char*
 * ref:
 * https://www.cnblogs.com/keye/p/17901216.html
 *  std::string::npos 是 std::string 类中的一个静态常量，通常用于表示字符串操作中的特殊值，表示在字符串中未找到匹配的位置。
 *  npos 是 size_t 类型的常量，其值在不同平台上可能有所不同，但通常是一个非常大的正整数。
 * @param originalPath: 原始路径
 * @param i: 需要添加的序号
 * @param iEye: 左眼或者右眼，L或者R
 * @return char*
 */
char *getNewFilePath(char *originalPath, int i, char iEye)
{
    // 将C风格字符串转换为C++的std::string

    std::string path(originalPath);

    // 找到最后一个斜杠以分离路径和文件名

    size_t lastSlashPos = path.find_last_of("/\\");
    std::string directory = (lastSlashPos == std::string::npos) ? "" : path.substr(0, lastSlashPos + 1);
    std::string fileName = (lastSlashPos == std::string::npos) ? path : path.substr(lastSlashPos + 1);

    // 找到文件名中的最后一个点以分离文件名和扩展名

    size_t dotPos = fileName.find_last_of('.');
    std::string baseName = (dotPos == std::string::npos) ? fileName : fileName.substr(0, dotPos);
    std::string extension = (dotPos == std::string::npos) ? "" : fileName.substr(dotPos);

    // 构造新的文件名

    std::ostringstream newFileNameStream;
    newFileNameStream << baseName << "_" << i << "_" << iEye << extension;
    std::string newFileName = newFileNameStream.str();

    // 合并新路径

    std::string newPath = directory + newFileName;

    // 动态分配内存并返回C风格字符串

    char *result = new char[newPath.length() + 1];
    std::strcpy(result, newPath.c_str());
    return result;
}
