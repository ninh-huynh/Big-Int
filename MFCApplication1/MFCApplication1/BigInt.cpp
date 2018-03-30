#include "stdafx.h"
#include "BigInt.h"





/**
* Hàm đặt giá trị các bit cho 1 số nguyên lớn.
* Chuyển số 1 nguyên lớn hệ 10 thành các bit trong bộ nhớ
*
* @dec_string chuỗi ký tự chứa số nguyên lớn
*/
void BigInt::set_bit(std::wstring&& dec_string)
{

	// đánh dấu nếu số âm
	const bool negative = dec_string[0] == '-';
	// đếm số chữ số
	short nDigits = negative ? dec_string.length() - 1 : dec_string.length();

	short nBits = 0;
	const short last_digit_index = dec_string.length() - 1;
	short first_digit_index = negative ? 1 : 0;
	memset(m_bits, 0, MAX_BYTES);

	while (nDigits > 0)
	{
		if ((dec_string[last_digit_index] - '0') % 2 == 1)
			push_back(1, nBits);
		else
			push_back(0, nBits);

		// cập nhật thương số
		int i = last_digit_index;
		while (i >= first_digit_index)
		{
			if (i > first_digit_index && (dec_string[i - 1] - '0') % 2 == 1)
			{
				dec_string[i - 1]--;
				dec_string[i] = (dec_string[i] - '0' + 10) / 2 + '0';
			}
			else
			{
				dec_string[i] = (dec_string[i] - '0') / 2 + '0';
			}

			i--;
		}

		if (dec_string[first_digit_index] == '0')
		{
			nDigits--;
			first_digit_index++;
		}

		//std::cout << dec_string.c_str() + first_digit_index << std::endl;
	}
	// bù 2 nếu số âm
	if (negative)
	{
		BYTE carry = 1;
		// đảo tất cả các bit & cộng 1
		std::for_each(std::begin(m_bits), std::end(m_bits),
			[&carry](BYTE& byte) -> void
		{
			BYTE temp = byte ^ UCHAR_MAX;
			byte = temp + carry;
			carry = temp == UCHAR_MAX && carry == 1 ? 1 : 0;
		});
	}
}

/**
* hàm thêm 1 bit vào đầu dãy bit (MSB)
*
* @bit giá trị bit cần thêm (0 hoặc 1)
* @nBits số bit dãy đang có
*/
void BigInt::push_back(const short bit, short& nBits)
{
	const size_t block_size = sizeof(*m_bits) * 8;
	m_bits[nBits / block_size] |= (bit << (nBits % block_size));
	nBits++;
}

/**
* Hàm chuyển dãy bit trong bộ nhớ của số nguyên lớn thành hệ 10.
* Kết quả được chứa trong chuỗi.
*
* @return chuỗi chứa dãy số nguyên lớn hệ 10
*/
std::wstring BigInt::get_dec_string() const
{
	std::wstring res(L"0");

	for (int i = MAX_BYTES - 2; i >= 0; --i)
	{
		const auto byte_value = static_cast<BYTE>(m_bits[i]);
		std::wstring temp = std::to_wstring(byte_value*pow(2, 8 * i));
		res = add_dec_string(res, temp.substr(0, temp.length() - 7));
	}

	const auto byte_value = static_cast<char>(m_bits[MAX_BYTES - 1]);
	std::wstring temp = std::to_wstring(byte_value * pow(2, 8 * (MAX_BYTES - 1)));
	res = add_dec_string(res, temp.substr(0, temp.length() - 7));

	const int index = res.find_first_not_of('0');
	if (index == std::wstring::npos)
		return L"0";
	return res.substr(index, res.length() - index);
}

