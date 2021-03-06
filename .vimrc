if has('win32')
    let g:VimOI_CompileArgs = ['/Od', '/nologo', '/utf-8', '/EHsc', '/W4', '/D_CRT_SECURE_NO_WARNINGS', '/DDEBUG']
else
    let g:VimOI_CompileArgs = ['-O0', '-Wall', '-Wextra', '-g', '-DDEBUG']
    let g:VimOI_CompileProg = 'g++'
endif
let g:ycm_confirm_extra_conf = 0
" Fast yank and copy the whole file
nnoremap YY gg"+yG''zz
nnoremap YP ggVG"+p
" Create solution file from template
command! -nargs=1 BegProb execute "edit " . luaeval("assert(loadfile('utils/do.lua'))('" . <q-args> . "')")
command! EndProb execute (has("win32") ? "!move " : "!mv ") . expand('%') . ' '
            \ . expand('%:h:h') . '/done/' . expand('%:t') | bw
command! DelProb execute (has("win32") ? "!del " : "!rm ") . expand('%') | bw
command! EndProbAsTmpl execute (has("win32") ? "!move " : "!mv ") . expand('%') . ' '
            \ . expand('%:h:h') . '/tmpl/' . input("Template name: ", expand('%:t:r')) . '.cpp' | bw
command! Clean execute (has("win32") ? "!del *.obj *.exe" : "!rm a.out")

" Going to merge into VimOI
command! OINewRedirBuf below 7split | enew | echom "Redir buffer at " . bufnr('%') | set bt=nofile winfixheight

