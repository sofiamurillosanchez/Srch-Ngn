cmake_minimum_required(VERSION 3.16)
project(21f_final_proj_template)

set(CMAKE_CXX_STANDARD 17)


add_executable(21f_final_proj_template main.cpp catch_setup.cpp DSAvlTree.h DSHashTable.h tests.cpp
document_parser.cpp document_parser.h query_processor.cpp query_processor.h user_interface.cpp user_interface.h
        porter2_stemmer-master/porter2_stemmer.cpp porter2_stemmer-master/porter2_stemmer.h doc.cpp doc.h)