/**
* Hàm cộng 2 số nguyên lớn chứa trong chuỗi.
*
* @dec_string1 chuỗi chứa số nguyên số hạng đầu tiên, có thể âm hoặc dương
* @dec_string2 chuỗi chứa số nguyên số hạng thứ 2, có thể âm hoặc dương
*
* @return chuỗi chứa tổng
*/
std::wstring BigInt::add_dec_string(std::wstring dec_string1, std::wstring dec_string2)
{
	// hàm kiểm tra âm/dương của số nguyên chứa trong chuỗi
	const auto is_negative =
		[](const std::wstring& dec_string) -> bool
	{
		return dec_string[0] == '-';
	};
	// hàm trả về trị tuyệt đối của số nguyên trong chuỗi
	const auto abs =
		[](const std::wstring& dec_string) -> std::wstring
	{
		int index = dec_string.find_first_not_of('-');
		return dec_string.substr(index, dec_string.length() - index);
	};

	if (!is_negative(dec_string1) && !is_negative(dec_string2))			// cả 2 đều dương
	{
		const int max_len = std::max<int>(dec_string1.length(), dec_string2.length());
		// chèn thêm các số 0 ở đầu chuỗi ngắn => đảm bảo chiều dài 2 chuỗi bằng nhau
		if (max_len > dec_string1.length())
			dec_string1.insert(0, max_len - dec_string1.length(), '0');
		if (max_len > dec_string2.length())
			dec_string2.insert(0, max_len - dec_string2.length(), '0');

		std::wstring result(L"0", max_len + 1);
		int carry = 0;
		// thực hiện phép cộng trên 2 chuỗi (toán tiểu học), lưu kết quả vào 'result'
		// thực hiện cộng theo thứ tự tăng dần hệ số => duyệt chuỗi từ sau về đầu
		std::transform(dec_string1.rbegin(), dec_string1.rend(), dec_string2.rbegin(), result.rbegin(),
			[&carry](const char a, const char b) -> char
		{
			int temp = a - '0' + b - '0' + carry;
			carry = temp / 10;
			return temp % 10 + '0';
		});

		result[0] = carry + '0';		// cộng thêm nếu còn nhớ
										// xóa các số 0 thừa ở đầu chuỗi kết quả
		const int index = result.find_first_not_of(L'0');
		if (index != std::wstring::npos)
			return result.substr(index, result.length() - index);

		return result;
	}
	else if (is_negative(dec_string1) && is_negative(dec_string2))		// cả 2 số đều âm
	{
		// -a - b = - (a + b)
		std::wstring temp = add_dec_string(dec_string1.erase(0, 1), dec_string2.erase(0, 1));
		return temp.insert(0, 1, '-');
	}
	else			// 1 âm, 1 dương
	{
		std::wstring abs_dec_string1 = abs(dec_string1);
		std::wstring abs_dec_string2 = abs(dec_string2);
		const int max_len = std::max<int>(abs_dec_string1.length(), abs_dec_string2.length());
		// chèn 0 vào đầu => đảm bảo chiều dài 2 chuỗi = nhau
		if (max_len > abs_dec_string1.length())
			abs_dec_string1.insert(0, max_len - abs_dec_string1.length(), L'0');
		if (max_len > abs_dec_string2.length())
			abs_dec_string2.insert(0, max_len - abs_dec_string2.length(), L'0');
		// xác định kết quả âm hay dương
		const bool negative = (is_negative(dec_string1) && abs_dec_string1 > abs_dec_string2) ||
			(is_negative(dec_string2) && abs_dec_string2 > abs_dec_string1);

		std::wstring result(L"0", std::max<int>(abs_dec_string1.length(), abs_dec_string2.length()) + 1);
		// đảm bảo số bị trừ luôn >= số trừ
		if (abs_dec_string1 < abs_dec_string2)
			std::swap(abs_dec_string1, abs_dec_string2);

		int carry = 0;
		// thực hiện phép trừ 2 chuỗi số dương (toán tiểu học)
		std::transform(abs_dec_string1.rbegin(), abs_dec_string1.rend(), abs_dec_string2.rbegin(), result.rbegin(),
			[&carry](const char a, const char b) -> char
		{
			int temp = a - '0' - (b - '0' + carry);
			if (temp < 0)
			{
				temp += 10;
				carry = 1;
			}
			else
				carry = 0;

			return temp % 10 + '0';
		});

		// thêm dấu '-' nếu đã xác định trước kết quả là âm
		if (negative)
		{
			const int index = result.find_first_not_of('0');
			if (index > 0)
				result[index - 1] = '-';
			else
				result.insert(0, 1, '-');
		}

		// loại bỏ các số 0 thừa ở đầu
		const int index = result.find_first_not_of(L'0');
		if (index == std::wstring::npos)
			return L"0";
		if (index > 0)
			return result.substr(index, result.length() - index);

		return result;
	}
}

BigInt::BigInt(const std::wstring& dec_string)
{
	set_bit(std::wstring(dec_string));
}

BigInt::BigInt(std::wstring&& dec_string)
{
	set_bit(std::move(dec_string));
}

BigInt::BigInt(const wchar_t* dec_string) : BigInt(std::wstring(dec_string)) {}

BigInt::BigInt(const int & num)
{
	set_bit(std::to_wstring(num));
}

