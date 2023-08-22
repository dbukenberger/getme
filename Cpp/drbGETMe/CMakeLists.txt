set(target drbGETMe)

set(sourcefiles
   "main.cpp"
)

add_executable(${target} ${sourcefiles})

target_link_libraries(${target}
    PRIVATE
        smoothing
        common
        pmp_vis
)
