#NoEnv
#NoEnv ; Recommended for performance and compatibility with future AutoHotkey releases.
; #Warn  ; Enable warnings to assist with detecting common errors.
SendMode Input ; Recommended for new scripts due to its superior speed and reliability.
SetWorkingDir %A_ScriptDir% ; Ensures a consistent starting directory.
SysGet, VirtualHeight, 79
SysGet, VirtualWidth, 78
CoordMode, Pixel, Screen

arg1 := A_Args[1]
arg2 := A_Args[2]

__ImageFilePath := arg1
MaxAttempts := 2
First := 1

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
        if(A_Args.Length = 2 && First = 1){
            hwnd := WinExist("ahk_exe %arg2%")
            First := 0
        }

        WinGet, winState, MinMax, ahk_id %hwnd%
        WinActivate, ahk_id %hwnd%
        Attempt := 1
        while (Attempt <= MaxAttempts)
        {
            ; 在当前窗口中查找图像
            gui,add,picture,hwnd__mypic,%__ImageFilePath%
            if FileExist(__ImageFilePath){
                controlgetpos,,,__img_w,__img_h,,ahk_id %__mypic%
                CoordMode Pixel
                ImageSearch, __FoundX, __FoundY, 0, 0, VirtualWidth, VirtualHeight,%__ImageFilePath%
                CoordMode Mouse
                PicX:=__FoundX + __img_w/2
                PicY:=__FoundY + __img_h/2
                if (ErrorLevel=0) {
                    MouseGetPos, NowX, NowY
                    CoordMode, Mouse , Screen
                    __ClickX:=PicX
                    __ClickY:=PicY
                    __ClickTimes:=1
                    Click %__ClickX%, %__ClickY%, %__ClickTimes%
                    __ClickX:=NowX
                    __ClickY:=NowY
                    __ClickTimes:=0
                    Click %__ClickX%, %__ClickY%, %__ClickTimes%
                    ExitApp
                } else {
                }
            } else {
            }
            Attempt += 1
        }
        if (winState = -1) ; 最小化状态
        {
            ; 将窗口还原（最小化回去）
            WinMinimize, ahk_id %hwnd%
        }
        else ; 非最小化状态
        {
            ; 不执行任何操作
        }
    }
}
MsgBox, 0, 提示, 未找到按钮
ExitApp
