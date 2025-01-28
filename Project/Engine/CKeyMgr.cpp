#include "pch.h"
#include "CKeyMgr.h"
#include "CEngine.h"
#include "CTimeMgr.h"

UINT key_value[(int)Keyboard::Keyboard_end] = 
{
    VK_OEM_3,
    VK_OEM_MINUS,
    VK_OEM_PLUS,
    VK_OEM_4,
    VK_OEM_6,
    VK_OEM_5,
    VK_OEM_1,
    VK_OEM_7,
    VK_OEM_COMMA,
    VK_OEM_PERIOD,
    VK_OEM_2,

    'Q',
    'W',
    'E',
    'R',
    'T',
    'Y',
    'U',
    'I',
    'O',
    'P',
    'A',
    'S',
    'D',
    'F',
    'G',
    'H',
    'J',
    'K',
    'L',
    'Z',
    'X',
    'C',
    'V',
    'B',
    'N',
    'M',

    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',
    '0',

    VK_F1,
    VK_F2,
    VK_F3,
    VK_F4,
    VK_F5,
    VK_F6,
    VK_F7,
    VK_F8,
    VK_F9,
    VK_F10,
    VK_F11,
    VK_F12,

    VK_ESCAPE,
    VK_TAB,
    VK_CAPITAL,
    VK_LSHIFT,
    VK_LCONTROL,
    VK_LMENU,
    VK_BACK,
    VK_RETURN,
    VK_RSHIFT,
    VK_RMENU,
    VK_RCONTROL,
    VK_SPACE,

    VK_UP,
    VK_DOWN,
    VK_LEFT,
    VK_RIGHT,

    VK_DECIMAL,
    VK_NUMPAD0,
    VK_NUMPAD1,
    VK_NUMPAD2,
    VK_NUMPAD3,
    VK_NUMPAD4,
    VK_NUMPAD5,
    VK_NUMPAD6,
    VK_NUMPAD7,
    VK_NUMPAD8,
    VK_NUMPAD9,

    VK_LBUTTON,
    VK_RBUTTON

    // Keyboard_end
};


CKeyMgr::CKeyMgr()
    : m_MousePos(0, 0)
    , m_CommandTimeout(0)
{

}

CKeyMgr::~CKeyMgr()
{

}

void CKeyMgr::Init()
{
	// ��� Ű ���¸� NONE, ������ ���� �� �������� �ʱ�ȭ
	for (int i = 0; i < (int)Keyboard::Keyboard_end; ++i)
	{
		m_keyboardInfo.push_back(key_info{ Key_state::NONE, false });
	}
}

void CKeyMgr::Tick()
{
    if (GetFocus() == CEngine::GetInst()->GetMainWndHwnd())
    {
        // Ű���� ���� ������Ʈ
        for (int i = 0; i < (int)Keyboard::Keyboard_end; ++i)
        {
            // Ű���尡 ������ ���Ⱦ���
            if (m_keyboardInfo[i].isPrevPressed)
            {
                // ���絵 ���� ������
                if (GetAsyncKeyState(key_value[i]) & 0x8001)
                {
                    m_keyboardInfo[i].state = Key_state::PRESSED;
                }
                // ����� ������
                else
                {
                    m_keyboardInfo[i].state = Key_state::RELEASE;
                    m_keyboardInfo[i].isPrevPressed = false;
                }
            }
            // ������ ���� �� ����
            else
            {
                // ����� ���� ������
                if (GetAsyncKeyState(key_value[i]) & 0x8001)
                {
                    m_keyboardInfo[i].state = Key_state::TAP;
                    m_keyboardInfo[i].isPrevPressed = true;
                    m_Command.push_back((Keyboard)i);
                    m_CommandTimeout = 0.f;
                }
                // ��� �ȴ����־���
                else
                {
                    m_keyboardInfo[i].state = Key_state::NONE;
                }
            }
        }

        // ���콺 ���� ������Ʈ
        POINT point;
        GetCursorPos(&point);
        ScreenToClient(CEngine::GetInst()->GetMainWndHwnd(), &point);

        // Ŀ�ǵ� ����
        m_CommandTimeout += CTimeMgr::GetInst()->GetDeltaTime();
        if (m_CommandTimeout > 0.3)
        {
            m_CommandTimeout = 0.f;
            m_Command.clear();
        }

        // ���� ������ ���콺��ǥ ���
        m_MousePrevPos = m_MousePos;

        // ���� ���콺 ��ǥ ���
        m_MousePos = Vec2((float)point.x, (float)point.y);

        // ���콺 Ŀ���� �����ϴ� ���⺤�͸� ����
        m_DragDir = m_MousePos - m_MousePrevPos;
    }
    else
    {
        for (size_t i = 0; i < m_keyboardInfo.size(); ++i)
        {
            if (Key_state::TAP == m_keyboardInfo[i].state
                || Key_state::PRESSED == m_keyboardInfo[i].state)
            {
                m_keyboardInfo[i].state = Key_state::RELEASE;
            }

            else if (Key_state::RELEASE == m_keyboardInfo[i].state)
            {
                m_keyboardInfo[i].state = Key_state::NONE;
            }

            m_keyboardInfo[i].isPrevPressed = false;
        }

        // ���콺 ��ǥ ����
        *((int*)&m_MousePos.x) = 0xffffffff;
        *((int*)&m_MousePos.y) = 0xffffffff;
    }


}

bool CKeyMgr::IsMouseOffScreen()
{
    Vec2 vResolution = CEngine::GetInst()->GetResolution();

    if (vResolution.x <= m_MousePos.x || vResolution.y <= m_MousePos.y
        || m_MousePos.x < 0 || m_MousePos.y < 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}