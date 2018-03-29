
// MFCApplication1Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "RunFileDlg.h"
#include "afxdialogex.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

// ánh xạ từ ký tự sang các toán tử
std::map<char, BigInt(*)(const BigInt&, const BigInt&)> CalculatorDlg::m_char_to_opr =
		{ {'+', &operator+}, 
		  {'-', &operator-},
		  {'*', &operator*},
		  {'/', &operator/},
		  {'%', &operator%},
		  {'&', &operator&},
		  {'|', &operator|},
		  {'^', &operator^}};
std::deque<char> CalculatorDlg::m_inputs;			// lưu toán tử/dấu ngoặc người dùng nhập vào
std::deque<BigInt> CalculatorDlg::m_operands;			// lưu các toán hạng


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CalculatorDlg dialog


CalculatorDlg::CalculatorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent)
	, m_text_input(_T("0"))
	, m_text_history(_T(""))
	, m_text_hex(_T(""))
	, m_text_bin(_T(""))
	, m_value(0)
	, m_base(DECIMAL)
	, m_prev(NONE)
	, m_parentheses(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_operands.push_back(0);

	// khởi tạo các biến lưu thông tin font
	m_fonts[IDC_EDIT_INPUT] = new CFont();
	m_fonts[IDC_EDIT_HISTORY] = new CFont();
	m_fonts[IDC_EDIT_HEX] = new CFont();
	m_fonts[IDC_EDIT_DEC] = new CFont();
	m_fonts[IDC_EDIT_BIN] = new CFont();

	// tạo font
	m_fonts[IDC_EDIT_INPUT]->CreateFontW(40, 0, 0, 0, 500, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS
		, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	m_fonts[IDC_EDIT_HISTORY]->CreateFontW(30, 0, 0, 0, 500, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS
		, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	m_fonts[IDC_EDIT_HEX]->CreateFontW(25, 0, 0, 0, 500, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS
		, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	m_fonts[IDC_EDIT_DEC]->CreateFontW(25, 0, 0, 0, 500, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS
		, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	m_fonts[IDC_EDIT_BIN]->CreateFontW(25, 0, 0, 0, 500, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS
		, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
}

void CalculatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_INPUT, m_text_input);
	//DDV_MaxChars(pDX, m_text_input, 128);
	DDX_Text(pDX, IDC_EDIT_HISTORY, m_text_history);
	DDX_Text(pDX, IDC_EDIT_HEX, m_text_hex);
	DDX_Text(pDX, IDC_EDIT_DEC, m_text_dec);
	DDX_Text(pDX, IDC_EDIT_BIN, m_text_bin);
}

BEGIN_MESSAGE_MAP(CalculatorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT_INPUT, &CalculatorDlg::OnEnChangeEdit2)
	ON_BN_CLICKED(IDC_BUTTON_C, &CalculatorDlg::OnBnClickedButtonC)
	ON_BN_CLICKED(IDC_BUTTON_NUMBER_1, &CalculatorDlg::OnBnClickedButtonNumber1)
	ON_BN_CLICKED(IDC_BUTTON_NUMBER_2, &CalculatorDlg::OnBnClickedButtonNumber2)
	ON_BN_CLICKED(IDC_BUTTON_NUMBER_3, &CalculatorDlg::OnBnClickedButtonNumber3)
	ON_BN_CLICKED(IDC_BUTTON_NUMBER_4, &CalculatorDlg::OnBnClickedButtonNumber4)
	ON_BN_CLICKED(IDC_BUTTON_NUMBER_5, &CalculatorDlg::OnBnClickedButtonNumber5)
	ON_BN_CLICKED(IDC_BUTTON_NUMBER_6, &CalculatorDlg::OnBnClickedButtonNumber6)
	ON_BN_CLICKED(IDC_BUTTON_NUMBER_7, &CalculatorDlg::OnBnClickedButtonNumber7)
	ON_BN_CLICKED(IDC_BUTTON_NUMBER_8, &CalculatorDlg::OnBnClickedButtonNumber8)
	ON_BN_CLICKED(IDC_BUTTON_NUMBER_9, &CalculatorDlg::OnBnClickedButtonNumber9)
	ON_BN_CLICKED(IDC_BUTTON_NUMBER_0, &CalculatorDlg::OnBnClickedButtonNumber0)
	ON_BN_CLICKED(IDC_RADIO_BIN, &CalculatorDlg::OnBnClickedRadioBin)
	ON_BN_CLICKED(IDC_RADIO_HEX, &CalculatorDlg::OnBnClickedRadioHex)
	ON_BN_CLICKED(IDC_BUTTON_SUBTRACT, &CalculatorDlg::OnBnClickedButtonSubtract)
	ON_BN_CLICKED(IDC_BUTTON_EQUAL, &CalculatorDlg::OnBnClickedButtonEqual)
	ON_BN_CLICKED(IDC_BUTTON_MULTIPLY, &CalculatorDlg::OnBnClickedButtonMultiply)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CalculatorDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DIVIDE, &CalculatorDlg::OnBnClickedButtonDivide)
	ON_BN_CLICKED(IDC_BUTTON_CE, &CalculatorDlg::OnBnClickedButtonCE)
	ON_BN_CLICKED(IDC_BUTTON_MOD, &CalculatorDlg::OnBnClickedButtonModulo)
	ON_BN_CLICKED(IDC_BUTTON_AND, &CalculatorDlg::OnBnClickedButtonAnd)
	ON_BN_CLICKED(IDC_BUTTON_OR, &CalculatorDlg::OnBnClickedButtonOr)
	ON_BN_CLICKED(IDC_BUTTON_XOR, &CalculatorDlg::OnBnClickedButtonXor)
	ON_BN_CLICKED(IDC_BUTTON_NOT, &CalculatorDlg::OnBnClickedButtonNot)
	ON_BN_CLICKED(IDC_BUTTON_SHIFTRIGHT, &CalculatorDlg::OnBnClickedButtonShiftRight)
	ON_BN_CLICKED(IDC_BUTTON_SHIFTLEFT, &CalculatorDlg::OnBnClickedButtonShiftLeft)
	ON_BN_CLICKED(IDC_RADIO_DEC, &CalculatorDlg::OnBnClickedRadioDec)
	ON_BN_CLICKED(IDC_BUTTON_LEFT_PARENTHESE, &CalculatorDlg::OnBnClickedButtonLeftParenthese)
	ON_BN_CLICKED(IDC_BUTTON_RIGHT_PARENTHESE, &CalculatorDlg::OnBnClickedButtonRightParenthese)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_A, &CalculatorDlg::OnBnClickedButtonCharA)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_B, &CalculatorDlg::OnBnClickedButtonCharB)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_C, &CalculatorDlg::OnBnClickedButtonCharC)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_D, &CalculatorDlg::OnBnClickedButtonCharD)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_E, &CalculatorDlg::OnBnClickedButtonCharE)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_F, &CalculatorDlg::OnBnClickedButtonCharF)
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_HELP_ABOUT, &CalculatorDlg::OnHelpAbout)
	ON_COMMAND(ID_FILE_OPEN32771, &CalculatorDlg::OnFileOpen)
	ON_COMMAND(ID_FILE_EXIT, &CalculatorDlg::OnFileExit)
	ON_COMMAND(ID_EDIT_COPY32773, &CalculatorDlg::OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE32774, &CalculatorDlg::OnEditPaste)
	ON_EN_SETFOCUS(IDC_EDIT_INPUT, &CalculatorDlg::OnSetFocusInput)
	ON_EN_SETFOCUS(IDC_EDIT_HISTORY, &CalculatorDlg::OnSetFocusHistory)
	ON_EN_SETFOCUS(IDC_EDIT_HEX, &CalculatorDlg::OnSetFocusHex)
	ON_EN_SETFOCUS(IDC_EDIT_DEC, &CalculatorDlg::OnSetFocusDec)
	ON_EN_SETFOCUS(IDC_EDIT_BIN, &CalculatorDlg::OnSetFocusBin)
	ON_BN_CLICKED(IDC_BUTTON1, &CalculatorDlg::OnBnClickedButtonChangeSign)
	ON_BN_CLICKED(IDC_BUTTON2, &CalculatorDlg::OnBnClickedButtonBackspace)
