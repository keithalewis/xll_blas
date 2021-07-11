// xll_blas.cpp - BLAS wrapper
#include "xll_blas.h"

using namespace xll;
using namespace blas;

AddIn xai_blas_vector(
	Function(XLL_HANDLEX, "xll_blas_vector", "\\BLAS.VECTOR")
	.Arguments({
		Arg(XLL_FPX, "array", "is an array of numbers."),
		Arg(XLL_LONG, "_incr", "is the stride of the vector. Default is 1."),
		})
	.Uncalced()
	.FunctionHelp("Return a handle to a BLAS strided vector.")
	.Documentation(R"(
Return a handle to a <code>xll::blas<double></code> vector. The size of
the array is multiplied by the increment.
)")
);
HANDLEX WINAPI xll_blas_vector(const _FPX* pv, LONG dn)
{
#pragma XLLEXPORT
	HANDLEX h = INVALID_HANDLEX;

	try {
		if (dn == 0) {
			dn = 1;
		}

		handle<blas::vector<double>> h_(new blas::vector_array<double>(size(*pv), dn));
		if (h_) {
			h_->copy(size(*pv), pv->array);
		}

		h = h_.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}