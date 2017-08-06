#pragma once
#include <vector>
#include "SFML\Graphics.hpp"

namespace Helpers {
	//clamps a value between min and max-1
	inline float clamp(float value, float min, float max) {
		if (value < min) {
			return min;
		}
		if (value >= max) {
			return max;
		}

		return value;
	}

	inline float getLength(const sf::Vector2f & v) {
		float length = sqrt(v.x * v.x + v.y * v.y);
		return length;
	}

	inline sf::Vector2f normaliseCopy(const sf::Vector2f & v) {
		sf::Vector2f n;
		float length = getLength(v);
		if (length != 0) {
			n.x = v.x / length;
			n.y = v.y / length;
		}
		return n;
	}

	inline void normalise(sf::Vector2f & v) {
		v = normaliseCopy(v);
	}
	
	inline float randomNumberF(float min, float max) {
		return ((float(rand()) / float(RAND_MAX)) * (max - min)) + min;
	}

	inline int randomNumber(int min, int max) {
		return rand() % (max - min + 1) + min;
	}


	inline void limit(sf::Vector2f& v, float max) {
		if (Helpers::getLength(v) > max) {
			v = Helpers::normaliseCopy(v) * max;
		}
	}



	inline sf::Vector2f lerp(const sf::Vector2f & v1, const sf::Vector2f & v2, float percent) {
		if (percent > 1.f)
			percent = 1.f;
		else if (percent < 0.f)
			percent = 0.f;
		sf::Vector2f v = v2 - v1;
		float distanceAlong = getLength(v) * percent;

		v = v1 + (normaliseCopy(v) * distanceAlong);
		return v;
	}

	inline float lerp(float start, float end, float percent) {
		if (percent > 1.f)
			percent = 1.f;
		else if (percent < 0.f)
			percent = 0.f;
		return start + ((end - start) * percent);
	}

	template<typename T, typename Comparer1, typename Comparer2 = Comparer1>
	inline int binarySearch(const std::vector<T>& v, const T& target, Comparer1 equals, Comparer2 lessThanEquals) {
		int low = 0;
		int high = v.size();
		while (low <= high) {
			int mid = low + (high - low) / 2;
			if (mid == low)
				return (mid < low) ? mid : low;
			if (equals(v[mid], target))
				return mid;
			else if (lessThanEquals(v[mid], target)) 
				low = mid + 1;
			else
				high = mid - 1;
		}
		return -1;
	}
}