END_MESSAGE_MAP()


// CalculatorDlg message handlers

BOOL CalculatorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		BOOL bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		//ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	// check 'Dec' Radio Button
	CButton *pBt = (CButton*)this->GetDlgItem(IDC_RADIO_DEC);
	pBt->SetCheck(TRUE);

	// set for edit box
	
	static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_INPUT))->SetFont(m_fonts[IDC_EDIT_INPUT]);
	static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_HISTORY))->SetFont(m_fonts[IDC_EDIT_HISTORY]);
	static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_HEX))->SetFont(m_fonts[IDC_EDIT_HEX]);
	static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_DEC))->SetFont(m_fonts[IDC_EDIT_DEC]);
	static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_BIN))->SetFont(m_fonts[IDC_EDIT_BIN]);

	// disable button
	enable_char_button(FALSE);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CalculatorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CalculatorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CalculatorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BOOL CalculatorDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect r;
	pDC->GetClipBox(&r);

	pDC->FillSolidRect(r, RGB(0xD9, 0xE4, 0xF1)); //windows 7's calculator background
	return TRUE;
}

void CalculatorDlg::OnEnChangeEdit2()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CalculatorDlg::insert_num(int num)
{
	if (m_prev != NUMBER)
	{
		if (m_prev == CALCULATE)
		{
			m_text_history = "";
			std::deque<char>().swap(m_inputs);
			std::deque<BigInt>().swap(m_operands);
		}
		else if (m_prev == UNARY_OPERATOR)
		{
			std::deque<char>().swap(m_inputs);
			std::deque<BigInt>().swap(m_operands);
			m_text_history = "";
		}
		else if (m_prev == RIGHT_PARENTHESES)
			return;
		m_value = 0;
	}

	if (m_inputs.empty() && !m_operands.empty())
		m_operands.pop_back();

	if (m_base == DECIMAL)
	{
		m_value = m_value * 10 + num;
		if (m_text_history.GetLength() == 1 && m_text_history[0] == _T('0'))
			m_text_history = to_wstring(m_value).c_str();
		else
			m_text_history.AppendChar(num + '0');
	}
	else if (m_base == BINARY && (num == 0 || num == 1))
	{
		m_value = (m_value << 1) + num;
		m_text_history.AppendChar(num + '0');
	}
	else if (m_base == HEXADECIMAL)
	{
		m_value = m_value * 16 + num;
		if (num >= 10 && num <= 15)
			m_text_history.AppendChar(num + 'A' - 10);
		else
			m_text_history.AppendChar(num + '0');
	}

	m_prev = NUMBER;

}

