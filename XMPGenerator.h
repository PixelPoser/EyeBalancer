#ifndef XMP_GENERATOR_H
#define XMP_GENERATOR_H

#include <string>
#include "XMP.hpp"

class XMPGenerator
{
public:
    XMPGenerator();
    ~XMPGenerator();

    void generateXMP(float cropTop, float cropLeft, float cropBottom, float cropRight,
                     float cropAngle, bool constrainToWarp, bool hasCrop);
    void processFolder(const std::string& folderPath, bool outputXMPForJPEGs);

private:
    std::string getFileFormat(const std::string& filePath);
    bool hasAlignmentData(const std::string& rawFilePath);
    void updateAlignmentXMP(const std::string& rawFilePath);
    void saveXMPSidecar(const std::string& rawFilePath);
    void embedXMP(const std::string& imagePath);

    SXMPMeta m_xmp;
};

#endif // XMP_GENERATOR_H