
// LAMDSerialCommDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "LAMDSerialComm.h"
#include "LAMDSerialCommDlg.h"
#include "afxdialogex.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLAMDSerialCommDlg ��ȭ ����



CLAMDSerialCommDlg::CLAMDSerialCommDlg(CWnd* pParent /*=NULL*/)
    : CDialogEx(CLAMDSerialCommDlg::IDD, pParent)
    , m_str_comport(_T(""))
    , m_str_baudrate(_T(""))
    , comport_state(FALSE)
    , m_dCtrlFreq(0)
    , m_u32CtrlSwtch(0)
    , m_dTestmode(0)
    , m_u32CtrlFreq(0)
    , m_bRadioState(FALSE)
    , m_str_FtwValue(_T(""))
    , m_str_AttValue(_T(""))
    , m_dDDS_Freq(0)
    , n_u16EditLineCount(0)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLAMDSerialCommDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_COMPORT, m_combo_comport);
    DDX_Control(pDX, IDC_COMBO_BAUDRATE, m_combo_baudrate);
    DDX_CBString(pDX, IDC_COMBO_COMPORT, m_str_comport);
    DDX_CBString(pDX, IDC_COMBO_BAUDRATE, m_str_baudrate);
    DDX_Control(pDX, IDC_RICHEDIT2_VIEW, m_edit_view);
    DDX_Control(pDX, IDC_EDIT_SEND, m_edit_send);
    DDX_Text(pDX, IDC_EDIT_CTRL_FREQ, m_dCtrlFreq);
    DDX_Text(pDX, IDC_EDIT_CTRL_SWTCH, m_u32CtrlSwtch);
    DDX_Text(pDX, IDC_EDIT_TESTMODE, m_dTestmode);
    DDX_Text(pDX, IDC_STATIC_FTW_VALUE, m_str_FtwValue);
    DDX_Text(pDX, IDC_STATIC_ATT_VALUE, m_str_AttValue);
}

BEGIN_MESSAGE_MAP(CLAMDSerialCommDlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_CBN_DROPDOWN(IDC_COMBO_COMPORT, &CLAMDSerialCommDlg::OnCbnDropdownComboComport)
    ON_MESSAGE(WM_MYCLOSE, &CLAMDSerialCommDlg::OnThreadclosed)
    ON_MESSAGE(WM_MYRECEIVE, &CLAMDSerialCommDlg::OnReceive)
    ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CLAMDSerialCommDlg::OnBnClickedButtonConnect)
    ON_CBN_SELCHANGE(IDC_COMBO_COMPORT, &CLAMDSerialCommDlg::OnCbnSelchangeComboComport)
    ON_CBN_SELCHANGE(IDC_COMBO_BAUDRATE, &CLAMDSerialCommDlg::OnCbnSelchangeComboBaudrate)
    ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CLAMDSerialCommDlg::OnBnClickedButtonClear)
    ON_BN_CLICKED(IDC_BUTTON_SEND, &CLAMDSerialCommDlg::OnBnClickedButtonSend)
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_BUTTON_APPLY, &CLAMDSerialCommDlg::OnBnClickedButtonApply)
    ON_BN_CLICKED(IDC_RADIO_OP, &CLAMDSerialCommDlg::OnBnClickedRadioOp)
    ON_BN_CLICKED(IDC_RADIO_TEST, &CLAMDSerialCommDlg::OnBnClickedRadioTest)
END_MESSAGE_MAP()


// CLAMDSerialCommDlg �޽��� ó����

BOOL CLAMDSerialCommDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
    //  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
    SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
    SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

    // TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
    GetSerialPort();
    SetComboBaudrate();
    GetDlgItem(IDC_BUTTON_CONNECT)->SetWindowText(_T("OPEN"));
    m_str_comport = _T("COM3");
    m_str_baudrate = _T("115200");
    UpdateData(FALSE);

    SetDlgItemText(IDC_EDIT_CTRL_FREQ, _T("7.725"));
    SetDlgItemText(IDC_EDIT_CTRL_SWTCH, _T("350"));

    CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_OP);
    pButton->SetCheck(TRUE);
    GetDlgItem(IDC_EDIT_TESTMODE)->EnableWindow(FALSE);

    memset(TransData, 0, 256);

    return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CLAMDSerialCommDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // �������� �׸��ϴ�.
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CLAMDSerialCommDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}



void CLAMDSerialCommDlg::OnCbnDropdownComboComport()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    GetSerialPort();
}


