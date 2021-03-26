#include "Filter.h"
#include <QImage>
#include <algorithm>




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





QImage Filter::process(const QImage& img) const
{
	QImage result = img;
	maxColor(img);

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

	color.setRgb(static_cast<int>(color.red() * 255 / maxR),
		static_cast<int>(color.green() * 255 / maxG),
		static_cast<int>(color.blue() * 255 / maxB));

	return color;
	

}


QColor MatrixFilter::calcNewPixelColor(const QImage& img, int x, int y) const
{
	float returnR = 0;
	float returnG = 0;
	float returnB = 0;

	int size = mKernel.getSize();
	int radius = mKernel.getRadius();

	for (int i = -radius; i <= radius; ++i)
	{
		for (int j = -radius; j <= radius; ++j)
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