bool BigInt::operator!=(const BigInt& other) const
{
	for (int i = 0; i < MAX_BYTES; ++i)
	{
		if (this->m_bits[i] != other.m_bits[i])
			return true;
	}

	return false;
}


bool BigInt::operator==(const BigInt& other) const
{
	for (int i = 0; i < MAX_BYTES; ++i)
	{
		if (this->m_bits[i] != other.m_bits[i])
			return false;
	}

	return true;
}

/**
 * Toán tử nhập
 */
std::wistream& operator >> (std::wistream& inDev, BigInt& num)
{
	std::wstring dec_string;
	inDev >> dec_string;
	num.set_bit(std::move(dec_string));

	return inDev;
}

/**
 * Toán tử xuất
 */
std::wostream& operator<<(std::wostream& outDev, const BigInt& num)
{
	outDev << num.get_dec_string();

	return outDev;
}

/**
 * Toán tử dịch phải
 * 
 * @num số nguyên cần dịch
 * @shift số đơn vị dịch
 * 
 * @return trả về 1 số nguyên kiểu BigInt
 * 
 * - Toán tử dịch phải ta cần chú ý đến dấu của số cần dịch. Cụ thế, ta tiến hành giống như 
 *   toán tử dịch trái ở cả hai trường hợp shift < 8 và shift > 8, nhưng ở đây thứ tự duyệt sẽ là
 *   từ byte cao nhất về byte thấp nhất. Sau đó xét dấu của số ban đầu, nếu là âm thì ta bật 
 *   shift bit cao nhất lên 1.
 */
BigInt operator >> (BigInt num, int shift)
{
	BYTE carry = 0x00;
	BYTE fill_value = 0x00;
	const bool negative = num.m_bits[MAX_BYTES - 1] >> 7;		// kiểm tra số âm

	const int n_byte_shift = shift / CHAR_BIT;
	shift %= 8;

	if (negative)
	{
		carry = (0x7f >> (7 - shift)) & 0xff;		// carry = 
		fill_value = UCHAR_MAX;
	}

	// dịch shift/8 đơn vị byte
	if (n_byte_shift > 0)
	{
		memcpy(num.m_bits, num.m_bits + n_byte_shift, (MAX_BYTES - n_byte_shift) * sizeof(char));
		memset(num.m_bits + MAX_BYTES - n_byte_shift, fill_value, n_byte_shift * sizeof(char));
	}

	// dịch (shift % 8) đơn vị bit
	if (shift > 0)
	{
		for (int i = MAX_BYTES - 1; i >= 0; --i)
		{
			// nhớ các bit được dịch sang byte phía sau
			BYTE temp = (num.m_bits[i] << (CHAR_BIT - shift)) >> (CHAR_BIT - shift);
			// dịch byte hiện tại
			num.m_bits[i] >>= shift;
			num.m_bits[i] |= carry << (CHAR_BIT - shift);
			carry = temp;
		}
	}

	return num;
}

/**
 * hàm chuyển một số nguyên sang chuỗi
 * 
 * @num số nguyên cần chuyển đổi
 * 
 * @return trả về chuỗi chứa số nguyên
 */
std::wstring to_wstring(const BigInt& num)
{
	return num.get_dec_string();
}

// hàm phụ cho toán tử dịch trái
char Nho_Bit_Dich_Trai(char a, int shift)
{
	char tmp = 0;
	int t = 0, s = 7;
	while (shift != t) {
		tmp = tmp << 1;
		tmp = tmp | (a >> (s) & 1);
		s--; t++;
	}
	return tmp;
}
BigInt operator << (BigInt num, int shift)
{

	while (shift > 8) {
		char tmp;
		int i = MAX_BYTES - 2;
		for (; i >= 0; i--)
		{
			tmp = Nho_Bit_Dich_Trai(num.m_bits[i], 8);
			num.m_bits[i + 1] = (num.m_bits[i + 1] << 8) | tmp;
		}
		num.m_bits[0] = num.m_bits[0] << 8;
		shift -= 8;
	}
	char tmp;
	int i = MAX_BYTES - 2;
	for (; i >= 0; i--)
	{
		tmp = Nho_Bit_Dich_Trai(num.m_bits[i], shift);
		num.m_bits[i + 1] = (num.m_bits[i + 1] << shift) | tmp;
	}
	num.m_bits[0] = num.m_bits[0] << shift;
	return num;
}

