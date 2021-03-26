#pragma once
#include <QImage>
#include <math.h>

class Filter
{
protected:
	virtual QColor calcNewPixelColor(const QImage& img, int x, int y) const = 0;

public:
	virtual ~Filter() = default;
	virtual QImage process(const QImage& img) const;

};


class InvertFilter : public Filter
{
	QColor calcNewPixelColor(const QImage& img, int x, int y) const override;
		
};


class GrayScaleFilter : public Filter
{
	QColor calcNewPixelColor(const QImage& img, int x, int y) const override;

};


class SepiaFilter : public Filter
{
	QColor calcNewPixelColor(const QImage& img, int x, int y) const override;

};


class BrightFilter : public Filter
{
	QColor calcNewPixelColor(const QImage& img, int x, int y) const override;
};


class PerfectReflectorFilter : public Filter
{
	QColor calcNewPixelColor(const QImage& img, int x, int y) const override;
};


class Kernel
{
protected:

	std::unique_ptr<float[]>data;
	std::size_t radius;
	std::size_t getLen() const { return getSize() * getSize(); }

public:

	Kernel(std::size_t radius) : radius(radius)
	{
		data = std::make_unique<float[]>(getLen());
	}

	Kernel(const Kernel& other) : Kernel(other.radius)
	{
		std::copy(other.data.get(), other.data.get() + getLen(), data.get());
	}

	std::size_t getRadius() const { return radius; }
	std::size_t getSize() const { return 2 * radius + 1; }
	float operator[](std::size_t id) const { return data[id]; }
	float& operator [] (std::size_t id) { return data[id]; }
};


class MatrixFilter : public Filter
{
protected:
	Kernel mKernel;
	QColor calcNewPixelColor(const QImage& img, int x, int y)const override;

public:
	MatrixFilter(const Kernel& kernel) : mKernel(kernel) {};
	virtual ~MatrixFilter() = default;

};


class BlurKernel : public Kernel
{
public:
	using Kernel::Kernel;
	BlurKernel(std::size_t radius = 2) :Kernel(radius)
	{
		for (std::size_t i = 0; i < getLen(); i++)
			data[i] = 1.0f / getLen();
	}
};

class BlurFilter : public MatrixFilter
{
public:
	BlurFilter(std::size_t radius =1): MatrixFilter(BlurKernel(radius)) {}
};


class GaussianKernel : public Kernel
{
public:
	using Kernel::Kernel;
	GaussianKernel(std::size_t radius = 2, float sigma = 3.f) :Kernel(radius)
	{
		float norm = 0;
		int signed_radius = static_cast<int>(radius);
		for (int x = -signed_radius; x <= signed_radius; ++x)
			for (int y = -signed_radius; y <= signed_radius; ++y)
			{
				std::size_t idx = (x + radius) * getSize() + (y + radius);
				data[idx] = std::exp(-(x * x + y * y) / (2 * sigma * sigma));
				norm += data[idx];
			}

		for (std::size_t i = 0; i < getLen(); ++i)
		{
			data[i] /= norm;
		}

	}
};

class GaussianFilter : public MatrixFilter
{
public:
	GaussianFilter(std::size_t radius = 1) :MatrixFilter(GaussianKernel(radius)) {}
}; 


class SharpnessKernel : public Kernel
{
public:
	using Kernel::Kernel;
	SharpnessKernel(std::size_t radius = 2) :Kernel(radius)
	{
		data[0] = 0; data[1] = -1; data[2] = 0;
		data[3] = -1; data[4] = 5; data[5] = -1;
		data[6] = 0; data[7] = -1; data[8] = 0;
	}
};

class SharpnessFilter : public MatrixFilter
{
	public:
		SharpnessFilter(std::size_t radius = 1) : MatrixFilter(SharpnessKernel(radius)) {}
};


class SobelXKernel : public Kernel
{
public:
	using Kernel::Kernel;
	SobelXKernel(std::size_t radius = 1) :Kernel(radius)
	{
		data[0] = -1; data[1] = -2; data[2] = -1;
		data[3] = 0; data[4] = 0; data[5] = 0;
		data[6] = 1; data[7] = 2; data[8] = 1;
	}
};

class SobelXFilter : public MatrixFilter
{
public:
	SobelXFilter(std::size_t radius = 1) : MatrixFilter(SobelXKernel(radius)) {}
};


class SobelYKernel: public Kernel
{
//public:
//	using Kernel::Kernel;
//	SobelYKernel(std::size_t radius = 1) :Kernel(radius)
//	{
//		data[0] = -1; data[1] = 0; data[2] = 1;
//		data[3] = -2; data[4] = 0; data[5] = 2;
//		data[6] = -1; data[7] = 0; data[8] = 1;
//	}
};

class SobelYFilter : public MatrixFilter
{
//public:
//	SobelYFilter(std::size_t radius = 1) : MatrixFilter(SobelYKernel(radius)) {}
};


