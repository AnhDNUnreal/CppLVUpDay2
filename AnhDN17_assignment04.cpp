#include <iostream>
using namespace std;

int* get_even_num(int* arr, int* sz)				// arr: [input] vùng nhớ chứa mảng số nguyên ; sz : [output] kích thước của mảng chứa số chẵn
{
	int eInt, eEven = 0;
	int* p = NULL;
	eInt = _msize(arr) / sizeof(int);				// lấy kích thước của mảng arr. _msize: Returns the size of a memory block allocated in the heap
	p = new int[eInt];
	if (p)
	{
		memset(p, 0x00, eInt * sizeof(int));
		for (int i = 0; i < eInt; i++)
			if (arr[i] % 2 == 0)
			{
				p[eEven] = arr[i];
				eEven++;
			}
		*sz = eEven;	// đẩy kích thước mảng số chẵn ra output sz
	}
	return p;   // return con trỏ trỏ đến vùng nhớ chứa mảng số chẵn
	delete[] p;
}

int main()
{
	int* pInt = NULL;		// con trỏ trỏ tới vùng nhớ chứa mảng số nguyên
	int szInt;		    	// kích thước của mảng số nguyên
	int* pEven = NULL;		// con trỏ trỏ tới vùng nhớ chứa mảng số chẵn
	do
	{
		cout << "Nhap kich thuoc mang so nguyen A: ";
		cin >> szInt;
	} while (szInt <= 0);
	// 
	pInt = new int[szInt];	// Allocate memory for array	
	if (pInt)
	{
		memset(pInt, 0x00, szInt * sizeof(int));
		cout << "Nhap cac phan tu cua mang so nguyen:\n";
		for (int idx = 0; idx < szInt; idx++)	
		{
			cout << "A[" << idx << "] = ";
			cin >> pInt[idx];	//hoặc cin >> *(pInt + idx);		
		}		
	}

	pEven = get_even_num(pInt, &szInt);	// Tối ưu biến: sau khi gọi hàm thì szInt sẽ nhận được giá trị mới là kích thước mảng số chẵn
	cout << "\nMang vua nhap vao co: " << szInt << " so chan. Do la so: ";
	for (int i = 0; i < szInt; i++)
		cout << pEven[i] << ", ";
	delete[] pInt;
	delete[] pEven;
}