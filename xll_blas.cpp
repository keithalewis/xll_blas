// xll_blas.cpp - BLAS wrapper
#include "xll_blas.h"

using namespace xll;
using namespace blas;

AddIn xai_blas_vector(
	Function(XLL_HANDLEX, "xll_blas_vector", "\\BLAS.VECTOR")
	.Arguments({
		Arg(XLL_LONG, "size", "is the size of the vector."),
		Arg(XLL_LONG, "step", "is the stride of the vector. Default is 1."),
		})
	.Uncalced()
	.FunctionHelp("Return a handle to a BLAS strided vector.")
);
HANDLEX WINAPI xll_blas_vector(LONG n, LONG dn)
{
#pragma XLLEXPORT
	HANDLEX h = INVALID_HANDLEX;

	try {
		handle<blas::vector<double>> h_(new blas::vector_array<double>(n, dn));
		h = h_.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}