#SingleInstance, Force
SendMode Input
SetWorkingDir, %A_ScriptDir%

SetTimer, CloseApp, 15000

arg1 := A_Args[1]
arg2 := A_Args[2]
arg3 := A_Args[3]
arg4 := A_Args[4]
arg5 := A_Args[5]

posX := arg1
posY := arg2
width := arg3
height := arg4
programPath := arg5

;将posX、posY、width、height从字符串转换为数字
posX := posX + 0
posY := posY + 0
width := width + 0
height := height + 0

Run, %programPath%

; 等待应用程序窗口出现
; 如果超过15秒还没有出现，则退出脚本

WinWait, ahk_exe %programPath%


; 获取窗口的句柄
WinGet, hwnd, ID, ahk_exe %programPath%

;如果posx，posy，width，height都为0，则最小化窗口
if (posX == 0 && posY == 0 && width == 0 && height == 0)
{
    WinMinimize, ahk_id %hwnd%
    ExitApp
}

CloseApp(){
    ExitApp
}

; 调整窗口的位置和大小
WinMove, ahk_id %hwnd%, , %posX%, %posY%, %width%, %height%
ExitApp
