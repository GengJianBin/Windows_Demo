#include <windows.h>
#include <iostream>
#include <stdio.h>

int SaveScreenPartAsBmp(int x, int y, int width, int height, const std::string& filename) {
    // 获取桌面设备上下文
    HDC screenDC = GetDC(nullptr);
    HDC memDC = CreateCompatibleDC(screenDC);
    HBITMAP memBmp = CreateCompatibleBitmap(screenDC, width, height);
    HGDIOBJ oldBmp = SelectObject(memDC, memBmp);

    // 使用BitBlt复制屏幕部分到内存DC
    BitBlt(memDC, 0, 0, width, height, screenDC, x, y, SRCCOPY);

    // 创建BMP文件头和信息头
    BITMAPFILEHEADER bmpFileHeader;
    BITMAPINFOHEADER bmpInfoHeader;
    memset(&bmpFileHeader, 0, sizeof(BITMAPFILEHEADER));
    memset(&bmpInfoHeader, 0, sizeof(BITMAPINFOHEADER));

    bmpFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    bmpFileHeader.bfSize = bmpFileHeader.bfOffBits + (width * height * 4);
    bmpFileHeader.bfType = 0x4D42; // 'BM'

    bmpInfoHeader.biBitCount = 32;
    bmpInfoHeader.biCompression = BI_RGB;//0;
    bmpInfoHeader.biHeight = height;
    bmpInfoHeader.biPlanes = 1;
    bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfoHeader.biWidth = width;
    bmpInfoHeader.biSizeImage = width * height * 4;

    // 创建文件并写入BMP文件头和信息头
    FILE* fp = NULL;
    fopen_s(&fp, filename.c_str(), "wb");
    if (!fp) {
        std::cerr << "Error: Cannot open file for writing." << std::endl;
        return 1;
    }

    fwrite(&bmpFileHeader, 1, sizeof(BITMAPFILEHEADER), fp);
    fwrite(&bmpInfoHeader, 1, sizeof(BITMAPINFOHEADER), fp);

    // 获取DIB数据
    unsigned char* bmpData;
    if (!GetDIBits(memDC, memBmp, 0, height, nullptr, (BITMAPINFO*)&bmpInfoHeader, DIB_RGB_COLORS)) {
        std::cerr << "Error: GetDIBits failed." << std::endl;
        fclose(fp);
        return 1;
    }

    // 为DIB分配内存
    bmpData = new unsigned char[bmpInfoHeader.biSizeImage];
    if (!GetDIBits(memDC, memBmp, 0, height, bmpData, (BITMAPINFO*)&bmpInfoHeader, DIB_RGB_COLORS)) {
        std::cerr << "Error: GetDIBits failed." << std::endl;
        delete[] bmpData;
        fclose(fp);
        return 1;
    }

    // 写入DIB数据
    fwrite(bmpData, 1, bmpInfoHeader.biSizeImage, fp);

    // 清理资源
    delete[] bmpData;
    SelectObject(memDC, oldBmp);
    DeleteObject(memBmp);
    DeleteDC(memDC);
    ReleaseDC(nullptr, screenDC);
    fclose(fp);

    return 0;
}

int main() {
    // 保存屏幕左上角100x100像素区域到BMP文件
    if (SaveScreenPartAsBmp(0, 0, 2880, 1920, "screenshot.bmp") != 0) {
        std::cerr << "Failed to save screenshot." << std::endl;
        return 1;
    }

    std::cout << "Screenshot saved successfully." << std::endl;
    return 0;
}