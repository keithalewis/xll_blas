// xll_blas.h - BLAS routines
#pragma once
#include <memory>
#include "fms_blas/fms_blas.h"
//#define XLOPERX XLOPER
#include "xll/xll/xll.h"

#define CATEGORY "BLAS"

namespace xll {

	// Use FPX as backing store
	template<class T>
	struct allocator {
		static_assert(std::is_same_v<T, double>);
		using value_type = T;

		FPX fp;

		[[nodiscard]] T* allocate(std::size_t n)
		{
			ensure(n < INT_MAX);

			fp.resize(static_cast<int>(n), 1);

			return fp.begin();
		}
		void deallocate(T*, std::size_t)
		{
			fp.resize(0, 0);
		}
	};

	using fpvector = blas::vector_alloc<double, xll::allocator<double>>;
	using fpmatrix = blas::matrix_alloc<double, xll::allocator<double>>;

	blas::vector<double> vector(_FPX* pv)
	{
		return blas::vector<double>(size(*pv), pv->array, 1);
	}
	blas::matrix<double> matrix(_FPX* pm)
	{
		return blas::matrix<double>(pm->rows, pm->columns, pm->array);
	}

	// return underlying FP pointer or nullptr
	inline _FPX* vector_handle(_FPX* pv)
	{
		if (size(*pv) == 1) {
			xll::handle<blas::vector<double>> v_(pv->array[0]);
			if (v_) {
				auto v = v_.as<fpvector>();

				return v ? v->alloc.fp.get() : nullptr;
			}
		}

		return nullptr;
	}

	// return underlying FP pointer or nullptr
	inline _FPX* matrix_handle(_FPX* pv)
	{
		if (size(*pv) == 1) {
			xll::handle<blas::matrix<double>> v_(pv->array[0]);
			if (v_) {
				auto v = v_.as<fpmatrix>();

				return v ? v->alloc.fp.get() : nullptr;
			}
		}

		return nullptr;
	}


} // namepace xll