/**
 * Toán tử cộng 2 số nguyên BigInt
 * 
 * @lhs số hạng trái (left hand side)
 * @rhs số hạng phải (right hand side)
 * 
 * @return trả về một số nguyên kiểu BigInt
 * 
 * ----Thuật toán----
 * - Thực hiện bằng cách cộng lần lượt từng byte của dãy số, dùng 1 bit nhớ lưu lại bit nhớ
 *   khi cộng từng cặp byte (nếu có), bit nhớ sẽ được cộng vào phép cộng của cặp byte tiếp theo.
 *   
 * - Bit nhớ được bật khi tổng của cặp byte vượt quá giới hạn của kiểu unsigned char (>255).
 */
BigInt operator+(const BigInt & lhs, const BigInt & rhs)
{
	bool carry = 0; // bit nhớ, mặc định là 0.
	BigInt result_int;
	for (int i = 0; i < MAX_BYTES; i++)
	{
		WORD temp_num; // lưu tạm kết quả cộng cặp byte thứ i.
		temp_num = lhs.m_bits[i] + rhs.m_bits[i] + carry; // cộng thêm bit nhớ nếu có.
		carry = 0; // cộng xong thì trả lại 0.

		if (temp_num > 255) // tổng vượt quá giới hạn của uchar thì bật bit nhớ.
		{
			carry = 1;
		}
		result_int.m_bits[i] = temp_num; // gán kết quả cuối.
	}
	return result_int;
}

/**
 * Hàm trừ 2 số nguyên kiểu BigInt
 * 
 * @lhs toán hạng trái
 * @rhs toán hạng phải
 * 
 * @reutrn số nguyên kiểu BigInt
 * 
 * -----Thuật toán-----
 * - Tiến hành lấy dãy bit (dùng các toản tử built-int bitwise) của số hạng rhs bù 2, sau đó lấy 
 *   dãy bit của lhs cộng với dãy bit vừa có được. Lưu dãy bit kết quả vào một số nguyên kiểu BigInt
 */
BigInt operator - (const BigInt& lhs, const BigInt& rhs)	// Toán tử -
{
	BigInt kq;
	BigInt Bu_2;

	// Bù 1 của số rhs
	for (int i = 0; i < MAX_BYTES; i++) {
		Bu_2.m_bits[i] = ~(rhs.m_bits[i]);
	}

	// Bù 2 = Bù 1 + 1 của số rhs
	BYTE Nho = 1;
	for (int i = 0; i <MAX_BYTES; i++) {
		WORD tmp1 = Bu_2.m_bits[i] + Nho;
		Bu_2.m_bits[i] = tmp1;
		Nho = tmp1 / (UCHAR_MAX + 1);
	}

	// Cộng lsh với bù 2 của rhs
	BYTE carry = 0x00;
	for (int i = 0; i <MAX_BYTES; i++) {
		WORD tmp2 = lhs.m_bits[i] + Bu_2.m_bits[i] + carry;
		kq.m_bits[i] = tmp2;
		carry = tmp2 / (UCHAR_MAX + 1);
	}
	return kq;
}

/**
 * Hàm chia 2 số BigInt với nhau
 * 
 * @lhs toán hạng trái
 * @rhs toán hạng phải
 * 
 * @return số nguyên kiểu BigInt
 * 
 * Sử dụng thuật toán Booth
 */

BigInt operator/(const BigInt & lhs, const BigInt & rhs)
{
	bool isZero = true;
	for (int i = 0; i < MAX_BYTES; i++)
	{
		if (rhs.m_bits[i] != 0)
		{
			isZero = false;
			break;
		}
	}
	if (isZero)
		throw std::invalid_argument("cannot divide by zero");

	BigInt A, Q, M;
	int k = MAX_BYTES * 8;
	Q = lhs;
	M = rhs;

	if (Q.m_bits[MAX_BYTES - 1] >> 7)
		Q = BigInt(L"0") - Q;

	if (M.m_bits[MAX_BYTES - 1] >> 7)
		M = BigInt(L"0") - M;

	for (int i = 0; i < MAX_BYTES; i++)
		A.m_bits[i] = 0;

	while (k)
	{
		//Dịch trái từng bit trên mảng bit [A, Q]
		unsigned char msbQ = Q.m_bits[MAX_BYTES - 1] >> 7;
		A = A << 1;
		A.m_bits[0] = A.m_bits[0] | msbQ;
		Q = Q << 1;

		A = A - M;
		if ((A.m_bits[MAX_BYTES - 1] >> 7) & 1)
		{
			A = A + M;
		}
		else
			Q.m_bits[0] = Q.m_bits[0] | 1;		//Bật bit 0 (Q0 = 1)

		k--;
	}

	//Nếu số chia và số bị chia khác dấu, đổi dấu thương
	if ((lhs.m_bits[MAX_BYTES - 1] >> 7) != (rhs.m_bits[MAX_BYTES - 1] >> 7))
		return BigInt(L"0") - Q;
	return Q;
}

