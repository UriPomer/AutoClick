﻿; 启动 cmd 并输入指令 --minimize-to-tray
arg1 := A_Args[1]
arg2 := A_Args[2]

Run, cmd.exe /k start /d "%arg1%" %arg2%,, Hide
ExitApp

