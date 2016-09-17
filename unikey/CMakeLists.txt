
set(UNIKEY_SRCS
    byteio.cpp
    byteio.h
    charset.cpp
    charset.h
    convert.cpp
    data.cpp
    data.h
    error.cpp
    inputproc.cpp
    inputproc.h
    keycons.h
    mactab.cpp
    mactab.h
    pattern.cpp
    pattern.h
    ukengine.cpp
    ukengine.h
    unikey.cpp
    unikey.h
    usrkeymap.cpp
    usrkeymap.h
    vnconv.h
    vnlexi.h
)


add_library(unikey STATIC ${UNIKEY_SRCS})
set_target_properties(unikey PROPERTIES COMPILE_FLAGS "-fPIC")
target_link_libraries(unikey LibIntl::LibIntl)