void CLAMDSerialCommDlg::GetSerialPort(void)
{
    HKEY hKey;

    // �ø�����Ʈ ��ȣ���� ��ϵ� ��ġ
    RegOpenKey(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"), &hKey);

    TCHAR szData[20], szName[100];
    DWORD index = 0, dwSize = 100, dwSize2 = 20, dwType = REG_SZ;

    // �޺��ڽ� ����
    m_combo_comport.ResetContent();
    memset(szData, 0x00, sizeof(szData));
    memset(szName, 0x00, sizeof(szName));

    // hKey -> ��������Ű �ڵ�
    // index -> ���� ������ �ε���
    // szName -> �׸��� ����� �迭
    // dwSize -> �迭�� ũ��
    while(ERROR_SUCCESS == RegEnumValue(hKey, index, szName, &dwSize, NULL, NULL, NULL, NULL))
    {
        index++;

        // szName -> �����ͽ��� �׸��� �̸�  
        // dwType -> �׸��� Ÿ��, ���⿡���� �η� ������ ���ڿ�  
        // szData -> �׸��� ����� �迭  
        // dwSize2 -> �迭�� ũ��  

        RegQueryValueEx(hKey, szName, NULL, &dwType, (LPBYTE)szData, &dwSize2);
        m_combo_comport.AddString(CString(szData));

        memset(szData, 0x00, sizeof(szData));
        memset(szName, 0x00, sizeof(szName));
        dwSize = 100;
        dwSize2 = 20;
    }
    RegCloseKey(hKey);
}


void CLAMDSerialCommDlg::SetComboBaudrate(void)
{
    //m_combo_baudrate.AddString(_T("2400"));
    //m_combo_baudrate.AddString(_T("4800"));
    //m_combo_baudrate.AddString(_T("9600"));
    //m_combo_baudrate.AddString(_T("19200"));
    //m_combo_baudrate.AddString(_T("38400"));
    //m_combo_baudrate.AddString(_T("57600"));
    m_combo_baudrate.AddString(_T("115200"));
}


LRESULT CLAMDSerialCommDlg::OnThreadclosed(WPARAM length, LPARAM lParam)
{
    //overlapped i/o �ڵ����ݴ´�.
    ((CMycomm*)lParam)->HandleClose();
    delete ((CMycomm*)lParam);

    return 0;
}


LRESULT CLAMDSerialCommDlg::OnReceive(WPARAM length, LPARAM lParam)
{
    CString str;
    char data[20000];
    if (m_comm)
    {
        m_comm->Receive(data, length); //length ���̸�ŭ�����͸��޴´�.
        data[length] = _T('\0');
        str += _T("\r\n");
        for (int i = 0; i<length; i++)
        {
            str += data[i];
        }
        m_edit_view.ReplaceSel(str); //����Ʈ�ڽ���ǥ���ϱ�����
        str = "";
        //UpdateData(FALSE);
        m_edit_view.LineScroll(m_edit_view.GetLineCount());
    }
    return 0;
}


void CLAMDSerialCommDlg::OnBnClickedButtonConnect()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    if (comport_state)
    {
        if (m_comm)        //����Ʈ�������ϸ�
        {
            m_comm->Close();
            m_comm = NULL;
            AfxMessageBox(_T("COM ��Ʈ����"));
            comport_state = false;
            GetDlgItem(IDC_BUTTON_CONNECT)->SetWindowText(_T("OPEN"));
            GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(false);
        }
    }
    else
    {
        m_comm = new CMycomm(_T("\\\\.\\") + m_str_comport, m_str_baudrate, _T("None"), _T("8 Bit"), _T("1 Bit"));         // initial Comm port
        if (m_comm->Create(GetSafeHwnd()) != 0) //�����Ʈ���������������ڵ����ѱ��.
        {
            AfxMessageBox(_T("COM ��Ʈ����"));
            comport_state = true;
            GetDlgItem(IDC_BUTTON_CONNECT)->SetWindowText(_T("CLOSE"));
            GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(true);
        }
        else
        {
            AfxMessageBox(_T("ERROR!"));
        }

    }
}


void CLAMDSerialCommDlg::OnCbnSelchangeComboComport()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    UpdateData();
}


void CLAMDSerialCommDlg::OnCbnSelchangeComboBaudrate()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    UpdateData();
}


void CLAMDSerialCommDlg::OnBnClickedButtonClear()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    GetDlgItem(IDC_RICHEDIT2_VIEW)->SetWindowText(_T(""));
}


void CLAMDSerialCommDlg::OnBnClickedButtonSend()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    CString str;
    GetDlgItem(IDC_EDIT_SEND)->GetWindowText(str);
    str += _T("\r\n");
    m_comm->Send((LPCTSTR)str, str.GetLength());
}