/**
 * Hàm chia dư 2 số nguyên
 * 
 * @lhs toán hạng trái
 * @rhs toán hạng phải
 * 
 * @return số nguyên kiểu BigInt
 * 
 * Sử dụng thuật toán Booth (trả về phần dư)
 */
BigInt operator%(const BigInt & lhs, const BigInt & rhs)
{
	bool isZero = true;
	for (int i = 0; i < MAX_BYTES; i++)
	{
		if (rhs.m_bits[i] != 0)
		{
			isZero = false;
			break;
		}
	}
	if (isZero)
		throw std::invalid_argument("cannot divide by zero");

	BigInt A, Q, M;
	int k = MAX_BYTES * 8;
	Q = lhs;
	M = rhs;

	if (Q.m_bits[MAX_BYTES - 1] >> 7)
		Q = BigInt(L"0") - Q;

	if (M.m_bits[MAX_BYTES - 1] >> 7)
		M = BigInt(L"0") - M;

	for (int i = 0; i < MAX_BYTES; i++)
		A.m_bits[i] = 0;

	while (k)
	{
		//Dịch trái từng bit trên mảng bit [A, Q]
		unsigned char msbQ = Q.m_bits[MAX_BYTES - 1] >> 7;
		A = A << 1;
		A.m_bits[0] = A.m_bits[0] | msbQ;
		Q = Q << 1;

		A = A - M;
		if ((A.m_bits[MAX_BYTES - 1] >> 7) & 1)
		{
			A = A + M;
		}
		else
			Q.m_bits[0] = Q.m_bits[0] | 1;		//Bật bit 0 (Q0 = 1)

		k--;
	}

	//Nếu số chia và số bị chia khác dấu, đổi dấu phần dư
	if ((lhs.m_bits[MAX_BYTES - 1] >> 7) != (rhs.m_bits[MAX_BYTES - 1] >> 7))
		return BigInt(L"0") - A;
	return A;
}

/**
 * Hàm nhân 2 số nguyên BigInt
 * 
 * @lhs toán hạng trái
 * @rhs toán hạng phải
 * 
 * @return số nguyên kiểu BigInt
 * 
 * Sử dụng thuật toán Booth
 */
BigInt operator*(const BigInt & lhs, const BigInt & rhs)
{
	BigInt A, M, Q;
	M = lhs;
	Q = rhs;
	for (int i = 0; i < MAX_BYTES; i++)
		A.m_bits[i] = 0;
	int k = MAX_BYTES * 8;
	char Q_1, Q_0 = 0;
	while (k)
	{
		//Lấy 2 bit cuối Q_0, Q_1
		Q_1 = k == MAX_BYTES * 8 ? 0 : Q_0;
		Q_0 = Q.m_bits[0] & 1;

		if (Q_0 == 1 && Q_1 == 0)
			A = A - M;
		else if (Q_0 == 0 && Q_1 == 1)
			A = A + M;

		//Dịch phải mảng bit [A, Q, Q_1]
		unsigned char lsbA = A.m_bits[0] & 1;
		A = A >> 1;
		Q = Q >> 1;
		if (lsbA == 1)
			Q.m_bits[MAX_BYTES - 1] = Q.m_bits[MAX_BYTES - 1] | (1 << 7); //  bật bit nhớ
		else
			Q.m_bits[MAX_BYTES - 1] = Q.m_bits[MAX_BYTES - 1] & (~(1 << 7)); // tắt bit nhớ
		k--;
	}

	//// Nếu A khác 0 hoặc A khác -1 thì kết quả nhân tràn số
	//for (int i = 0; i < MAX_BYTES - 1; i++)
	//	if (A.m_bits[i] != A.m_bits[i + 1])
	//		throw std::overflow_error("Overflow!");
	return Q;
}

/**
 * Toán tử AND
 * 
 * @lhs toán hạng trái
 * @rhs toán hạng phải
 * 
 * @return số nguyên kiểu BigInt
 */
