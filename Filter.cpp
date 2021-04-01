#include "Filter.h"
#include <QImage>
#include <algorithm>
#include <cmath>
#define M_PI      3.14159265358979323846




template <class T>
T clamp(T value, T max, T min)
{
	if (value > max)
		return max;
	if (value < min)
		return min;
	return value;
}

int maxR = 0;
int maxG = 0;
int maxB = 0;
int minR = 255;
int minG = 255;
int minB = 255;


void maxColor(const QImage& img)
{
	

	for (int x = 0; x < img.width() / 2; x++)
		for (int y = 0; y < img.height(); y++)
		{
			 QColor color = img.pixelColor(x, y);

			 maxR = std::max(maxR, color.red());
			 maxG = std::max(maxG, color.green());
			 maxB = std::max(maxB, color.blue());

		}
}


void minColor(const QImage& img)
{


	for (int x = 0; x < img.width() / 2; x++)
		for (int y = 0; y < img.height(); y++)
		{
			QColor color = img.pixelColor(x, y);

			minR = std::min(minR, color.red());
			minG = std::min(minG, color.green());
			minB = std::min(minB, color.blue());

		}
}



QImage Filter::process(const QImage& img) const
{
	QImage result = img;
	maxColor(img);
	minColor(img);

	for (int x=0; x < img.width()/2; x++)
		for (int y = 0; y < img.height(); y++)
		{
			QColor color = calcNewPixelColor(img, x, y);
			result.setPixelColor(x, y, color);
		}

	return result;
}


QColor InvertFilter::calcNewPixelColor(const QImage& img, int x, int y) const
{
	QColor color = img.pixelColor(x, y);
	color.setRgb(255 - color.red(), 255 - color.green(), 255 - color.blue());
	return color;

}


QColor GrayScaleFilter::calcNewPixelColor(const QImage& img, int x, int y) const
{
	QColor color = img.pixelColor(x, y);
	int intensity = 0.299 * color.red() + 0.587 * color.green() + 0.114 * color.blue();
	color.setRgb(intensity, intensity, intensity);
	return color;
}


QColor SepiaFilter::calcNewPixelColor(const QImage& img, int x, int y) const
{
	const int K = 20;
	QColor color = img.pixelColor(x, y);
	int intensity = 0.299 * color.red() + 0.587 * color.green() + 0.114 * color.blue();
	color.setRgb(clamp(static_cast<int>(intensity + 2 * K), 255, 0),
		clamp(static_cast<int>(intensity + 0.5 * K), 255, 0),
		clamp(static_cast<int>(intensity - 1 * K), 255, 0));
	return color;

}


QColor BrightFilter::calcNewPixelColor(const QImage& img, int x, int y) const
{
	const int K = 30;
	QColor color = img.pixelColor(x, y);
	color.setRgb(clamp(color.red() + K, 255, 0),
		clamp(color.green() + K, 255, 0),
		clamp(color.blue() + K, 255, 0));
	return (color);
}


QColor PerfectReflectorFilter::calcNewPixelColor(const QImage& img, int x, int y) const
{

	QColor color = img.pixelColor(x, y);

	color.setRgb(clamp(static_cast<int>(color.red() * 255 / maxR), 255, 0),
		clamp(static_cast<int>(color.green() * 255 / maxG), 255, 0),
		clamp(static_cast<int>(color.blue() * 255 / maxB), 255, 0));

	return color;
	

}


QColor WaveFilter::calcNewPixelColor(const QImage& img, int x, int y) const
{
	QColor color = img.pixelColor(x, y);

	if (x > img.width() - int(20 * sin(M_PI * x / 30)) - 1) return color;

	color = img.pixelColor(x + int(20 * sin(M_PI * x / 30)), y);

	return color;
	
}


QColor CarryoverFilter::calcNewPixelColor(const QImage& img, int x, int y)const
{
	if (x >= img.width() - 50) return QColor(0, 0, 0);
	QColor color = img.pixelColor(x + 50, y);

	return color;
}


QColor StretchingHistogramFilter::calcNewPixelColor(const QImage& img, int x, int y)const
{
	QColor color = img.pixelColor(x, y);

	color.setRgb(static_cast<int>((color.red() - minR) * 255 / (maxR-minR)),
		static_cast<int>((color.green() - minG) * 255 / (maxG - minG)),
		static_cast<int>((color.blue() - minB) * 255 / (maxB - minB)));

	return color;

}