void CalculatorDlg::update_text()
{	
	m_text_dec = to_wstring(m_value).c_str();
	m_text_bin = DecToBin(m_value).c_str();
	m_text_hex = DecToHex(m_value).c_str();

	if (m_base == DECIMAL)
		m_text_input = m_text_dec;
	else if (m_base == HEXADECIMAL)
		m_text_input = m_text_hex;
	else
		m_text_input = m_text_bin;


	UpdateData(FALSE);
	scroll_to_end_of_edit_box();
}

void CalculatorDlg::insert_operator(char opr)
{
}

void CalculatorDlg::update_posfix_number()
{
}


bool CalculatorDlg::priority_less_equal_than(char opr1, char opr2)
{
	static std::map<char, int> precedence =
	{ {'*', 5},
	  {'/', 5},
	  {'%', 5},
	  {'+', 4},
	  {'-', 4},
	  {'&', 3},
	  {'|', 2},
	  {'^', 1},
	  {'(', 0},
	  {')', 0}
	};
	                   
	return precedence[opr1] <= precedence[opr2];
}

bool CalculatorDlg::is_operator(char ch)
{
	return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%' || ch == '&' || ch == '|' || ch == '^';
}

void CalculatorDlg::hide_caret()
{
	// hide edit box caret
	// input edit box
	static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_INPUT))->SetFocus();
	static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_INPUT))->HideCaret();
	// history edit box
	static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_HISTORY))->SetFocus();
	static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_HISTORY))->HideCaret();

	static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_HEX))->SetFocus();
	static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_HEX))->HideCaret();

	static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_DEC))->SetFocus();
	static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_DEC))->HideCaret();

	static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_BIN))->SetFocus();
	static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_BIN))->HideCaret();
}

// Scroll về cuối của các text box
void CalculatorDlg::scroll_to_end_of_edit_box()
{
	// input edix box
	HWND hwnd = static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_INPUT))->GetSafeHwnd();
	SendMessageA(hwnd, EM_SETSEL, 0, -1); // Chọn tất cả
	SendMessageA(hwnd, EM_SETSEL, -1, -1);// Bỏ chọn và giữ vị trí ở cuối text box
	SendMessageA(hwnd, EM_REPLACESEL, 0, (LPARAM)""); // Chèn chuỗi vào vị trí cuối và scroll

	// history edit box
	hwnd = static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_HISTORY))->GetSafeHwnd();
	SendMessageA(hwnd, EM_SETSEL, 0, -1); // Chọn tất cả
	SendMessageA(hwnd, EM_SETSEL, -1, -1);// Bỏ chọn và giữ vị trí ở cuối text box
	SendMessageA(hwnd, EM_REPLACESEL, 0, (LPARAM)""); // Chèn chuỗi vào vị trí cuối và scroll

	// hex edit box
	hwnd = static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_HEX))->GetSafeHwnd();
	SendMessageA(hwnd, EM_SETSEL, 0, -1); // Chọn tất cả
	SendMessageA(hwnd, EM_SETSEL, -1, -1);// Bỏ chọn và giữ vị trí ở cuối text box
	SendMessageA(hwnd, EM_REPLACESEL, 0, (LPARAM)""); // Chèn chuỗi vào vị trí cuối và scroll

	// dec edit box
	hwnd = static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_DEC))->GetSafeHwnd();
	SendMessageA(hwnd, EM_SETSEL, 0, -1); // Chọn tất cả
	SendMessageA(hwnd, EM_SETSEL, -1, -1);// Bỏ chọn và giữ vị trí ở cuối text box
	SendMessageA(hwnd, EM_REPLACESEL, 0, (LPARAM)""); // Chèn chuỗi vào vị trí cuối và scroll

	// bin edit box
	hwnd = static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_BIN))->GetSafeHwnd();
	SendMessageA(hwnd, EM_SETSEL, 0, -1); // Chọn tất cả
	SendMessageA(hwnd, EM_SETSEL, -1, -1);// Bỏ chọn và giữ vị trí ở cuối text box
	SendMessageA(hwnd, EM_REPLACESEL, 0, (LPARAM)""); // Chèn chuỗi vào vị trí cuối và scroll
}