BigInt operator&(const BigInt & lhs, const BigInt & rhs)
{
	BigInt ResultInt;
	for (int i = 0; i < MAX_BYTES; i++)
	{
		ResultInt.m_bits[i] = lhs.m_bits[i] & rhs.m_bits[i];
	}
	return ResultInt;
}

/**
* Toán tử OR
*
* @lhs toán hạng trái
* @rhs toán hạng phải
*
* @return số nguyên kiểu BigInt
*/
BigInt operator|(const BigInt & lhs, const BigInt & rhs)
{
	BigInt ResultInt;
	for (int i = 0; i < MAX_BYTES; i++)
	{
		ResultInt.m_bits[i] = lhs.m_bits[i] | rhs.m_bits[i];
	}
	return ResultInt;
}

/**
* Toán tử XOR
*
* @lhs toán hạng trái
* @rhs toán hạng phải
*
* @return số nguyên kiểu BigInt
*/
BigInt operator^(const BigInt & lhs, const BigInt & rhs)
{
	BigInt ResultInt;
	for (int i = 0; i < MAX_BYTES; i++)
	{
		ResultInt.m_bits[i] = lhs.m_bits[i] ^ rhs.m_bits[i];
	}
	return ResultInt;
}

/**
* Toán tử NOT
*
* @lhs toán hạng
*
* @return số nguyên kiểu BigInt
*/
BigInt operator~(const BigInt & lhs)
{
	BigInt ResultInt;
	for (int i = 0; i < MAX_BYTES; i++)
	{
		ResultInt.m_bits[i] = ~lhs.m_bits[i];
	}
	return ResultInt;
}

/**
 * Hàm chuyển số nguyên lớn hệ 10 sang hệ 2
 * 
 * @num số nguyên hệ 10 cần chuyển
 * 
 * @return chuỗi chứa dãy bit nhị phân
 * 
 * ---------Thuật toán-----------
 * 
 * - Áp dụng thuật toán chia 2 (kiểu BigInt modulo BigInt) lấy số dư (có 2 TH: 0 hoặc 1) và 
 *   thêm phần tử đó vào đầu chuỗi kết quả. Nếu là số âm thì xử lý chuỗi thành chuỗi bù 2 rồi mới trả về kết quả.
 *   
 * - Cụ thể:
 *    + Kiểm tra xem số là dương hay âm? Nếu âm thì đảo dấu.
 *    + Thực hiện chia 2 lấy số dư và thêm số dư vào đầu chuỗi
 *    + Nếu số âm thì tiến hành đổi qua bù 2:
 *        * Đảo bit toàn bộ phần tử trong chuỗi.
 *        * Tiến hành cộng chuỗi bit lên 1 đơn vị.
 *    + Kết quả được trả về chuỗi đã xử lý xong.
 */
std::wstring DecToBin(BigInt num)
{
	//Tìm vị trí bit 1 đầu tiên trong chuỗi
	int pos = -1;
	for (int i = MAX_BYTES - 1; i >= 0 && pos == -1; i--)
		for (int j = CHAR_BIT - 1; j >= 0 && pos == -1; j--)
			if ((num.m_bits[i] >> j) & 1 == 1)
				pos = i * CHAR_BIT + j;

	//Nếu không tìm thấy thì trả về "0"
	if (pos == -1)
		return L"0";

	std::wstring binStr;
	binStr.resize(pos + 1, L'0');
	for (int i = 0; i < pos + 1; i++)
		if ((num.m_bits[i / CHAR_BIT] >> (i % CHAR_BIT)) & 1 == 1)
			binStr[i] = L'1';

	std::reverse(binStr.begin(), binStr.end());
	return binStr;
}

/**
 * Hàm chuyển số nhị phân sang số nguyên hệ 10
 * 
 * @bin_string chuỗi chứa dãy bit nhị phân
 * 
 * @return số nguyên BigInt
 * 
 * 
 * ------Thuật toán-------
 * 
 * - Áp dụng thuật toán chuyển đổi hệ 2 sang 10, lần lượt tính giá trị với từng bit (theo hệ 10) 
 *   trong bin_string sau đó cộng dồn vào biến kết quả kiểu BigInt.
 */
BigInt BinToDec(const std::wstring& bin_string)
{
	BigInt res = 0;
	std::wstring temp = L"0";

	int j = 0, i;
	for (i = bin_string.length() - 1; i >= 0; i--) {
		if (bin_string[i] == L'1') {
			temp = std::to_wstring(pow(2, j));
			temp = temp.substr(0, temp.length() - 7);
			res = res + BigInt(temp);
		}
		j++;
	}

	return res;
}