QColor MedianFilter::calcNewPixelColor(const QImage& img, int x, int y)const
{
	const int Rad = 2;

	if ((x >= Rad) && (x < img.width() - Rad) && (y >= Rad) && (y < img.height() - Rad))
	{

		const size_t  size = (2 * Rad + 1) * (2 * Rad + 1);
		int arrR[size];
		int arrG[size];
		int arrB[size];
		int k = 0;
		int median = (size - 1) / 2;

		for (int i = x - Rad; i <= x + Rad; i++)
			for (int j = y - Rad; j <= y + Rad; j++)
			{
				QColor color = img.pixelColor(i, j);
				arrR[k] = color.red();
				arrG[k] = color.green();
				arrB[k] = color.blue();
				k++;
			}
		std::sort(arrR, arrR + size);
		std::sort(arrG, arrG + size);
		std::sort(arrB, arrB + size);

		return QColor(arrR[median], arrG[median], arrB[median]);
	}
	else
	{
		QColor color = img.pixelColor(x, y);
		return color;
	}

}


QColor MatrixFilter::calcNewPixelColor(const QImage& img, int x, int y) const
{
	float returnR = 0;
	float returnG = 0;
	float returnB = 0;

	int size = mKernel.getSize();
	int radius = mKernel.getRadius();

	for (int i = -radius; i <= radius; i++)
	{
		for (int j = -radius; j <= radius; j++)
		{
			int idx = (i + radius) * size + j + radius;

			QColor color = img.pixelColor(clamp(x + j, img.width() - 1, 0),
				clamp(y + i, img.height() - 1, 0));

			returnR += color.red() * mKernel[idx];
			returnG += color.green() * mKernel[idx];
			returnB += color.blue() * mKernel[idx];

		}
	}

	return QColor(clamp(returnR, 255.f, 0.f),
		clamp(returnG, 255.f, 0.f),
		clamp(returnB, 255.f, 0.f));

}


QColor EmbossingFilter::calcNewPixelColor(const QImage& img, int x, int y) const
{
	float returnR = 0;
	float returnG = 0;
	float returnB = 0;

	int size = mKernel.getSize();
	int radius = mKernel.getRadius();

	for (int i = -radius; i <= radius; i++)
	{
		for (int j = -radius; j <= radius; j++)
		{
			int idx = (i + radius) * size + j + radius;

			QColor color = img.pixelColor(clamp(x + j, img.width() - 1, 0),
				clamp(y + i, img.height() - 1, 0));

			returnR += color.red() * mKernel[idx];
			returnG += color.green() * mKernel[idx];
			returnB += color.blue() * mKernel[idx];

		}
	}

	return QColor(clamp(returnR+128, 255.f, 0.f),
		clamp(returnG+128, 255.f, 0.f),
		clamp(returnB+128, 255.f, 0.f));
}


QColor SobelFilter::calcNewPixelColor(const QImage& img, int x, int y) const
{
	float returnR1 = 0;
	float returnG1 = 0;
	float returnB1 = 0;

	float returnR2 = 0;
	float returnG2 = 0;
	float returnB2 = 0;

	float returnR = 0;
	float returnG = 0;
	float returnB = 0;

	int size = mKernel.getSize();
	int radius = mKernel.getRadius();

	for (int i = -radius; i <= radius; i++)
	{
		for (int j = -radius; j <= radius; j++)
		{
			int idx = (i + radius) * size + j + radius;

			QColor color = img.pixelColor(clamp(x + j, img.width() - 1, 0),
				clamp(y + i, img.height() - 1, 0));

			returnR1 += color.red() * mKernel[idx];
			returnG1 += color.green() * mKernel[idx];
			returnB1 += color.blue() * mKernel[idx];

			returnR2 += color.red() * mKernel[idx];
			returnG2 += color.green() * mKernel2[idx];
			returnB2 += color.blue() * mKernel2[idx];

		}
	}

	returnR = sqrt(returnR1 * returnR1 + returnR2 * returnR2);
	returnG = sqrt(returnG1 * returnG1 + returnG2 * returnG2);
	returnB = sqrt(returnB1 * returnB1 + returnB2 * returnB2);


	return QColor(clamp(returnR, 255.f, 0.f),
		clamp(returnG, 255.f, 0.f),
		clamp(returnB, 255.f, 0.f));

}