void CalculatorDlg::process(char ch)
{
	BigInt lhs, rhs;
	if (ch == ')')
	{
		// tính biểu thức giữa 2 dấu ngoặc
		while (m_inputs.back() != '(')
		{
			rhs = m_operands.back();
			m_operands.pop_back();
			lhs = m_operands.back();
			m_operands.pop_back();

			try
			{
				m_operands.push_back(m_char_to_opr[m_inputs.back()](lhs, rhs));

			}
			catch(const std::exception &e)
			{
				this->OnBnClickedButtonC();
				m_operands.push_back(0);
				m_text_input = e.what();
				UpdateData(FALSE);
				return;
			}
			m_inputs.pop_back();
		}

		m_inputs.pop_back();		// lấy dấu ngoặc '(' ra
		m_parentheses--;
	}
	else
	{
		// tính giá trị các biểu thức đằng trước nếu các toán tử đằng trước...
		// ...có độ ưu tiên bằng hoặc cao hơn
		while (!m_inputs.empty() && priority_less_equal_than(ch, m_inputs.back()))
		{
			rhs = m_operands.back();		// lấy ra toán hạng phải
			m_operands.pop_back();
			lhs = m_operands.back();		// lấy ra toán hạng trái
			m_operands.pop_back();

			m_operands.push_back(m_char_to_opr[m_inputs.back()](lhs, rhs));		// gọi hàm tính toán theo 2 toán hạng

			m_inputs.pop_back();		// lấy toán tử vừa tính ra khỏi hàng đợi
		}

		m_inputs.push_back(ch);		// đẩy vào toán tử vừa nhập
	}

	//if (m_operands.empty() && !m_inputs.empty() && (m_inputs.back() == '-' || m_inputs.back() == '+'))
	//	m_operands.push_back(0);
	if (m_inputs.size() > 1)
		m_value = m_operands.back();
	else
		m_value = m_operands.front();
	update_text();
}

void CalculatorDlg::process_operator(char opr)
{
	if (m_inputs.empty() && m_prev == NONE)
		m_text_history = "0";
	if (m_prev != BINARY_OPERATOR)		// lần nhập trước ko phải là toán tử
	{
		if (m_prev == NUMBER)
			m_operands.push_back(m_value);
		else if (m_prev == CALCULATE && !m_inputs.empty())	// người dùng nhấn '=' trước đó
		{
			// lấy toán tử cũ ra
			m_inputs.pop_back();
			// lấy số hạng cũ (rhs) ra
			m_operands.pop_back();
			// cập nhật thông tin của history edit box
			m_text_history = to_wstring(m_operands.back()).c_str();
		}
		m_text_history.AppendChar(opr);
		process(opr);
		m_prev = BINARY_OPERATOR;
	}
	else		// lần nhập trước là toán tử
	{
		// nếu toán tử vừa nhập khác toán tử phía trước => thay thế toán tử trước đó
		if (m_inputs.back() != opr)
		{
			// cập nhật toán tử trong hàng đợi
			m_inputs.pop_back();
			m_inputs.push_back(opr);
			// cập nhật toán tử hiển thị ra màn hình
			m_text_history.Delete(m_text_history.GetLength() - 1);
			m_text_history.AppendChar(opr);
			
			UpdateData(FALSE);
		}
	}
}

void CalculatorDlg::enable_char_button(BOOL flag)
{
	for (UINT button_id = IDC_BUTTON_CHAR_A; button_id <= IDC_BUTTON_CHAR_F; button_id++)
	{
		CButton *pBt = (CButton*)this->GetDlgItem(button_id);
		pBt->EnableWindow(flag);
	}
}

void CalculatorDlg::enable_2_9_button(BOOL flag)
{
	for (UINT button_id = IDC_BUTTON_NUMBER_2; button_id <= IDC_BUTTON_NUMBER_9; button_id++)
	{
		CButton *pBt = (CButton*)this->GetDlgItem(button_id);
		pBt->EnableWindow(flag);
	}
}

void CalculatorDlg::OnBnClickedButtonC()
{
	m_value = 0;
	m_text_history = "";
	std::deque<char>().swap(m_inputs);
	std::deque<BigInt>().swap(m_operands);
	m_prev = NONE;
	m_parentheses = 0;

	update_text();
}


void CalculatorDlg::OnBnClickedButtonNumber1()
{
	// button number 1
	
	insert_num(1);
	update_text();
}


void CalculatorDlg::OnBnClickedButtonNumber2()
{
	// button number 2
	if (m_base != BINARY)
	{
		insert_num(2);
		update_text();
	}
}


