#pragma once

#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/core/memory.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot{    
    template<typename T, int size_kb>
    class MemArena{
        unsigned long num_elements, num_elements_alloc;
        const unsigned long alloc_chunk_kb = size_kb * 1000;
        const unsigned long num_realloc = (alloc_chunk_kb/sizeof(T));
        const unsigned long mem_realloc = sizeof(T) * mem_realloc;
        T* mem;
        public:
            MemArena(){
                num_elements = 0;
                num_elements_alloc = 0;
                mem = NULL;
            };
            ~MemArena(){
                if(mem != NULL){
                    memfree(mem);
                }
                num_elements = 0;
                num_elements_alloc = 0;
                mem = NULL;
            };
            void add_element(const T *element){
                if(num_elements + 1 >= num_elements_alloc){
                    num_elements_alloc += num_realloc;
                    mem = static_cast<T*>(memrealloc(static_cast<void*>(mem), num_elements_alloc * sizeof(T)));
                    UtilityFunctions::print("Realloc pointer: ", num_elements, " / ", num_elements_alloc, " | ", num_elements_alloc);
                }
                memcpy(mem + num_elements, element, sizeof(T));
                num_elements += 1;
                
            };
            void add_element(const T &element){
                if(num_elements + 1 >= num_elements_alloc){
                    num_elements_alloc += num_realloc;
                    mem = static_cast<T*>(memrealloc(static_cast<void*>(mem), num_elements_alloc * sizeof(T)));
                    UtilityFunctions::print("Realloc ref: ", num_elements, " / ", num_elements_alloc, " | ", num_elements_alloc);
                }
                memcpy(mem + num_elements, &element, sizeof(T));
                num_elements += 1;
            };
            T* get_pointer(unsigned long element_id){
                return mem + element_id;
            };
            const T& get_element(unsigned long element_id){
                return mem[element_id];
            }
            unsigned long get_length(){return num_elements;}
    };
}