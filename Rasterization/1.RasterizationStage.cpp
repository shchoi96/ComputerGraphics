#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

// 2D Cross product
inline
float edgeFunction(const Vec2f& a, const Vec2f& b, const Vec2f& c)
{
	return (c[0] - a[0]) * (b[1] - a[1]) - (c[1] - a[1]) * (b[0] - a[0]);
}

int main(int argc, char** argv)
{
	// Vertex Information
	Vec2f v0 = { 491.407, 411.407 };
	Vec2f v1 = { 148.593, 68.5928 };
	Vec2f v2 = { 148.593, 411.407 };
	Vec3f c0 = { 1, 0, 0 };
	Vec3f c1 = { 0, 1, 0 };
	Vec3f c2 = { 0, 0, 1 };
	// Window size
	const uint32_t w = 512;
	const uint32_t h = 512;
	// Output
	Mat out(w, h, CV_8UC3);
	uchar* framebuffer = (uchar*)out.data;
	memset(framebuffer, 0x0, w * h * 3);

	// Area of a triangle
	float area = edgeFunction(v0, v1, v2);

	for (uint32_t j = 0; j < h; ++j) {
		for (uint32_t i = 0; i < w; ++i) {
			// Pixel Center
			Vec2f p = { i + 0.5f, j + 0.5f };
			// Sigend Area of inner triangle
			float w0 = edgeFunction(v1, v2, p);
			float w1 = edgeFunction(v2, v0, p);
			float w2 = edgeFunction(v0, v1, p);
			// Right side check
			if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
				// Convert Area to barycentric coordinates weight
				w0 /= area;
				w1 /= area;
				w2 /= area;
				// Interpolate a Color
				float r = w0 * c0[0] + w1 * c1[0] + w2 * c2[0];
				float g = w0 * c0[1] + w1 * c1[1] + w2 * c2[1];
				float b = w0 * c0[2] + w1 * c1[2] + w2 * c2[2];
				// [0,1] -> [0, 255]
				framebuffer[3 * (j * w + i) + 2] = (unsigned char)(r * 255);
				framebuffer[3 * (j * w + i) + 1] = (unsigned char)(g * 255);
				framebuffer[3 * (j * w + i) + 0] = (unsigned char)(b * 255);
			}
		}
	}

	imshow("out", out);
	waitKey(0);
	return 0;
}