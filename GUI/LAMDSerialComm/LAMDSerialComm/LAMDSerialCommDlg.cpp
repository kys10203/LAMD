
// LAMDSerialCommDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "LAMDSerialComm.h"
#include "LAMDSerialCommDlg.h"
#include "afxdialogex.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLAMDSerialCommDlg 대화 상자



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


// CLAMDSerialCommDlg 메시지 처리기

BOOL CLAMDSerialCommDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
    //  프레임워크가 이 작업을 자동으로 수행합니다.
    SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
    SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

    // TODO: 여기에 추가 초기화 작업을 추가합니다.
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

    return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CLAMDSerialCommDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // 아이콘을 그립니다.
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CLAMDSerialCommDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}



void CLAMDSerialCommDlg::OnCbnDropdownComboComport()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    GetSerialPort();
}


void CLAMDSerialCommDlg::GetSerialPort(void)
{
    HKEY hKey;

    // 시리얼포트 번호들이 등록된 위치
    RegOpenKey(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"), &hKey);

    TCHAR szData[20], szName[100];
    DWORD index = 0, dwSize = 100, dwSize2 = 20, dwType = REG_SZ;

    // 콤보박스 리셋
    m_combo_comport.ResetContent();
    memset(szData, 0x00, sizeof(szData));
    memset(szName, 0x00, sizeof(szName));

    // hKey -> 레지스터키 핸들
    // index -> 값을 가져올 인덱스
    // szName -> 항목값이 저장될 배열
    // dwSize -> 배열의 크기
    while(ERROR_SUCCESS == RegEnumValue(hKey, index, szName, &dwSize, NULL, NULL, NULL, NULL))
    {
        index++;

        // szName -> 레지터스터 항목의 이름  
        // dwType -> 항목의 타입, 여기에서는 널로 끝나는 문자열  
        // szData -> 항목값이 저장될 배열  
        // dwSize2 -> 배열의 크기  

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
    //overlapped i/o 핸들을닫는다.
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
        m_comm->Receive(data, length); //length 길이만큼데이터를받는다.
        data[length] = _T('\0');
        str += _T("\r\n");
        for (int i = 0; i<length; i++)
        {
            str += data[i];
        }
        m_edit_view.ReplaceSel(str); //에디트박스에표시하기위함
        str = "";
        //UpdateData(FALSE);
        m_edit_view.LineScroll(m_edit_view.GetLineCount());
    }
    return 0;
}


void CLAMDSerialCommDlg::OnBnClickedButtonConnect()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    if (comport_state)
    {
        if (m_comm)        //컴포트가존재하면
        {
            m_comm->Close();
            m_comm = NULL;
            AfxMessageBox(_T("COM 포트닫힘"));
            comport_state = false;
            GetDlgItem(IDC_BUTTON_CONNECT)->SetWindowText(_T("OPEN"));
            GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(false);
        }
    }
    else
    {
        m_comm = new CMycomm(_T("\\\\.\\") + m_str_comport, m_str_baudrate, _T("None"), _T("8 Bit"), _T("1 Bit"));         // initial Comm port
        if (m_comm->Create(GetSafeHwnd()) != 0) //통신포트를열고윈도우의핸들을넘긴다.
        {
            AfxMessageBox(_T("COM 포트열림"));
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
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    UpdateData();
}


void CLAMDSerialCommDlg::OnCbnSelchangeComboBaudrate()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    UpdateData();
}


void CLAMDSerialCommDlg::OnBnClickedButtonClear()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    GetDlgItem(IDC_RICHEDIT2_VIEW)->SetWindowText(_T(""));
}


void CLAMDSerialCommDlg::OnBnClickedButtonSend()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
        TransData[4] = 0;   // 미정
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
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    ChangeATTMode(RADIO_OP);
}


void CLAMDSerialCommDlg::OnBnClickedRadioTest()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
