#ifndef SHAKE_H
#define SHAKE_H

#include "cocos2d.h"

class Shake : public cocos2d::ActionInterval
{
public:
	/**
	* Creates the action.
	*
	* @param duration Duration time, in seconds.
	* @param speed Speed of camera moving while shaking
	* @param magnitude The power of shaking
	* @return An autoreleased MoveBy object.
	*/
	static Shake* create(float duration, float speed, float magnitude);

	//
	// Overrides
	//
	virtual Shake* clone() const override;
	virtual void startWithTarget(cocos2d::Node* target) override;

	/**
	* @param time in seconds
	*/
	virtual void update(float time) override;

CC_CONSTRUCTOR_ACCESS:
	Shake() {};
	virtual ~Shake() {};

	bool initWithDuration(float duration, float speed, float magnitude);

protected:
	float _speed;
	float _magnitude;
	float _randomStart;

	void init(void)
	{

	}

	float noise(float x, float y)
	{
		// Reference: http://webstaff.itn.liu.se/~stegu/simplexnoise/simplexnoise.pdf

		// Noise contributions from the three corners
		float n0, n1, n2;

		// Skew the input space to determine which simplex cell we're in
		float s = (x + y) * F2; // Hairy factor for 2D
		int i = fastFloor(x + s);
		int j = fastFloor(y + s);

		float t = static_cast<float>(i + j) * G2;
		float X0 = i - t;	// Unskew the cell origin back to (x,y) space
		float Y0 = j - t;
		float x0 = x - X0; // The x,y distances from the cell origin
		float y0 = y - Y0;

		// For the 2D case, the simplex shape is an equilateral triangle.
		// Determine which simplex we are in.
		int i1, j1; // Offsets for second (middle) corner of simplex in (i,j) coords
		if (x0 > y0)
		{
			// lower triangle, XY order: (0,0)->(1,0)->(1,1)
			i1 = 1;
			j1 = 0;
		}
		else
		{
			// upper triangle, YX order: (0,0)->(0,1)->(1,1)
			i1 = 0;
			j1 = 1;
		}

		// A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and
		// a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where
		// c = (3-sqrt(3))/6

		float x1 = x0 - i1 + G2; // Offsets for middle corner in (x,y) unskewed coords
		float y1 = y0 - j1 + G2;
		float x2 = x0 - 1.0f + 2.0f * G2; // Offsets for last corner in (x,y) unskewed coords
		float y2 = y0 - 1.0f + 2.0f * G2;

		// Work out the hashed gradient indices of the three simplex corners
		int ii = i & 255;
		int jj = j & 255;
		int gi0 = perm[ii + perm[jj]] % 12;
		int gi1 = perm[ii + i1 + perm[jj + j1]] % 12;
		int gi2 = perm[ii + 1 + perm[jj + 1]] % 12;

		// Calculate the contribution from the three corners
		float t0 = 0.5f - (x0 * x0) - (y0 * y0);
		if (t0 < 0.0f)
		{
			n0 = 0.0f;
		}
		else
		{
			t0 *= t0;
			n0 = t0 * t0 * dot(grad3[gi0], x0, y0); // (x,y) of grad3 used for 2D gradient
		}

		float t1 = 0.5f - (x1 * x1) - (y1 * y1);
		if (t1 < 0)
		{
			n1 = 0.0;
		}
		else
		{
			t1 *= t1;
			n1 = t1 * t1 * dot(grad3[gi1], x1, y1);
		}

		float t2 = 0.5f - (x2 * x2) - (y2 * y2);
		if (t2 < 0)
		{
			n2 = 0.0;
		}
		else
		{
			t2 *= t2;
			n2 = t2 * t2 * dot(grad3[gi2], x2, y2);
		}

		// Add contributions from each corner to get the final noise value.
		// The result is scaled to return values in the interval [-1,1].
		return 70.0f * (n0 + n1 + n2);
	}
	
private:
	CC_DISALLOW_COPY_AND_ASSIGN(Shake);

	// Pre calculated values
	static const float F2;	//F2 = 0.5f * (sqrt(3.0f) - 1.0f)) = 0.366025403f
	static const float G2;	//G2 = (3.0f - sqrt(3.0f)) / 6.0f = 0.211324865f

							// Permutation table. This is random numbers between 0 - 255 and it's fixed. 
							// All Simplex Noise implementation uses same table.
							// Originally the size of table is 255, but doubled the size by repeating
							// sequence of numbers to avoid wrapping index at 255 for each look up
	static int perm[512];

	// Gradient direction
	static const int grad3[12][3];

	// Fast floor. Faster than math.floor
	static float fastFloor(float value)
	{
		return value > 0 ? static_cast<int>(value) : static_cast<int>(value) - 1;
	}

	// Dot product
	static float dot(const int* grad, const float x, const float y)
	{
		return grad[0] * x + grad[1] * y;
	}
};


#endif