void CalculatorDlg::OnBnClickedButtonNumber3()
{
	// button number 3

	if (m_base != BINARY)
	{
		insert_num(3);
		update_text();
	}
}


void CalculatorDlg::OnBnClickedButtonNumber4()
{
	// button number 4

	if (m_base != BINARY)
	{
		insert_num(4);
		update_text();
	}
}


void CalculatorDlg::OnBnClickedButtonNumber5()
{
	// button number 5

	if (m_base != BINARY)
	{
		insert_num(5);
		update_text();
	}
}


void CalculatorDlg::OnBnClickedButtonNumber6()
{
	// button number 6

	if (m_base != BINARY)
	{
		insert_num(6);
		update_text();
	}
}


void CalculatorDlg::OnBnClickedButtonNumber7()
{
	// button number 7

	if (m_base != BINARY)
	{
		insert_num(7);
		update_text();
	}
}


void CalculatorDlg::OnBnClickedButtonNumber8()
{
	// button number 8

	if (m_base != BINARY)
	{
		insert_num(8);
		update_text();
	}
}


void CalculatorDlg::OnBnClickedButtonNumber9()
{
	// button number 9

	if (m_base != BINARY)
	{
		insert_num(9);
		update_text();
	}
}


void CalculatorDlg::OnBnClickedButtonNumber0()
{
	// button number 0

	insert_num(0);
	update_text();
}


void CalculatorDlg::OnBnClickedRadioBin()
{
	// bin radio button
	m_base = BINARY;
	update_text();
	m_text_history = m_text_bin;
	enable_2_9_button(FALSE);
	enable_char_button(FALSE);
	UpdateData(FALSE);
}


void CalculatorDlg::OnBnClickedRadioHex()
{
	// hex radion button
	m_base = HEXADECIMAL;
	update_text();
	m_text_history = m_text_hex;
	enable_2_9_button(TRUE);
	enable_char_button(TRUE);
	UpdateData(FALSE);
}


void CalculatorDlg::OnBnClickedButtonSubtract()
{
	// subtract operation
	process_operator('-');
}


void CalculatorDlg::OnBnClickedButtonEqual()
{
	if (m_inputs.empty())
		return;

	if (m_prev == NUMBER)				// lần nhập trước là số nhưng chưa đẩy vào hàng đợ
		m_operands.push_back(m_value);
	else if (m_prev == BINARY_OPERATOR)		// nhập dư 1 toán tử
	{
		m_inputs.pop_back();
		m_text_history.Delete(m_text_history.GetLength() - 1);
	}

	if (m_parentheses > 0)		// còn thiếu dấu ngoặc chưa đóng
	{
		while (m_inputs.back() == '(')
		{
			m_inputs.pop_back();
			m_parentheses--;
		}

		while (m_parentheses > 0)
		{
			process(')');
			m_parentheses--;
		}
	}

	
	if (m_prev == CALCULATE)		// trường hợp người dùng nhấn nhiều lần dấu '=' liên tiếp
	{
		// cập nhật history text box
		int index = m_text_history.Find(m_inputs.back());		// vị trí toán tử
		if (m_base == DECIMAL)
			m_text_history = to_wstring(m_operands.front()).c_str() + CString(m_inputs.front()) + to_wstring(m_operands.back()).c_str();
		else if (m_base == HEXADECIMAL)
			m_text_history = DecToHex(m_operands.front()).c_str() + CString(m_inputs.front()) + DecToHex(m_operands.back()).c_str();
		else
			m_text_history = DecToBin(m_operands.front()).c_str() + CString(m_inputs.front()) + DecToBin(m_operands.back()).c_str();
		// cập nhật hàng đợi toán hạng
		BigInt temp = m_char_to_opr[m_inputs.back()](m_operands.front(), m_operands.back());
		m_operands.pop_front();
		m_operands.push_front(temp);
	}
	else
	{
		while (m_inputs.size() > 1)		// chừa lại toán tử cuối cùng tính sau
		{
			BigInt rhs = m_operands.back();
			m_operands.pop_back();
			BigInt lhs = m_operands.back();
			m_operands.pop_back();

			try
			{
				m_operands.push_back(m_char_to_opr[m_inputs.back()](lhs, rhs));		// gọi toán tử tính giá trị theo 2 toán hạng
			}
			catch(const std::exception &e)
			{
				this->OnBnClickedButtonC();
				m_operands.push_back(0);
				m_text_input = e.what();
				UpdateData(FALSE);
				return;
			}
			m_inputs.pop_back();
		}

		if (m_operands.size() > 1)			// còn nhiều hơn 1 toán hạng
		{
			// tính toán tử cuối cùng
			BigInt temp;
			try
			{
				temp = m_char_to_opr[m_inputs.back()](m_operands.front(), m_operands.back()); // gọi toán tử tính giá trị theo 2 toán hạng

			}
			catch (const std::exception &e)
			{
				this->OnBnClickedButtonC();
				m_operands.push_back(0);
				m_text_input = e.what();
				UpdateData(FALSE);
				return;
			}
			m_operands.pop_front();
			m_operands.push_front(temp);
		}
	}

	m_prev = CALCULATE;
	m_value = m_operands.front();
	update_text();
	
}


