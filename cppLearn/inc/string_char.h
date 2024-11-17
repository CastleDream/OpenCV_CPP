
/**
 * @brief 对图像路径添加额外的序号和字符
 * @param originalPath: 原始路径
 * @param i: 需要添加的序号
 * @param iEye: 左眼或者右眼，L或者R
 * @return char*
 */
char *getNewFilePath(char *originalPath, int i, char iEye);