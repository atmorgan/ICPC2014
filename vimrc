set nocompatible
syntax on
set hlsearch
set incsearch
set backspace=indent,eol,start
set whichwrap=b,s,<,>,[,]
set number
filetype plugin indent on

set shiftwidth=4
set tabstop=4
set expandtab
set smarttab
set softtabstop=4
set cindent

function! Compile()
		let mp = &makeprg
		let ef = &errorformat

		let curr_dir = expand('%:h')
		if curr_dir == ''
				let curr_dir = '.'
		endif
		execute 'lcd ' . curr_dir

		if &filetype == "cpp"
				let &makeprg = "c++ -g -o %< -Wall %"
		elseif &filetype == "java"
				let &makeprg = "javac -Xlint %"
				set errorformat=%A%f:%l:\ %m,%-Z%p^,%-C%.%#
		else
				let &makeprg = "make"
		endif

		echo 'Compiling ' . expand('%')

		silent make
		execute ':cwindow'
		execute 'lcd -'
		redraw!

		let &errorformat = ef
		let &makeprg = mp
endfunction

noremap <F9> :update<CR>:call Compile()<CR>
noremap <F10> :cnext<CR>
noremap <F11> :cprevious<CR>

function! Run()
    let filename = expand('%:t:r')

    let curr_dir = expand('%:h')
    if curr_dir == ''
        let curr_dir = '.'
    endif
    execute 'lcd ' . curr_dir

    if &filetype=="java"
				execute "!java " . filename . " < " . filename . ".in"
    else
				"Windows: execute "!"   . filename . " < " . filename . ".in"
				"Linux:   execute "!./" . filename . " < " . filename . ".in"
				execute "!" . filename . " < " . filename . ".in"
    endif
    execute 'lcd -'
endfunction

noremap <F5> :call Run()<CR>

nnoremap j gj
nnoremap k gk
nnoremap tn :tabnew<Space>
