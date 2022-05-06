//refer from VanBD2
#include<iostream>
#include<cstring>
using namespace std;

struct bitMapFileHeader // giữ tiêu đề file bitmap
{
    unsigned char bfType[2];        // chỉ định loại file. Trường này ktra xem có đúng là tệp .bmp ko? Nếu có trường này sẽ nhận giá trị 0x4D42
    unsigned char bfSize[4];        // chỉ định kích cỡ theo byte của file bitmap
    unsigned char bfReserved1[2];   // dự trữ ; phải là 0
    unsigned char bfReserved2[2];   // dự trữ ; phải là 0
    unsigned char bfOffset[4];      // chỉ định độ lệch theo byte từ bitMapFileHeader tới bitmap bit
};

struct bitMapInfoHeader // giữ tiêu đề thông tin bitmap
{
    unsigned long biSize;               // chỉ định số byte theo yêu cầu của struct
    unsigned long biWidth;              // chỉ định độ rộng theo pixel
    unsigned long biHeight;             // chỉ định chiều cao theo pixel
    unsigned short biPlanes;            // chỉ định số lượng mặt phẳng màu, phải là 1
    unsigned short biBitsperPixel;      // chỉ định số lượng bit trên mỗi pixel
    unsigned long biCompression;        // chỉ định loại nén
    unsigned long biImageSize;          // kích cỡ image theo byte
    unsigned long biX_PixelsPerMeter;   // số lượng pixel trên mét trên trục x
    unsigned long biY_PixelsPerMeter;   // số lượng pixel trên mét trên trục y
    unsigned long biColorsTable;        // số lượng màu sử dụng bởi bitmap
    unsigned long biColorsImportant;    // số lượng màu quan trọng
};

struct Pixel
{
    unsigned char B;
    unsigned char R;
    unsigned char G;
};


bool bmp_reverse(const char* bmp_path, const char* reverse_bmp_path) {
    FILE* f;                    // con trỏ file 
    bitMapFileHeader fileHeader;    
    bitMapInfoHeader infoHeader;    
    unsigned char* dataImage;   // lưu giữ liệu image 

    f = fopen(bmp_path, "rb");  // mở tệp ở đường dẫn bmp_path ở chế độ đọc nhị phân (rb: read binary)
    if (!f) {
        cout << "File is not open" << endl;
        return false;
    }

    fread(&fileHeader, sizeof(bitMapFileHeader), 1, f); // đọc bitMapFileHeader
    if (fileHeader.bfType[0] != 0x42 || fileHeader.bfType[1] != 0x4D) {
        cout << "This is not file BM" << endl;
        fclose(f);
        return false;
    }

    fread(&infoHeader, sizeof(bitMapInfoHeader), 1, f);
    int padding = (infoHeader.biWidth * 3) % 4; // ?? padding
    int sizeOfImage = infoHeader.biWidth * infoHeader.biHeight * 3 + padding * infoHeader.biHeight;

    dataImage = new unsigned char[sizeOfImage];
    if (!dataImage) {        
        //delete[] dataImage; 
        fclose(f);
        return false;
    }
    memset(dataImage, 0, sizeOfImage);
    fread(dataImage, sizeOfImage, 1, f);
    if (!dataImage) {
        fclose(f);
        return false;
    }
    fclose(f);

    f = fopen(reverse_bmp_path, "wb");  // mở file chế độ ghi nhị phân (wb: write binary)
    if (!f) {
        cout << "File is not open" << endl;
        return false;
    }
    fwrite(&fileHeader, sizeof(bitMapFileHeader), 1, f);
    fwrite(&infoHeader, sizeof(bitMapInfoHeader), 1, f);

    Pixel* pixel = new Pixel[infoHeader.biWidth * infoHeader.biHeight];
    if (!pixel) {
        //delete[] pixel; 
        fclose(f);
        return false;
    }
    memset(pixel, 0, infoHeader.biWidth * infoHeader.biHeight);
    // chuyển mảng BitMap vào mảng Pixel đồng thời Reverse 
    for (int y = infoHeader.biHeight - 1; y >= 0; y--) {
        for (int x = 0; x < infoHeader.biWidth; x++) {
            pixel[y * infoHeader.biWidth + x].B = *(dataImage++);
            pixel[y * infoHeader.biWidth + x].R = *(dataImage++);
            pixel[y * infoHeader.biWidth + x].G = *(dataImage++);
        }
        dataImage = dataImage + padding;
    }

    unsigned char* data_reverse = new unsigned char[sizeOfImage];
    if (!data_reverse) {
        //delete[] data_reverse; 
        fclose(f);
        return false;
    }
    memset(data_reverse, 0, sizeOfImage);
    unsigned char* temp = data_reverse;
    // Chuyển mảng Pixel vào mảng BitMap
    for (int y = 0; y < infoHeader.biHeight; y++) {
        for (int x = 0; x < infoHeader.biWidth; x++) {
            *(temp++) = pixel[y * infoHeader.biWidth + x].B;
            *(temp++) = pixel[y * infoHeader.biWidth + x].R;
            *(temp++) = pixel[y * infoHeader.biWidth + x].G;
        }
        for (int i = 0; i < padding; i++) {
            *(temp++) = 0;
        }
    }
    
    // REVERSE trực tiếp
    //unsigned char* data_reverse = new unsigned char[sizeOfImage];
    //if (!data_reverse) {
    //    //delete[] data_reverse; 
    //    fclose(f);
    //    return false;
    //}
    //unsigned char* temp = data_reverse;
    //for (int y = infoHeader.biHeight - 1; y >= 0; y--){
    //    for (int x = 0; x < infoHeader.biWidth * 3; x++){
    //        *(temp++) = dataImage[y * (infoHeader.biWidth * 3 + padding) + x];
    //    }
    //    for(int i = 0; i < padding; i++){
    //        *(temp++) = 0;
    //    }
    //}
    
    fwrite(data_reverse, sizeOfImage, 1, f);
    fclose(f);

    /*
    fclose(f) là đã đóng file, giải phóng vùng nhớ nên không cần đoạn dưới để tránh lỗi "app_name.exe has triggered a breakpoint"
    delete[] dataImage;
    delete[] pixel;
    delete[] data_reverse;*/
    return true;
}

int main() {
    if (bmp_reverse("D:\\VSProject\\ReverseBitmap\\KFP.bmp", "D:\\VSProject\\ReverseBitmap\\Create1.bmp"))
    {
        cout << "File is create" << endl;
    }
    else {
        cout << "File is not create" << endl;
    }
    return 0;
}