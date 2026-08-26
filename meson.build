project('CST_LIBRARY', 'cpp', version: '1.0.0', default_options: ['cpp_std=c++17', 'optimization=1'])

cc = meson.get_compiler('cpp')

dep = declare_dependency(
  dependencies : cc.find_library('cst', dirs : [meson.current_source_dir()]),
)

source_files = files()
include_files = include_directories(['src', 'cst'])

subdir('src')

executable('cst', source_files, include_directories: include_files, dependencies: dep)