void CalculatorDlg::OnBnClickedButtonMultiply()
{
	// multiply operation
	process_operator('*');
}


void CalculatorDlg::OnBnClickedButtonAdd()
{
	// add operation
	process_operator('+');
}


void CalculatorDlg::OnBnClickedButtonDivide()
{
	// divide operation
	process_operator('/');
}


void CalculatorDlg::OnBnClickedButtonCE()
{
	// CE button
	this->OnBnClickedButtonC();
}


void CalculatorDlg::OnBnClickedButtonModulo()
{
	// mod operation
	process_operator('%');
}


void CalculatorDlg::OnBnClickedButtonAnd()
{
	// And operation
	process_operator('&');
}


void CalculatorDlg::OnBnClickedButtonOr()
{
	// Or operation
	process_operator('|');
}


void CalculatorDlg::OnBnClickedButtonXor()
{
	// Xor operation
	process_operator('^');
}


void CalculatorDlg::OnBnClickedButtonNot()
{
	// Not operation
	//m_operation = CalculatorDlg::Operation::NOT;
	m_value = ~m_value;
	m_text_history = to_wstring(m_value).c_str();
	update_text();
	m_prev = UNARY_OPERATOR;
}


void CalculatorDlg::OnBnClickedButtonShiftRight()
{
	// Shift right operation
	//m_operation = CalculatorDlg::Operation::SHIFT_RIGHT;
	m_value = m_value >> 1;
	m_text_history = to_wstring(m_value).c_str();
	update_text();
	m_prev = UNARY_OPERATOR;
}


void CalculatorDlg::OnBnClickedButtonShiftLeft()
{
	// Shift left operation
	//m_operation = CalculatorDlg::Operation::SHIFT_LEFT;
	m_value = m_value << 1;
	m_text_history = to_wstring(m_value).c_str();
	update_text();
	m_prev = UNARY_OPERATOR;
}


void CalculatorDlg::OnBnClickedRadioDec()
{
	m_base = DECIMAL;
	m_text_history = to_wstring(m_value).c_str();
	enable_2_9_button(TRUE);
	enable_char_button(FALSE);
	update_text();
}


void CalculatorDlg::OnBnClickedButtonLeftParenthese()
{
	// dấu ngoặc trái
	if (m_prev == CALCULATE)
	{
		this->OnBnClickedButtonC();
		m_inputs.push_back('(');
		m_prev = LEFT_PARENTHESES;
		m_text_history.AppendChar(_T('('));
		m_parentheses++;
		UpdateData(FALSE);
	}
	else if (m_prev != NUMBER)
	{
		if (m_prev == NONE && m_inputs.empty())
			m_operands.pop_back();

		m_text_history.AppendChar('(');
		m_inputs.push_back('(');
		UpdateData(FALSE);
		m_prev = LEFT_PARENTHESES;
		m_parentheses++;
	}
}


void CalculatorDlg::OnBnClickedButtonRightParenthese()
{
	// dấu ngoặc phải
	if (m_prev == NUMBER && m_parentheses > 0)
	{
		if (m_prev == NUMBER)
			m_operands.push_back(m_value);
		m_text_history.AppendChar(')');
		process(')');
		m_prev = RIGHT_PARENTHESES;
	}
}


void CalculatorDlg::OnBnClickedButtonCharA()
{
	// A button
	if (m_base == HEXADECIMAL)
	{
		if (m_prev == CALCULATE)
			m_value = 0;

		m_prev = NUMBER;
		insert_num('A' - 'A' + 10);
		update_text();
	}
}


void CalculatorDlg::OnBnClickedButtonCharB()
{
	// Char B button
	if (m_base == HEXADECIMAL)
	{
		if (m_prev == CALCULATE)
			m_value = 0;
		m_prev = NUMBER;

		insert_num('B' - 'A' + 10);
		update_text();
	}
}


void CalculatorDlg::OnBnClickedButtonCharC()
{
	// Char C button
	if (m_base == HEXADECIMAL)
	{
		if (m_prev == CALCULATE)
			m_value = 0;

		m_prev = NUMBER;
		insert_num('C' - 'A' + 10);
		update_text();
	}
}


void CalculatorDlg::OnBnClickedButtonCharD()
{
	// Char D button
	if (m_base == HEXADECIMAL)
	{
		if (m_prev == CALCULATE)
			m_value = 0;

		m_prev = NUMBER;
		insert_num('D' - 'A' + 10);
		update_text();
	}
}