/*@
 * Hàm chuyển số nguyên hệ 10 sang hệ 16
 * 
 * @num số nguyên hệ 10 cần chuyển
 * 
 * @return chuỗi chứa số hệ 16
 * 
 * --------Thuật toán---------
 * - Dùng 2 hàm đã viết: DecToBin và BinToHex
 */
std::wstring DecToHex(BigInt num)
{
	std::wstring bin_string = DecToBin(num);
	return BinToHex(bin_string);
}

/**
 * Hàm chuyển số hệ 16 sang 10
 * 
 * @hex_string chuỗi chứa số hệ 16
 * 
 * @return số nguyên BigInt
 * 
 * --------Thuật toán---------
 * - Áp dụng thuật toán chuyển đổi hệ 2 sang 10, lần lượt tính giá trị với từng ký tự hexa 
 *   (theo hệ 10) trong hex_string sau đó cộng dồn vào biến kết quả kiểu BigInt.
 */
BigInt HexToDec(const std::wstring& hex_string)
{
	BigInt res = 0;
	std::wstring temp = L"0";
	int l = hex_string.length() - 1;
	int i = 0;

	for (; i < hex_string.length(); i++) {
		if (hex_string[i] >= L'A'&&hex_string[i] <= L'F') {
			temp = std::to_wstring(hex_string[i] - 55);
			res = res + BigInt(temp << (4 * l));
			l--;
		}
		else if (hex_string[i] >= L'a'&&hex_string[i] <= L'f') {
			temp = std::to_wstring(hex_string[i] - 87);
			res = res + BigInt(temp << (4 * l));
			l--;
		}
		else {
			temp = std::to_wstring(hex_string[i] - 48);
			res = res + BigInt(temp << (4 * l));
			l--;
		}
	}

	return res;
}

/// Cách 2: dựa vào các hàm đã viết
/*BigInt HexToDec(const std::wstring& hex_string)
{
BigInt res("0");
std::wstring bin_string = HexToBin(hex_string);
res = BinToDec(bin_string);
return res;
}*/


/**
 * Hàm chuyển số nhị phân sang thập lục phân
 * 
 * @bin_string chuỗi ký tự chứa dãy nhị phân cần chuyển
 * 
 * @return chuỗi ký tự chứa số hệ 16
 * 
 * -------Thuật toán--------
 * - Áp dụng theo kiểu xử lý chuỗi, tuần tự từ phải qua trái với từng nhóm 4 bits 
 *   đổi thành giá trị quy đổi qua ký tự hex tương ứng rồi thêm vào đầu chuỗi kết quả.
 *   
 * - Cụ thể
 *   + Tạo ra 1 chuỗi mẫu các ký tự từ 0…F của số hex ứng với giá trị từ 0 
 *     đến 15 trong số thập phân. (Giá trị 0 ứng với ký tự “thứ 0” của mẫu, 
 *     giá trị F ứng với ký tự “thứ 15” của mẫu).
 *   + Từ trái qua phải với mỗi bit thì cộng vào một biến tạm theo công 
 *     thức 2^(vị trí mod 4) đến khi biến đếm chia hết cho 4 (thì biến tạm đã mang 
 *     giá trị của nhóm 4 bits) thì quy đổi ra kí tự hex bằng cách dò vị trí 
 *     của nó so với chuỗi mẫu. Thêm ký tự hex vào chuỗi và gán giá trị 
 *     biến tạm bằng 0.
 *   + Kết thúc vòng lặp mà giá trị biến tạm vẫn lớn hơn 0 tức là các bit 
 *     chưa quy ra không chia hết cho 4 thì tiếp tục quy giá trị biến tạm ra ký tự hex.
 *   + Kết quả được trả về cho chuỗi đã xử lý xong.
 */
