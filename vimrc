"Windows: :e $HOME/_vimrc
"Linux:   :e $HOME/.vimrc

set nocompatible
set number
syntax on
filetype plugin indent on
set bs=indent,eol,start
set et
set sw=4
set ts=4
set hls

nnoremap j gj
nnoremap k gk
nnoremap tn :tabnew<Space>
nnoremap <C-l> gt
nnoremap <C-h> gT
nnoremap <C-m> :make<CR>

"set backspace=indent,eol,start
"set expandtab
"set shiftwidth=4
"set tabstop=4
"set hlsearch
