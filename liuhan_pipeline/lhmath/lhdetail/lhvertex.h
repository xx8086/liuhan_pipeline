#pragma once

namespace lh_pipeline {

	template <typename T, unsigned int N>
	class LhVerTex {
	public:
		typedef T value_type;
	public:
		~LhVerTex() {}
		constexpr LhVerTex() {
			for (int i = 0; i < N; i++) {
				datas[i] = 0;
			}
		}
		constexpr LhVerTex(T scalar) {
			for (int i = 0; i < N; i++) {
				datas[i] = scalar;
			}
		}
		constexpr LhVerTex(LhVerTex<T, N> const & v) {
			for (int i = 0; i < N; i++) {
				datas[i] = v.datas[i];
			}
		}

	public:
		T datas[N];
		unsigned int counts = N;
	};
}