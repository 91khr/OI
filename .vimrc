let g:VimOI_CompileArgs = ['/Od', '/nologo', '/utf-8', '/EHsc', '/W4', '/D_CRT_SECURE_NO_WARNINGS', '/DDEBUG']
command! Done execute 'AsyncRun lua done.lua ' . expand('%:h') . ' ' . expand('%:t:r') . ' ' . expand('%:t')
command! -nargs=1 Do AsyncRun lua do.lua undone/<args>.cpp
command! Test execute 'AsyncRun -post=copen python test/' . expand('%:t:r') . '.py'

