let SessionLoad = 1
let s:so_save = &g:so | let s:siso_save = &g:siso | setg so=0 siso=0 | setl so=-1 siso=-1
let v:this_session=expand("<sfile>:p")
let NetrwTopLvlMenu = "Netrw."
let NetrwMenuPriority =  80 
silent only
silent tabonly
cd ~/Desktop/duck/42_miniRT
if expand('%') == '' && !&modified && line('$') <= 1 && getline(1) == ''
  let s:wipebuf = bufnr('%')
endif
let s:shortmess_save = &shortmess
if &shortmess =~ 'A'
  set shortmess=aoOA
else
  set shortmess=aoO
endif
badd +41 Makefile
badd +172 srcs/opencl_srcs/ray.c
badd +49 srcs/create/scene_objects.c
badd +36 srcs/opencl_srcs/bxdf.c
badd +18 srcs/opencl_srcs/utils.c
badd +51 srcs/main.c
badd +102 srcs/utils/utils.c
badd +86 inc/minirt.h
badd +20 inc/data_struct.h
badd +75 srcs/opencl_srcs/brdf.c
badd +45 srcs/intersects/object_intercept.c
badd +1 ~/Desktop/duck/42_miniRT
badd +40 srcs/opencl_srcs/opencl.h
badd +1 scenes/lands.rt
argglobal
%argdel
$argadd ~/Desktop/duck/42_miniRT
edit srcs/opencl_srcs/ray.c
let s:save_splitbelow = &splitbelow
let s:save_splitright = &splitright
set splitbelow splitright
wincmd _ | wincmd |
vsplit
1wincmd h
wincmd w
let &splitbelow = s:save_splitbelow
let &splitright = s:save_splitright
wincmd t
let s:save_winminheight = &winminheight
let s:save_winminwidth = &winminwidth
set winminheight=0
set winheight=1
set winminwidth=0
set winwidth=1
wincmd =
argglobal
balt srcs/create/scene_objects.c
setlocal foldmethod=expr
setlocal foldexpr=v:lua.LazyVim.treesitter.foldexpr()
setlocal foldmarker={{{,}}}
setlocal foldignore=#
setlocal foldlevel=99
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldenable
142
sil! normal! zo
163
sil! normal! zo
let s:l = 172 - ((33 * winheight(0) + 31) / 62)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 172
let s:c = 39 - ((37 * winwidth(0) + 56) / 112)
if s:c > 0
  exe 'normal! ' . s:c . '|zs' . 39 . '|'
else
  normal! 039|
endif
lcd ~/Desktop/duck/42_miniRT
wincmd w
argglobal
if bufexists(fnamemodify("~/Desktop/duck/42_miniRT/srcs/opencl_srcs/opencl.h", ":p")) | buffer ~/Desktop/duck/42_miniRT/srcs/opencl_srcs/opencl.h | else | edit ~/Desktop/duck/42_miniRT/srcs/opencl_srcs/opencl.h | endif
if &buftype ==# 'terminal'
  silent file ~/Desktop/duck/42_miniRT/srcs/opencl_srcs/opencl.h
endif
balt ~/Desktop/duck/42_miniRT/srcs/opencl_srcs/bxdf.c
setlocal foldmethod=indent
setlocal foldexpr=0
setlocal foldmarker={{{,}}}
setlocal foldignore=#
setlocal foldlevel=99
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldenable
let s:l = 28 - ((25 * winheight(0) + 31) / 62)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 28
normal! 016|
wincmd w
wincmd =
tabnext 1
if exists('s:wipebuf') && len(win_findbuf(s:wipebuf)) == 0 && getbufvar(s:wipebuf, '&buftype') isnot# 'terminal'
  silent exe 'bwipe ' . s:wipebuf
endif
unlet! s:wipebuf
set winheight=1 winwidth=20
let &shortmess = s:shortmess_save
let &winminheight = s:save_winminheight
let &winminwidth = s:save_winminwidth
let s:sx = expand("<sfile>:p:r")."x.vim"
if filereadable(s:sx)
  exe "source " . fnameescape(s:sx)
endif
let &g:so = s:so_save | let &g:siso = s:siso_save
set hlsearch
nohlsearch
doautoall SessionLoadPost
unlet SessionLoad
" vim: set ft=vim :
