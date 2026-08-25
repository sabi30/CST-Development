project('CST_LIBRARY', 'cpp', version: '1.0.0', default_options: ['cpp_std=c++17', 'optimization=2'])

source_files = files()
include_files = include_directories(['src', 'cst'])

subdir('src')

library('cst', source_files, include_directories: include_files)