void CalculatorDlg::OnBnClickedButtonCharE()
{
	// Char E button
	if (m_base == HEXADECIMAL)
	{
		if (m_prev == CALCULATE)
			m_value = 0;

		m_prev = NUMBER;
		insert_num('E' - 'A' + 10);
		update_text();
	}
}


void CalculatorDlg::OnBnClickedButtonCharF()
{
	// Char F button
	if (m_base == HEXADECIMAL)
	{
		if (m_prev == CALCULATE)
			m_value = 0;

		m_prev = NUMBER;
		insert_num('F' - 'A' + 10);
		update_text();
	}
}


void CalculatorDlg::OnHelpAbout()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}


void CalculatorDlg::OnFileOpen()
{
	RunFileDlg dlg;
	dlg.DoModal();
}

// Exit program
void CalculatorDlg::OnFileExit()
{
	PostQuitMessage(0);
	//EndDialog(0);
}

// Copy to clipboard
void CalculatorDlg::OnEditCopy()
{
	UpdateData(TRUE);

	if (!OpenClipboard())
	{
		AfxMessageBox(L"Cannot open the Clipboard");
		return;
	}
	// Remove the current Clipboard contents
	if (!EmptyClipboard())
	{
		AfxMessageBox(L"Cannot empty the Clipboard");
		return;
	}
	size_t size = sizeof(TCHAR)*(1 + m_text_input.GetLength());
	HGLOBAL hResult = GlobalAlloc(GMEM_MOVEABLE, size);
	LPTSTR lptstrCopy = (LPTSTR)GlobalLock(hResult);
	memcpy(lptstrCopy, m_text_input.GetBuffer(), size);
	GlobalUnlock(hResult);

#ifndef _UNICODE
	if (::SetClipboardData(CF_TEXT, hResult) == NULL)
#else
	if (::SetClipboardData(CF_UNICODETEXT, hResult) == NULL)
#endif
	{
		TRACE("Unable to set Clipboard data");
		GlobalFree(hResult);
		CloseClipboard();
		return;
	}

	CloseClipboard();
}


void CalculatorDlg::OnEditPaste()
{
	//open the clipboard
	CString from_clipboard;

	if (OpenClipboard())
	{
		HANDLE hData = GetClipboardData(CF_TEXT);
		char * buffer = (char*)GlobalLock(hData);
		from_clipboard = buffer;
		GlobalUnlock(hData);
		CloseClipboard();
	}

	//kiểm tra dữ liệu & cập nhật giá trị
	const std::wregex dec_pattern(L"\\-?[[:digit:]]+");		// dấu trừ (option) và các ký tự từ '0' -> '9'
	const std::wregex bin_pattern(L"[01]+");
	const std::wregex hex_pattern(L"[0123456789ABCDEFabcdef]+");

	// dọn dẹp các hàng đợi
	std::deque<char>().swap(m_inputs);
	std::deque<BigInt>().swap(m_operands);

	if (m_base == DECIMAL)
	{
		if (!std::regex_match(static_cast<LPCTSTR>(from_clipboard), dec_pattern))	// dữ liệu không hợp lệ
			return;	

		m_value = BigInt(from_clipboard);
		m_text_history = to_wstring(m_value).c_str();
		m_prev = NUMBER;
	}
	else if (m_base == BINARY)
	{
		if (!std::regex_match(static_cast<LPCTSTR>(from_clipboard), bin_pattern))	// dữ liệu không hợp lệ
			return;

		m_value = BinToDec(static_cast<LPCTSTR>(from_clipboard));
		m_text_history = to_wstring(m_value).c_str();
		m_prev = NUMBER;
	}
	else
	{
		if (!std::regex_match(static_cast<LPCTSTR>(from_clipboard), hex_pattern))	// dữ liệu không hợp lệ
			return;
		
		m_value = HexToDec(static_cast<LPCTSTR>(from_clipboard));
		m_text_history = to_wstring(m_value).c_str();
		m_prev = NUMBER;
	}



	update_text();		// cập nhật text box
}

