///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2019-2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <SciPP/Core/CoreTypes.hpp>

namespace scp
{
	namespace _scp
	{
		#pragma pack(push, 1)
		template<typename TValue, uint8_t Row, uint8_t Col, typename TRow>
		class Mat
		{
			static_assert(Row == 2 || Row == 3 || Row == 4);
			static_assert(Col == 2 || Col == 3 || Col == 4);

			public:

				using IsMat = bool;
				using ValueType = TValue;

				constexpr Mat() = default;
				constexpr Mat(TValue diagValue);
				constexpr Mat(const std::initializer_list<TValue>& values);
				constexpr Mat(const Mat<TValue, Row, Col, TRow>& mat) = default;
				constexpr Mat(Mat<TValue, Row, Col, TRow>&& mat) = default;

				constexpr Mat<TValue, Row, Col, TRow>& operator=(const Mat<TValue, Row, Col, TRow>& mat) = default;
				constexpr Mat<TValue, Row, Col, TRow>& operator=(Mat<TValue, Row, Col, TRow>&& mat) = default;

				constexpr TRow& operator[](uint8_t i);
				constexpr const TRow& operator[](uint8_t i) const;

				constexpr Mat<TValue, Row, Col, TRow>& operator+=(const Mat<TValue, Row, Col, TRow>& mat);
				constexpr Mat<TValue, Row, Col, TRow>& operator-=(const Mat<TValue, Row, Col, TRow>& mat);

				constexpr ~Mat() = default;

			private:

				TRow _rows[Row];
		};
		#pragma pack(pop)

		template<typename TValue, uint8_t Row, uint8_t Col, typename TRow>
		constexpr Mat<TValue, Row, Col, TRow> operator+(const Mat<TValue, Row, Col, TRow>& a, const Mat<TValue, Row, Col, TRow>& b);
		template<typename TValue, uint8_t Row, uint8_t Col, typename TRow>
		constexpr Mat<TValue, Row, Col, TRow> operator-(const Mat<TValue, Row, Col, TRow>& a, const Mat<TValue, Row, Col, TRow>& b);

		template<typename TValue, uint8_t RowA, uint8_t ColARowB, uint8_t ColB, typename TRowA, typename TRowB>
		constexpr Mat<TValue, RowA, ColB, TRowB> operator*(const Mat<TValue, RowA, ColARowB, TRowA>& a, const Mat<TValue, ColARowB, ColB, TRowB>& b);

		// TODO: matrix-vector product, inverse, determinant, unary minus
	}

	template<typename TValue, uint8_t Size, typename TRow>
	constexpr TValue determinant(const _scp::Mat<TValue, Size, Size, TRow>& m);
	template<typename TValue, uint8_t Size, typename TRow>
	constexpr _scp::Mat<TValue, Size, Size, TRow> inverse(const _scp::Mat<TValue, Size, Size, TRow>& m);
}