std::wstring BinToHex(const std::wstring& bin_string) //Minh Nhật code
{
	//Trường hợp số 0
	if (bin_string == L"0") return L"0";

	std::wstring hex_string;
	hex_string.reserve(MAX_BYTES*CHAR_BIT / 4);
	std::wstring sample(L"0123456789ABCDEF"); //Mẫu ký tự hex

	char bit_pos = 0, value = 0; //Ý tưởng: Gom nhóm 4 bit thành 1 ký tự hex từ phải sang trái

	while (bit_pos < bin_string.length())
	{
		value += (bin_string[bin_string.length() - bit_pos - 1] - '0')*(char)pow(2, bit_pos % 4);
		if (bit_pos % 4 == 3) //Mỗi 4 bit tiếng hành chuyển thành 1 ký tự hex rồi xóa giá trị nhớ value
		{
			hex_string.insert(0, 1, sample[value]);
			value = 0;
		}
		bit_pos++;
	}
	if (value != 0) hex_string.insert(0, 1, sample[value]); //Nếu số bit của chuỗi k chia hết cho 4 tức là còn ít hơn 4 bit -> tiếp tục chuyển giá trị nhớ value thành ký tự hex

	return hex_string;
}

/**
 * Hàm chuyển số hệ 16 sang hệ 2
 * 
 * @hex_string chuỗi ký tự chứa số hệ 16
 * 
 * @return chuỗi ký tự chứa dãy bit nhị phân
 * 
 * ------Thuật toán--------
 * 
 * - Giải quyết theo kiểu xử lý chuỗi, tuần tự từ phải qua trái với mỗi ký tự của 
 *   chuỗi đổi qua 1 nhóm 4 bits (giá trị tối thiểu là 0000 (ứng với 0), tối đa 1111 (ứng với 15))
 *   
 * - Cụ thể
 *   + Tạo ra 1 chuỗi mẫu các ký tự từ 0…F của số hex ứng với giá trị từ 0 
 *     đến 15 trong số thập phân. (Giá trị 0 ứng với ký tự “thứ 0” của mẫu, 
 *     giá trị F ứng với ký tự “thứ 15” của mẫu).
 *   + Tiến hành từ phải qua trái với mỗi ký tự trong chuỗi đầu vào cho đến ký tự “thứ 1”:
 *     * Đối chiếu ký tự với mẫu, thứ tự của ký tự trong mẫu là giá trị của ký tự đó.
 *     * Dùng thuật toán chia 2 lấy dư để chuyển đổi sang nhị phân  
 *     * Làm đầy bit 0 nếu không đủ 4 bit (vì giá trị hex từ 0 đến 7 sẽ không có đủ 4 bit)
 *   + Ký tự cuối cùng của chuỗi đầu vào tiến hành các bước nhưng ở trên 
 *     nhưng KHÔNG làm đầy bit 0 cho đủ 4 bit.
 *   + Kết quả được trả về chuỗi đã xử lý xong.
 */
std::wstring HexToBin(const std::wstring& hex_string) //Minh Nhật code
{
	if (hex_string == L"0") return L"0";

	std::wstring bin_string;
	bin_string.reserve(MAX_BYTES*CHAR_BIT);
	std::wstring sample(L"0123456789ABCDEF"); //mẫu ký tự hex

	char ch_pos = 0, value;
	while (ch_pos < hex_string.length() - 1)
	{
		value = sample.find_first_of(hex_string[hex_string.length() - ch_pos - 1]);

		char bit_fill = 4; //số bit 0 bù cho đủ nhóm 4-bit
		while (value != 0)
		{
			bin_string.insert(0, 1, value % 2 + L'0');
			value /= 2;
			bit_fill--;
		}
		bin_string.insert(0, bit_fill, L'0'); //làm cho đủ 4-bit

		ch_pos++;
	}

	//xử lý ký tự đầu tiên
	value = sample.find_first_of(hex_string[0]);
	while (value != 0)
	{
		bin_string.insert(0, 1, value % 2 + L'0');
		value /= 2;
	}

	return bin_string;
}

/**
 * Hàm chuyển tổng hợp giữa các hệ 2, 10, 16
 * 
 * @number chuỗi chứa số cần chuyển
 * @old_base hệ ban đầu
 * @new_base hệ sau khi chuyển
 * 
 * @return chuỗi chứa kết quả
 */
std::wstring ConvertBase(std::wstring number, int old_base, int new_base)
{
	switch (old_base)
	{
	case 2:
		switch (new_base)
		{
		case 2:
			return number;
		case 10:
			return to_wstring(BinToDec(number));
		case 16:
			return BinToHex(number);
		default: ;
		}
	case 10:
		switch (new_base)
		{
		case 2:
			return DecToBin(number);
		case 10:
			return number;
		case 16:
			return DecToHex(number);
		default: ;
		}
	case 16:
		switch (new_base)
		{
		case 2:
			return HexToBin(number);
		case 10:
			return to_wstring(HexToDec(number));
		case 16:
			return number;
		default: ;
		}
	default: ;
	}
	return {};
}
