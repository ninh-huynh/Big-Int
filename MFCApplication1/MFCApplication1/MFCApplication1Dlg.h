
// MFCApplication1Dlg.h : header file
//

#pragma once

#include "BigInt.h"
#include "afxwin.h"
#include <map>
#include <stack>
#include <queue>
#include <functional>

#define N_EDIT_BOX 5

// CalculatorDlg dialog
class CalculatorDlg : public CDialogEx
{
// Construction
public:
	CalculatorDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit2();

protected:
	enum Base {DECIMAL = 10, BINARY = 2, HEXADECIMAL = 16};
	enum PREVIOUS { NONE, NUMBER, UNARY_OPERATOR, BINARY_OPERATOR, LEFT_PARENTHESES, RIGHT_PARENTHESES, CALCULATE};

	std::map<UINT, CFont*> m_fonts;		// lưu thông tin font của edit box hiển thị giá trị
	//bool m_calculated;		// đánh dấu người dùng vừa nhấn phím =
			// ánh xạ từ ký tự sang các hàm toán tử
	static std::map<char, BigInt(*)(const BigInt&, const BigInt&)> m_char_to_opr;
	static std::deque<char> m_inputs;			// lưu thông tin toán tử/dấu ngoặc người dùng nhập vào
	static std::deque<BigInt> m_operands;		// chứa các toán hạng trong ngăn xếp, được ánh xạ theo chỉ số...
												// ...từ các toán hạng trong m_inputs
	int m_parentheses;		// lưu số dấu ngoặc hiện tại có trong biểu thức
	PREVIOUS m_prev;			// lưu thông tin nhập vào trước đó (toán hạng, dấu ngoặc, toán tử)
	
	CString m_text_input;		// lưu giá trị hiện thị dưới dạng chuỗi
	BigInt m_value;		// lưu giá trị hiển thị
	Base m_base;		// lưu hệ cơ số hiện tại

	CString m_text_history;		// lưu lịch sử nhập
	CString m_text_hex;			// lưu kết quả theo hexadecimal
	CString m_text_dec;			// lưu kết quả theo decimal
	CString m_text_bin;			// lưu kết quả theo binary

protected:

	void insert_num(int num);
	void update_text();
	void insert_operator(char opr);
	void update_posfix_number();
	bool priority_less_equal_than(char opr1, char opr2);
	bool is_operator(char ch);
	void hide_caret();
	void scroll_to_end_of_edit_box();
	void process(char ch);
	void process_operator(char opr);
	void disable_char_button();
	void enable_char_button(BOOL);
	void enable_2_9_button(BOOL);
public:
	afx_msg void OnBnClickedButtonC();
	afx_msg void OnBnClickedButtonNumber1();
	afx_msg void OnBnClickedButtonNumber2();
	afx_msg void OnBnClickedButtonNumber3();
	afx_msg void OnBnClickedButtonNumber4();
	afx_msg void OnBnClickedButtonNumber5();
	afx_msg void OnBnClickedButtonNumber6();
	afx_msg void OnBnClickedButtonNumber7();
	afx_msg void OnBnClickedButtonNumber8();
	afx_msg void OnBnClickedButtonNumber9();
	afx_msg void OnBnClickedButtonNumber0();
	afx_msg void OnBnClickedRadioBin();
	afx_msg void OnBnClickedRadioHex();
	afx_msg void OnBnClickedButtonSubtract();
	afx_msg void OnBnClickedButtonEqual();
	afx_msg void OnBnClickedButtonMultiply();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDivide();
	afx_msg void OnBnClickedButtonCE();
	afx_msg void OnBnClickedButtonModulo();
	afx_msg void OnBnClickedButtonAnd();
	afx_msg void OnBnClickedButtonOr();
	afx_msg void OnBnClickedButtonXor();
	afx_msg void OnBnClickedButtonNot();
	afx_msg void OnBnClickedButtonShiftRight();
	afx_msg void OnBnClickedButtonShiftLeft();
	afx_msg void OnBnClickedRadioDec();
	afx_msg void OnBnClickedButtonLeftParenthese();
	afx_msg void OnBnClickedButtonRightParenthese();
	afx_msg void OnBnClickedButtonCharA();
	afx_msg void OnBnClickedButtonCharB();
	afx_msg void OnBnClickedButtonCharC();
	afx_msg void OnBnClickedButtonCharD();
	afx_msg void OnBnClickedButtonCharE();
	afx_msg void OnBnClickedButtonCharF();
	afx_msg BOOL CalculatorDlg::OnEraseBkgnd(CDC* pDC);
	afx_msg void OnHelpAbout();
	afx_msg void OnFileOpen();
	afx_msg void OnFileExit();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSetFocusInput();
	afx_msg void OnSetFocusHistory();
	afx_msg void OnSetFocusHex();
	afx_msg void OnSetFocusDec();
	afx_msg void OnSetFocusBin();

	afx_msg void OnBnClickedButtonChangeSign();
	afx_msg void OnBnClickedButtonBackspace();
};
