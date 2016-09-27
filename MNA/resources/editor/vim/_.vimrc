set nocompatible
set number
set ruler
" show matching bracets
set showmatch
set backspace=indent,eol,start
set incsearch		" do incremental searching

filetype plugin indent on
syntax on

set autoindent		" always set autoindenting on
"set softtabstop=2
set shiftwidth=4
set tabstop=8
set noexpandtab "better not to exandtab becase it may disable Makefile commands

set cindent

set cino=>s,e0,n-0.5s,f0,{0.5s,}0,^-0.5s,:0.5s,=0.5s,l1,b1,g0,h0.5s,p0.5s,t0.5s,i0.5s,+0.5s,c3,C0,/0,(0,u2s,U0,w0,W0,m0,M0,j0,)20,*30,#0

"set shiftwidth=2
"set cino=>s,e0,n0,f0,{s,}0,^0,:s,=s,l1,b1,g0,hs,ps,ts,is,+s,c3,C0,/0,(0,u0,U0,w0,W0,m0,M0,j0
"""""""" |  |  |  |  +  |  |  |  |  +  +  +  |  |  |  |  |  |  |  |  +  +  |  |  |  |  |  |  |   |   |

"set nu!
"set guioptions+=g,b,r,l,a
set list
set listchars=tab:>-,trail:-,precedes:<,extends:>

if has("autocmd")
	" Remeber last position where the cursor was
	autocmd BufReadPost *
	\ if line("'\"") > 0 && line ("'\"") <= line("$") |
	\   exe "normal g'\"" |
	\ endif

	"set wrap
  "autocmd FileType text setlocal textwidth=78
	"set whichwrap=<,>,b,s,[,],h,l
	" recognise more lex file
	au BufNewFile,BufRead *.l,*.ll setf lex
	" recognise more yacc file
	au BufNewFile,BufRead *.y,*.yy setf yacc
	" recognise spectre file
	au BufNewFile,BufRead *.spec,*scs setf spectre
	" recognise Verilogams file
	au BufNewFile,BufRead *.va,*.vams setf verilogams
	" recognise more SystemVerilog file
	au BufNewFile,BufRead *.v,*.vh,*.sv,*.svi setf sv
	" recognise spice file
	au BufNewFile,BufRead *.spi,*.spice,*.spic,*.sp,*.cir,*.hsp,*.adm,*.ckt,*.mod,*.in,*.lib,*.inc,*.inp setf spice
endif

if &term == "xterm"
	set t_kb=
	fixdel
	set t_kD=[3~
endif

" minibufexpl.vim recommended settings
let g:miniBufExplMapWindowNavVim = 1
let g:miniBufExplMapWindowNavArrows = 1
let g:miniBufExplMapCTabSwitchBufs = 1
let g:miniBufExplModSelTarget = 1

""""""""""""""""""""""""""""""
" vimgdb setting
""""""""""""""""""""""""""""""
let g:vimgdb_debug_file = ""
run macros/gdb_mappings.vim

" Taglist (ctags)
"
"let Tlist_Ctags_Cmd = '/usr/bin/ctags'
let Tlist_Show_One_File = 1
let Tlist_Exit_OnlyWindow = 1
let Tlist_Use_Left_Window = 1
map <silent> <F12> :TlistToggle<cr>
