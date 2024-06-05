#NoEnv
#NoEnv  ; Recommended for performance and compatibility with future AutoHotkey releases.
; #Warn  ; Enable warnings to assist with detecting common errors.
SendMode Input  ; Recommended for new scripts due to its superior speed and reliability.
SetWorkingDir %A_ScriptDir%  ; Ensures a consistent starting directory.
SysGet, VirtualHeight, 79
SysGet, VirtualWidth, 78
CoordMode, Pixel, Screen

arg1 := A_Args[1]

; 获取所有打开的窗口句柄列表
WinGet, WindowList, List

; 遍历窗口列表
Loop, % WindowList
{
    hwnd := WindowList%A_Index%

    WinGetTitle, title, ahk_id %hwnd%

    if (title != "" && title != "Program Manager")
    {
        ; 检查窗口最小化状态
        WinGet, winState, MinMax, ahk_id %hwnd%
        

        if (winState = -1) ; 最小化状态
        {
            ; 将窗口的软件地址保存到JSON
            WinGet, processPath, ProcessPath, ahk_id %hwnd%
            posX := 0
            posY := 0
            posW := 0
            posH := 0
            ; MsgBox, %processPath%
            ; 这里将 processPath 写入 JSON 文件
        }
        else ; 非最小化状态
        {
            ; 将窗口的软件地址、位置、大小保存到JSON
            WinGet, processPath, ProcessPath, ahk_id %hwnd%
            WinGetPos, posX, posY, posW, posH, ahk_id %hwnd%
            ; MsgBox, %processPath% %posX% %posY% %posW% %posH%
            ; 这里将 processPath、posX、posY、posW、posH 写入 JSON 文件
        }
        Run, ../AutoNewProfile/AutoNewProfile.exe %posX% %posY% %posW% %posH% "%processPath%" "%arg1%"
    }
}

ExitApp
