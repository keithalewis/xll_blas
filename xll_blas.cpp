// xll_blas.cpp - BLAS wrapper
#include "xll_blas.h"

using namespace xll;
using namespace blas;

#ifdef _DEBUG

Auto<Open> xao_allocator_test([]() {
	try {
		xll::allocator<double> alloc;
		double* p = alloc.allocate(3);
		ensure(p != nullptr);
		ensure(alloc.fp.size() == 3);
		alloc.fp[0] = 1;

		xll::allocator<double> alloc2{ alloc };
		ensure(alloc2.fp.size() == 3);
		ensure(alloc2.fp[0] == 1);

		fpvector fp(3,2);
		ensure(fp.size() == 3);
		ensure(fp.capacity() == 6);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return FALSE;
	}


	return TRUE;
});

#endif // _DEBUG

AddIn xai_blas_vector_(
	Function(XLL_HANDLEX, "xll_blas_vector_", "\\BLAS.VECTOR")
	.Arguments({
		Arg(XLL_FPX, "vector", "is an array of numbers or the size of the vector."),
		Arg(XLL_LONG, "_incr", "is the optional stride of the vector. Default is 1."),
		})
	.Uncalced()
	.FunctionHelp("Return a handle to a BLAS strided vector.")
	.Documentation(R"(
Return a handle to a <code>xll::blas<double></code> vector. The size of
the array is multiplied by the increment.
)")
);
HANDLEX WINAPI xll_blas_vector_(const _FPX* pv, LONG dn)
{
#pragma XLLEXPORT
	HANDLEX h = INVALID_HANDLEX;

	try {
		if (dn != 0 and size(*pv) == 1) {
			int n = static_cast<int>(pv->array[0]);
			handle<blas::vector<double>> h_(new fpvector(n, dn));
			h = h_.get();
		}
		else {
			handle<blas::vector<double>> h_(new fpvector(size(*pv), &pv->array[0], 1));
			h = h_.get();
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}

AddIn xai_blas_vector(
	Function(XLL_FPX, "xll_blas_vector", "BLAS.VECTOR")
	.Arguments({
		Arg(XLL_HANDLEX, "handle", "is a handle to a vector."),
		})
	.FunctionHelp("Return BLAS vector.")
);
_FPX* WINAPI xll_blas_vector(HANDLEX h)
{
#pragma XLLEXPORT
	_FPX* fpx = nullptr;

	try {
		handle<blas::vector<double>> h_(h);
		ensure(h_);
		const auto& v_ = h_.as<fpvector>();
		
		fpx = v_->alloc.fp.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return fpx;
}

AddIn xai_blas_matrix_(
	Function(XLL_HANDLEX, "xll_blas_matrix_", "\\BLAS.MATRIX")
	.Arguments({
		Arg(XLL_FPX, "matrix", "is an array of numbers."),
		Arg(XLL_BOOL, "_trans", "is an optional boolean indicating the matrix is transposed. Default is FALSE."),
		})
		.Uncalced()
	.FunctionHelp("Return a handle to a BLAS matrix.")
	.Documentation(R"(
Return a handle to a BLAS matrix.
)")
);
HANDLEX WINAPI xll_blas_matrix_(const _FPX* pm, bool trans)
{
#pragma XLLEXPORT
	HANDLEX h = INVALID_HANDLEX;

	try {
		handle<blas::matrix<double>> h_(new 
			fpmatrix(pm->rows, pm->columns, &pm->array[0], trans ? CblasTrans : CblasNoTrans));
		h = h_.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}

AddIn xai_blas_matrix(
	Function(XLL_FPX, "xll_blas_matrix", "BLAS.MATRIX")
	.Arguments({
		Arg(XLL_HANDLEX, "handle", "is a handle to a matrix."),
		})
	.FunctionHelp("Return BLAS matrix.")
	.Documentation(R"()")
);
_FPX* WINAPI xll_blas_matrix(HANDLEX h)
{
#pragma XLLEXPORT
	_FPX* fpx = nullptr;

	try {
		handle<blas::matrix<double>> h_(h);
		ensure(h_);
		const auto& m_ = h_.as<fpmatrix>();

		fpx = m_->alloc.fp.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return fpx;
}