UINT8 CLAMDSerialCommDlg::TransmitUARTData(BYTE CMD, BYTE* Data, int Length)
{
    CString str;
    SYSTEMTIME systime;

    UINT8 m_u8Testmode = 0;
    double dTmp = 0.0;
    INT32 m_32CtrlFreq = 0;
    UpdateData(TRUE);

    m_dDDS_Freq = (m_dCtrlFreq / 4.0) - 1.7;
    dTmp = m_dDDS_Freq / 3.4;
    m_u32CtrlFreq = (UINT32)(dTmp * pow(2, 32));

    //CString str;
    //str.Format(_T("%f %d"),dTmp, m_u32CtrlFreq);
    //AfxMessageBox(str);

    TransData[0] = (m_u32CtrlFreq  >> 24) & 0xFF;
    TransData[1] = (m_u32CtrlFreq  >> 16) & 0xFF;
    TransData[2] = (m_u32CtrlFreq  >> 8) & 0xFF;
    TransData[3] = m_u32CtrlFreq & 0xFF;

    if(m_bRadioState)
    {
        m_u8Testmode = (UINT8)(ROUND(m_dTestmode * 2));
        TransData[4] = m_u8Testmode;
    }
    else
    {
        TransData[4] = 0;   // ����
    }

    if((m_dDDS_Freq*1000) < m_u32CtrlSwtch)
    {
        TransData[5] = 0;
    }
    else
    {
        TransData[5] = 1;
    }

    m_comm->Send((LPCTSTR)TransData, 7);

    m_dTestmode = (double)m_u8Testmode/(double)2;
    UpdateData(FALSE);

    ::GetLocalTime(&systime);
    str.Format(_T("[TX, %02d:%02d:%02d] %02X %02X %02X %02X %02X %02X %02X"), systime.wHour, systime.wMinute, systime.wSecond, TransData[0], TransData[1], TransData[2], TransData[3], TransData[4], TransData[5], TransData[6]);
    AddEventString(str);

    return UINT8();
}


HBRUSH CLAMDSerialCommDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

    UINT nID = pWnd -> GetDlgCtrlID();

    switch(nID)
    {
    case IDC_STATIC_FTW:
        pDC -> SetTextColor( RGB(255, 255, 255) );
        pDC -> SetBkColor( RGB(60, 60, 250) );
        hbr = ::CreateSolidBrush( RGB(60, 60, 250) );
        break;
    case IDC_STATIC_ATT:
        pDC -> SetTextColor( RGB(255, 255, 255) );
        pDC -> SetBkColor( RGB(60, 60, 250) );
        hbr = ::CreateSolidBrush( RGB(60, 60, 250) );
        break;
    case IDC_STATIC_SWTCH:
        pDC -> SetTextColor( RGB(255, 255, 255) );
        pDC -> SetBkColor( RGB(60, 60, 250) );
        hbr = ::CreateSolidBrush( RGB(60, 60, 250) );
        break;
    case IDC_STATIC_CRC:
        pDC -> SetTextColor( RGB(255, 255, 255) );
        pDC -> SetBkColor( RGB(60, 60, 250) );
        hbr = ::CreateSolidBrush( RGB(60, 60, 250) );
        break;
    }

    return hbr;
}


void CLAMDSerialCommDlg::OnBnClickedButtonApply()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    if(!comport_state)
    {
        AfxMessageBox(_T("ERROR!"));
        return;
    }

    TransmitUARTData(0, TransData, 7);

    m_str_FtwValue.Format(_T("0x%X"), m_u32CtrlFreq);
    m_str_AttValue.Format(_T("%.3lf MHz"), m_dDDS_Freq * 1000);
    UpdateData(FALSE);
}


void CLAMDSerialCommDlg::OnBnClickedRadioOp()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    ChangeATTMode(RADIO_OP);
}


void CLAMDSerialCommDlg::OnBnClickedRadioTest()
{
    // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    ChangeATTMode(RADIO_TEST);
}


void CLAMDSerialCommDlg::ChangeATTMode(int Radio)
{
    switch(Radio)
    {
    case RADIO_OP:
        GetDlgItem(IDC_EDIT_TESTMODE)->EnableWindow(FALSE);
        m_bRadioState = FALSE;
        break;
    case RADIO_TEST:
        GetDlgItem(IDC_EDIT_TESTMODE)->EnableWindow(TRUE);
        m_bRadioState = TRUE;
        break;
    }
}


void CLAMDSerialCommDlg::AddEventString(wchar_t* ap_string)
{
    if(m_edit_view.GetLineCount() > 500)
    {
        OnBnClickedButtonClear();
    }

    CString str = (LPCTSTR)ap_string;

	m_edit_view.SetSel(-1, -1);

    int len = m_edit_view.GetWindowTextLengthW();

    str += _T("\r\n");

    m_edit_view.SetSel(len, len);
    m_edit_view.ReplaceSel(str);

    m_edit_view.SetSel(-1, -1);

    n_u16EditLineCount++;
}


void CLAMDSerialCommDlg::AddEventString(CString str)
{
    if(m_edit_view.GetLineCount() > 500)
    {
        OnBnClickedButtonClear();
    }

    m_edit_view.SetSel(-1, -1);

    int len = m_edit_view.GetWindowTextLengthW();

    str += _T("\r\n");

    m_edit_view.SetSel(len, len);
    m_edit_view.ReplaceSel(str);

    m_edit_view.SetSel(-1, -1);

    n_u16EditLineCount++;
}
