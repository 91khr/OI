if has('win32')
    let g:VimOI_CompileArgs = ['/Od', '/nologo', '/utf-8', '/EHsc', '/W4', '/D_CRT_SECURE_NO_WARNINGS', '/DDEBUG']
else
    let g:VimOI_CompileArgs = ['-O0', '-Wall', '-Wextra', '-g', '-DDEBUG']
    let g:VimOI_CompileProg = 'g++'
    let g:compile_flags_cpp = ['-DDEBUG', '-ggdb', '-fsanitize=address', '-fsanitize=undefined',
                \ '-fno-omit-frame-pointer', '-lrt', '-fno-sanitize-recover', '-fstack-protector',
                \ '-D_GLIBCXX_DEBUG', '-D_GLIBCXX_DEBUG_PEDANTIC']
endif

import "ftext.vim"
function! s:Run(args)
    if g:asyncrun_code != 0 | return | endif
    eval s:ftext.TermRun(["systemd-run", "--user", "--scope", "-p", "MemoryLimit=" .. b:memlimit, "./a.out", a:args],
                \ #{ persist: v:true, unique: v:false })
    call lightline#update()
    set winfixheight
endfunction
autocmd BufRead *.cpp {
    command! -buffer -bar -nargs=* Compile exec "AsyncRun g++ % -std=c++20 -o a.out "
                \ .. "-Wall -Wextra -Weffc++ -Wpedantic -g "
                \ .. (has_key(g:, "compile_flags_cpp") ? g:compile_flags_cpp :
                \     get(b:, "compile_flags", []))->join(' ')
                \ .. " " .. <q-args>
    command! -buffer -bar -nargs=? Run Compile | au User AsyncRunStop ++once call s:Run(<q-args>)
    b:memlimit = "1M"
}

" Fast yank and copy the whole file
nnoremap YY gg"+yG''zz
nnoremap YP ggVG"+p

" Create solution file from template
command! -nargs=1 -bar BegProb execute "edit " . luaeval("assert(loadfile('utils/do.lua'))('" . <q-args> . "')")
command! -bar EndProb execute (has("win32") ? "!move " : "!mv ") . expand('%') . ' '
            \ . expand('%:h:h') . '/done/' . expand('%:t') | bw
command! -bar DelProb execute (has("win32") ? "!del " : "!rm ") . expand('%') | bw
command! -bar EndProbAsTmpl execute (has("win32") ? "!move " : "!mv ") . expand('%') . ' '
            \ . expand('%:h:h') . '/tmpl/' . input("Template name: ", expand('%:t:r')) . '.cpp' | bw
command! -bar Clean eval map(["out", "in", "ans", "log"] + (has("win32") ? ["obj", "exe"] : []),
            \ "glob('*.'..v:val, 0, 1)")->flatten()->map("delete(v:val)")

" Going to merge into VimOI
command! OINewRedirBuf below 7split | enew | echom "Redir buffer at " . bufnr('%') | set bt=nofile winfixheight

" Call GDB
command! -bar GDB term gdb -x gdbcmd.txt a.out

" View a graph
command! -range=% -bar Viz <line1>,<line2>w !xdot <(cat) &

