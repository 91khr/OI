if has('win32')
    let g:VimOI_CompileArgs = ['/Od', '/nologo', '/utf-8', '/EHsc', '/W4', '/D_CRT_SECURE_NO_WARNINGS', '/DDEBUG']
else
    let g:VimOI_CompileArgs = ['-O0', '-Wall', '-Wextra', '-g']
endif

function! s:DoneProblem()
    bwipeout
endfunction

command! Done call s:DoneProblem()
command! -nargs=1 Do e <args>.cpp
command! Test execute 'AsyncRun -post=copen python check.py'

let g:ycm_confirm_extra_conf = 0
" Fast yank the whole file
nnoremap YY gg"+yG''zz

