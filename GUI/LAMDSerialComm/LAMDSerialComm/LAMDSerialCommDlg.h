
// LAMDSerialCommDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "Mycomm.h"
#include "LAMDDefine.h"

// CLAMDSerialCommDlg 대화 상자
class CLAMDSerialCommDlg : public CDialogEx
{
    // 생성입니다.
public:
    CLAMDSerialCommDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

    CMycomm* m_comm;
    LRESULT OnThreadclosed(WPARAM length, LPARAM lParam);
    LRESULT OnReceive(WPARAM length, LPARAM lParam);
    // 대화 상자 데이터입니다.
    enum { IDD = IDD_LAMDSERIALCOMM_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

    // 구현입니다.
protected:
    HICON m_hIcon;

    // 생성된 메시지 맵 함수
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()
public:
    BOOL comport_state;

    CComboBox m_combo_comport;
    CComboBox m_combo_baudrate;
    CString m_str_comport;
    CString m_str_baudrate;

    CRichEditCtrl m_edit_view;
    CEdit m_edit_send;
    afx_msg void OnCbnDropdownComboComport();
    void GetSerialPort(void);
    void SetComboBaudrate(void);
    afx_msg void OnBnClickedButtonConnect();
    afx_msg void OnCbnSelchangeComboComport();
    afx_msg void OnCbnSelchangeComboBaudrate();
    afx_msg void OnBnClickedButtonClear();
    afx_msg void OnBnClickedButtonSend();
    UINT8 TransmitUARTData(BYTE CMD, BYTE* Data, int Length);
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnBnClickedButtonApply();

    double m_dCtrlFreq;
    UINT m_u32CtrlSwtch;
    double m_dTestmode;

    UINT32 m_u32CtrlFreq;
    afx_msg void OnBnClickedRadioOp();
    afx_msg void OnBnClickedRadioTest();
    void ChangeATTMode(int Radio);

     BYTE TransData[256];
     BOOL m_bRadioState;
     CString m_str_FtwValue;
     CString m_str_AttValue;
     double m_dDDS_Freq;
     void AddEventString(wchar_t* ap_string);
     void AddEventString(CString str);
     UINT16 n_u16EditLineCount;
};
