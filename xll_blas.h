// xll_blas.h - BLAS routines
#pragma once
#include <memory>
#include "fms_blas/fms_blas.h"
//#define XLOPERX XLOPER
#include "xll/xll/xll.h"

#define CATEGORY "BLAS"

namespace xll {

	template<class T>
	struct allocator {
		static_assert(std::is_same_v<T, double>);
		using value_type = T;

		FPX fp;

		[[nodiscard]] T* allocate(std::size_t n)
		{
			fp.resize((int)n, 1);

			return fp.begin();
		}
		void deallocate(T*, std::size_t)
		{
			fp.resize(0, 0);
		}
	};

	using fpvector = blas::vector_alloc<double, xll::allocator<double>>;

} // namepace xll