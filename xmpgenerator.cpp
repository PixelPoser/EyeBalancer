#include "XMPGenerator.h"
#include "XMP.hpp"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

XMPGenerator::XMPGenerator() {
    SXMPMeta::Initialize();
    SXMPFiles::Initialize();
}

XMPGenerator::~XMPGenerator() {
    SXMPFiles::Terminate();
    SXMPMeta::Terminate();
}

void XMPGenerator::generateXMP(float cropTop, float cropLeft, float cropBottom, float cropRight,
                               float cropAngle, bool constrainToWarp, bool hasCrop) {
    m_xmp.Clear();
    std::string namespaceURI = "http://ns.adobe.com/camera-raw-settings/1.0/";

    m_xmp.SetProperty_Float(namespaceURI.c_str(), "CropTop", cropTop);
    m_xmp.SetProperty_Float(namespaceURI.c_str(), "CropLeft", cropLeft);
    m_xmp.SetProperty_Float(namespaceURI.c_str(), "CropBottom", cropBottom);
    m_xmp.SetProperty_Float(namespaceURI.c_str(), "CropRight", cropRight);
    m_xmp.SetProperty_Float(namespaceURI.c_str(), "CropAngle", cropAngle);
    m_xmp.SetProperty_Bool(namespaceURI.c_str(), "CropConstrainToWarp", constrainToWarp);
    m_xmp.SetProperty_Bool(namespaceURI.c_str(), "HasCrop", hasCrop);
}

void XMPGenerator::processFolder(const std::string& folderPath, bool outputXMPForJPEGs) {
    namespace fs = boost::filesystem;

    try {
        if (fs::exists(folderPath) && fs::is_directory(folderPath)) {
            for (const auto& entry : fs::recursive_directory_iterator(folderPath)) {
                const auto& path = entry.path();
                if (fs::is_regular_file(path)) {
                    std::string fileFormat = getFileFormat(path.string());
                    if (fileFormat == "RAW") {
                        if (hasAlignmentData(path.string())) {
                            updateAlignmentXMP(path.string());
                        }
                        saveXMPSidecar(path.string());
                    } else if (fileFormat == "JPEG" && outputXMPForJPEGs) {
                        embedXMP(path.string());
                    }
                }
            }
        } else {
            throw std::runtime_error("Invalid folder path: " + folderPath);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error processing folder: " << e.what() << std::endl;
    }
}

std::string XMPGenerator::getFileFormat(const std::string& filePath) {
    std::string extension = boost::filesystem::path(filePath).extension().string();
    boost::algorithm::to_lower(extension);

    if (extension == ".jpg" || extension == ".jpeg") {
        return "JPEG";
    } else if (extension == ".cr2" || extension == ".cr3" || extension == ".nef" ||
               extension == ".arw" || extension == ".rw2" || extension == ".raf") {
        return "RAW";
    } else {
        return "Unknown";
    }
}

bool XMPGenerator::hasAlignmentData(const std::string& rawFilePath) {
    SXMPFiles file;
    if (file.OpenFile(rawFilePath.c_str(), kXMP_UnknownFile, kXMPFiles_OpenForRead)) {
        SXMPMeta xmp;
        if (file.GetXMP(&xmp)) {
            std::string namespaceURI = "http://ns.adobe.com/camera-raw-settings/1.0/";
            if (xmp.DoesPropertyExist(namespaceURI.c_str(), "HasCrop")) {
                bool hasCrop;
                xmp.GetProperty_Bool(namespaceURI.c_str(), "HasCrop", &hasCrop, nullptr);
                if (hasCrop) {
                    return true;
                }
            }
        }
        file.CloseFile();
    }
    return false;
}

void XMPGenerator::updateAlignmentXMP(const std::string& rawFilePath) {
    SXMPFiles file;
    if (file.OpenFile(rawFilePath.c_str(), kXMP_UnknownFile, kXMPFiles_OpenForUpdate)) {
        SXMPMeta xmp;
        if (file.GetXMP(&xmp)) {
            std::string namespaceURI = "http://ns.adobe.com/camera-raw-settings/1.0/";
            float cropTop, cropLeft, cropBottom, cropRight, cropAngle;
            bool constrainToWarp;

            if (xmp.GetProperty_Float(namespaceURI.c_str(), "CropTop", &cropTop, nullptr) &&
                xmp.GetProperty_Float(namespaceURI.c_str(), "CropLeft", &cropLeft, nullptr) &&
                xmp.GetProperty_Float(namespaceURI.c_str(), "CropBottom", &cropBottom, nullptr) &&
                xmp.GetProperty_Float(namespaceURI.c_str(), "CropRight", &cropRight, nullptr) &&
                xmp.GetProperty_Float(namespaceURI.c_str(), "CropAngle", &cropAngle, nullptr) &&
                xmp.GetProperty_Bool(namespaceURI.c_str(), "CropConstrainToWarp", &constrainToWarp, nullptr)) {
                m_xmp.SetProperty_Float(namespaceURI.c_str(), "CropTop", cropTop);
                m_xmp.SetProperty_Float(namespaceURI.c_str(), "CropLeft", cropLeft);
                m_xmp.SetProperty_Float(namespaceURI.c_str(), "CropBottom", cropBottom);
                m_xmp.SetProperty_Float(namespaceURI.c_str(), "CropRight", cropRight);
                m_xmp.SetProperty_Float(namespaceURI.c_str(), "CropAngle", cropAngle);
                m_xmp.SetProperty_Bool(namespaceURI.c_str(), "CropConstrainToWarp", constrainToWarp);
            }
        }
        file.CloseFile();
    }
}

void XMPGenerator::saveXMPSidecar(const std::string& rawFilePath) {
    std::string xmpFilePath = boost::filesystem::path(rawFilePath).replace_extension(".xmp").string();
    std::string xmpBuffer;
    m_xmp.SerializeToBuffer(&xmpBuffer, kXMP_UseCompactFormat);

    std::ofstream xmpFile(xmpFilePath, std::ios::out | std::ios::binary);
    if (xmpFile.is_open()) {
        xmpFile.write(xmpBuffer.c_str(), xmpBuffer.size());
        xmpFile.close();
    } else {
        throw std::runtime_error("Failed to save XMP sidecar file: " + xmpFilePath);
    }
}

void XMPGenerator::embedXMP(const std::string& imagePath) {
    SXMPFiles file;
    if (file.OpenFile(imagePath.c_str(), kXMP_UnknownFile, kXMPFiles_OpenForUpdate)) {
        std::string xmpBuffer;
        m_xmp.SerializeToBuffer(&xmpBuffer, kXMP_UseCompactFormat);

        if (file.PutXMP(xmpBuffer.c_str())) {
            file.CloseFile();
        } else {
            file.CloseFile();
            throw std::runtime_error("Failed to embed XMP metadata in file: " + imagePath);
        }
    } else {
        throw std::runtime_error("Failed to open file for XMP embedding: " + imagePath);
    }
}