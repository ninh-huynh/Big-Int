#include "stdafx.h"
#include "Test.h"

BigInt calculate(std::wstring oprt, const BigInt &n1, const BigInt &n2) {
	if (oprt == L"+")
		return n1 + n2;
	else if (oprt == L"-")
		return n1 - n2;
	else if (oprt == L"*")
		return n1 * n2;
	else if (oprt == L"/")
		return n1 / n2;
	else if (oprt == L"^")
		return n1 ^ n2;
	else if (oprt == L"|")
		return n1 | n2;
	else if (oprt == L"&")
		return n1 & n2;
	else if (oprt == L"~")
		return ~n1;

	int n = std::stoi(to_wstring(n2));
	if (oprt == L"<<")
		return n1 << n;
	else if (oprt == L">>")
		return n1 >> n;
}

void Test(std::wifstream& is, std::wofstream& os)
{
	while (!is.eof())
	{
		int p1;
		std::wstring p2, num1, num2, oprt;
		is >> p1 >> p2;

		if (p2 == L"10" || p2 == L"2" || p2 == L"16")
		{
			//Là phép chuyển hệ p1 -> p2
			is >> num1;
			os << ConvertBase(num1, p1, std::stoi(p2)) << '\n';
		}
		else	//Không phải phép chuyển hệ
		{
			//gán lại num1 = p2
			num1 = p2;
			is >> oprt;

			//Toán tử 2 ngôi thì nhập số hạng tiêp theo
			if (oprt != L"~")
				is >> num2;

			try {
				//Chuyển num1, num2 sang hệ 10
				std::wstring dec1, dec2;
				dec1 = ConvertBase(num1, p1, 10);
				dec2 = ConvertBase(num2, p1, 10);

				//Nếu tính được kết quả hợp lệ thì in ra kết quả cùng hệ p1
				BigInt result = calculate(oprt, BigInt(dec1), BigInt(dec2));
				os << ConvertBase(to_wstring(result), 10, p1) << '\n';
			}
			catch (std::exception &except)
			{
				os << except.what() << '\n';
			}
		}
	}

}