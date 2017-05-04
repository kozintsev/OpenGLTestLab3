#include <cstdlib>
#include <iostream>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>

const double M_PI = 3.14159265358979323846;
//имя файла изображение, нужно поменять
const char filename[] = "D:\\Temp\\0001.bmp"; 

LPCWSTR convert(const char * cs)
{
	size_t size = strlen(cs) + 1;
	wchar_t* wn = new wchar_t[size];
	size_t outSize;
	mbstowcs_s(&outSize, wn, size, cs, size - 1);
	return wn;
}

//Бумер для считанный точек из bmp файла
GLubyte *TxBits;
bool isLoadError = false;

void LoadBmp()
{
	LPCWSTR f = convert(filename);
	GLuint num_tex = 0; // num_tex - номер текстуры
	BITMAPFILEHEADER bfh; //Заголовок BMP файла
	BITMAPINFOHEADER bih; //Заголовок BMP файла
	GLubyte *buf; // массивы для цветов
	DWORD nBytesRead; // сколько данных прочтено с файла
	HANDLE FileHandle; // хендл, открываемого для чтения, файла
	int width, height; // ширина и высота файла
	// открываем файл для чтения
	FileHandle = CreateFile(f, // имя файла
		GENERIC_READ, // открыт для чтения
		0, // совместного использования нет
		NULL, // защита по умолчанию
		OPEN_EXISTING, // только существующий файл
		FILE_ATTRIBUTE_NORMAL, // атрибуты обычного файла
		NULL); // шаблона атрибутов нет
	if (FileHandle == INVALID_HANDLE_VALUE) // если ошибка при открытии то сообщаем
	{
		MessageBox(NULL, convert("Файл отсутствует!"), convert("Ошибка"), MB_OK | MB_ICONINFORMATION);
		isLoadError = true;
		return;
	}
	ReadFile(FileHandle, &bfh, sizeof(bfh), &nBytesRead, NULL); //считываем первый заголовок, он не нужен
	ReadFile(FileHandle, &bih, sizeof(bih), &nBytesRead, NULL); //считываем второй заголовок
	// записываем размер рисунка
	width = bih.biWidth;
	height = bih.biHeight;
	buf = new GLubyte[width*height * 3]; // выделяем память для исходного рисунка
	TxBits = new GLubyte[width*height * 3]; // выделяем память для получаемого рисунка
	ReadFile(FileHandle, buf, (width*height * 3), &nBytesRead, NULL); //считываем данные о цветах пикселей
	// в этом цикле меняем структуру цвета, т.к. BMP файл имеет структуру BGR, а нам нужен RGB
	for (int i = 0; i < width * height * 3; i += 3)
	{
		//переписываем массив цветов
		TxBits[i] = buf[i + 2];
		TxBits[i + 1] = buf[i + 1];
		TxBits[i + 2] = buf[i];
	}
	// тут инициализируется текстура
	glBindTexture(GL_TEXTURE_2D, num_tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, // размер текстуры
		0, GL_RGB, GL_UNSIGNED_BYTE, TxBits);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	CloseHandle(FileHandle); // закрываем файл
	// освобождаем память
	glClear(GL_COLOR_BUFFER_BIT);
	glRasterPos2i(0, 0);
	delete[] buf;
}

void DrawBMP()
{
	glDrawPixels(200, 200, GL_RGB, GL_UNSIGNED_BYTE, TxBits);
}

void init(void)
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glClearColor(0.0, 0.0, 0.0, 0.0);
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, w, 0, h, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
}

void DrawRotated()
{
	GLubyte * OneBit;
	OneBit = new GLubyte[3];
	for (int x = 0; x != 200; x++)
	{
		for (int y = 0; y != 200; y++)
		{
			GLfloat X, Y, X1, Y1, F;
			F = 45.0*(M_PI / 180);
			X = (GLfloat)y;
			Y = (GLfloat)x;
			X1 = X * cos(F) - Y * sin(F);
			Y1 = X * sin(F) + Y * cos(F);
			// смещение
			glRasterPos2i(X1 + 350, Y1);
			int t = x * 200 * 3 + y * 3;
			OneBit[0] = TxBits[t];
			OneBit[1] = TxBits[t + 1];
			OneBit[2] = TxBits[t + 2];
			glDrawPixels(1, 1, GL_RGB, GL_UNSIGNED_BYTE, OneBit);
		}
	}
}

void DrawRotatedtask()
{
	std::cout << "Start rotating" << std::endl;
	GLubyte * OneBit;
	OneBit = new GLubyte[3];
	for (int x = 0; x != 200; x++)
	{
		for (int y = 0; y != 200; y++)
		{
			GLfloat X, Y, X1, Y1, F;
			F = 45.0*(M_PI / 180);
			X = (GLfloat)y;
			Y = (GLfloat)x;
			X1 = ((1.1 * X) - (0.2 * Y)) / ((0.0075 * X) + (0.0005 * Y) + 1);
			Y1 = ((0.1 * X) + (0.9 * Y)) / ((0.0075 * X) + (0.0005 * Y) + 1);
			glRasterPos2i(X1 + 100, Y1 + 350);
			int t = x * 200 * 3 + y * 3;
			OneBit[0] = TxBits[t];
			OneBit[1] = TxBits[t + 1];
			OneBit[2] = TxBits[t + 2];
			glDrawPixels(1, 1, GL_RGB, GL_UNSIGNED_BYTE, OneBit);
		}
	}
	std::cout << "Roteted" << std::endl;
}
void DrawRotatedinterpol()
{
	std::cout << "Start rotating" << std::endl;
	GLubyte * OneBit;
	OneBit = new GLubyte[3];
	GLfloat F = 45.0*(M_PI / 180);

	for (int x = 0; x != 282; x++)
	{
		for (int y = 141; y != 282 + 141; y++)
		{
			GLfloat Y, X, X1, Y1;

			X1 = (GLfloat)x;
			Y1 = (GLfloat)y;
			Y = X1 * cos(-F) - Y1 * sin(-F);
			X = X1 * sin(-F) + Y1 * cos(-F);

			if (Y >= 200 && Y <= 400)
				if (X >= 0 && X <= 200)
				{
					glRasterPos2i(X1 + 200, Y1 + 200);
					int t = (int)X * 200 * 3 + (int)Y * 3;
					OneBit[0] = TxBits[t];
					OneBit[1] = TxBits[t + 1];
					OneBit[2] = TxBits[t + 2];
					glDrawPixels(1, 1, GL_RGB, GL_UNSIGNED_BYTE, OneBit);
				}
		}
	}
	std::cout << "Roteted" << std::endl;
}

void LoadTexture()
{
	LoadBmp();
	if (isLoadError) return;
	DrawBMP();
	DrawRotated();
	DrawRotatedtask();
	DrawRotatedinterpol();
	glFlush();
	delete[] TxBits;
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Drawing a Bitmapped Character");
	init();
	glutDisplayFunc(LoadTexture);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return EXIT_SUCCESS;
}