// Xử lý khi nhấn phím vật lý
BOOL CalculatorDlg::PreTranslateMessage(MSG* pMsg)
{
	// Ánh xạ từ ký tự sang các hàm xử lý phím nhấn
	static std::map<UINT, std::function<void(void)>> char_to_func =
	{
		// number
		{ '1', std::bind(&CalculatorDlg::OnBnClickedButtonNumber1, this) },
		{ '2', std::bind(&CalculatorDlg::OnBnClickedButtonNumber2, this) },
		{ '3', std::bind(&CalculatorDlg::OnBnClickedButtonNumber3, this) },
		{ '4', std::bind(&CalculatorDlg::OnBnClickedButtonNumber4, this) },
		{ '5', std::bind(&CalculatorDlg::OnBnClickedButtonNumber5, this) },
		{ '6', std::bind(&CalculatorDlg::OnBnClickedButtonNumber6, this) },
		{ '7', std::bind(&CalculatorDlg::OnBnClickedButtonNumber7, this) },
		{ '8', std::bind(&CalculatorDlg::OnBnClickedButtonNumber8, this) },
		{ '9', std::bind(&CalculatorDlg::OnBnClickedButtonNumber9, this) },
		{ '0', std::bind(&CalculatorDlg::OnBnClickedButtonNumber0, this) },

		// character
		{ 'A', std::bind(&CalculatorDlg::OnBnClickedButtonCharA, this) },
		{ 'B', std::bind(&CalculatorDlg::OnBnClickedButtonCharB, this) },
		{ 'C', std::bind(&CalculatorDlg::OnBnClickedButtonCharC, this) },
		{ 'D', std::bind(&CalculatorDlg::OnBnClickedButtonCharD, this) },
		{ 'E', std::bind(&CalculatorDlg::OnBnClickedButtonCharE, this) },
		{ 'F', std::bind(&CalculatorDlg::OnBnClickedButtonCharF, this) },
	};
	
	auto key = pMsg->wParam;

	if (pMsg->message == WM_KEYDOWN)
	{
		if (GetKeyState(VK_SHIFT) >= 0)			// no shift holding
		{
			if (key >= _T('0') && key <= _T('9'))		// nhấn số
				char_to_func[key]();
			else if (key >= VK_NUMPAD0 && key <= VK_NUMPAD9)		// nhấn số (numpad)
				char_to_func[key - VK_NUMPAD0 + '0']();
			else if (key >= _T('A') && key <= _T('F'))		// nhấn chữ
				char_to_func[key]();
			else if (key >= _T('a') && key <= _T('f'))		// nhấn chữ 
				char_to_func[key - 'a' + 'A']();
			else if (key == VK_ADD)
				this->OnBnClickedButtonAdd();		// cộng (numpad)
			else if (key == VK_SUBTRACT || key == VK_OEM_MINUS)
				this->OnBnClickedButtonSubtract();			// trừ
			else if (key == VK_MULTIPLY)
				this->OnBnClickedButtonMultiply();
			else if (key == VK_DIVIDE || key == VK_OEM_2)
				this->OnBnClickedButtonDivide();		// chia
			else if (key == VK_RETURN)
			{
				this->OnBnClickedButtonEqual();
				return TRUE;
			}
			else if (key == VK_ESCAPE)
				return TRUE;
			else if (key == VK_BACK)
				this->OnBnClickedButtonBackspace();
		}
		else	// shift holding
		{
			if (key == VK_OEM_PLUS)
				this->OnBnClickedButtonAdd();		// cộng (oem)
			else if (key == _T('8'))
				this->OnBnClickedButtonMultiply();		// *
			else if (key == _T('5'))
				this->OnBnClickedButtonModulo();		// %
			else if (key == _T('7'))
				this->OnBnClickedButtonAnd();			// And
			else if (key == VK_OEM_5)
				this->OnBnClickedButtonOr();		// Or
			else if (key == _T('6'))
				this->OnBnClickedButtonXor();		// Xor
			else if (key == VK_OEM_3)
				this->OnBnClickedButtonNot();		// Not
			else if (key == _T('9'))
				this->OnBnClickedButtonLeftParenthese();		// ngoặc trái
			else if (key == _T('0'))
				this->OnBnClickedButtonRightParenthese();		// ngoặc phải
		}
	}

	
	return CDialog::PreTranslateMessage(pMsg);
}

void CalculatorDlg::OnSetFocusInput()
{
	static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_INPUT))->HideCaret();
}

void CalculatorDlg::OnSetFocusHistory()
{
	static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_HISTORY))->HideCaret();
}

void CalculatorDlg::OnSetFocusHex()
{
	static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_HEX))->HideCaret();
}

void CalculatorDlg::OnSetFocusDec()
{
	static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_DEC))->HideCaret();
}

void CalculatorDlg::OnSetFocusBin()
{
	static_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_BIN))->HideCaret();
}


void CalculatorDlg::OnBnClickedButtonChangeSign()
{
	// đổi dấu
	m_value = 0 - m_value;

	update_text();
	if (m_base == DECIMAL)
		m_text_history = m_text_dec;
	else if (m_base == BINARY)
		m_text_history = m_text_bin;
	else
		m_text_history = m_text_hex;
	UpdateData(FALSE);

	std::deque<BigInt>().swap(m_operands);
	m_operands.push_back(m_value);
	std::deque<char>().swap(m_inputs);
	m_prev = UNARY_OPERATOR;
}

// nhấn nút backspace
void CalculatorDlg::OnBnClickedButtonBackspace()
{
	if (m_prev == NUMBER && m_value != 0)
	{
		if (m_base == DECIMAL)
			m_value = m_value / 10;
		else if (m_base == HEXADECIMAL)
			m_value = m_value / 16;
		else
			m_value = m_value / 2;

		m_text_history.Truncate(m_text_history.GetLength() - 1);
		update_text();
	}
}
