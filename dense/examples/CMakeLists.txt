add_executable( dense_inference_pr util.cpp dense_inference_pr.cpp )
add_executable( dense_inference_pr_additive util.cpp dense_inference_pr_additive.cpp )

target_link_libraries( dense_inference_pr_additive densecrf ${OpenCV_LIBS})
target_link_libraries( dense_inference_pr densecrf ${OpenCV_LIBS})
