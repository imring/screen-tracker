add_library(QtOct STATIC cvmatandqimage.cpp)
target_link_libraries(QtOct PUBLIC
    ${OpenCV_LIBRARIES}
    Qt${QT_VERSION}::Gui
)
target_include_directories(QtOct PUBLIC .)