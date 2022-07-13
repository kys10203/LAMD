
// LAMDSerialCommDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "Mycomm.h"
#include "LAMDDefine.h"

// CLAMDSerialCommDlg ��ȭ ����
class CLAMDSerialCommDlg : public CDialogEx
{
    // �����Դϴ�.
public:
    CLAMDSerialCommDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

    CMycomm* m_comm;
    LRESULT OnThreadclosed(WPARAM length, LPARAM lParam);
    LRESULT OnReceive(WPARAM length, LPARAM lParam);
    // ��ȭ ���� �������Դϴ�.
    enum { IDD = IDD_LAMDSERIALCOMM_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.

    // �����Դϴ�.
protected:
    HICON m_hIcon;

    // ������ �޽��� �� �Լ�
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
