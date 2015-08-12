add_executable( dense_inference_pr util.cpp dense_inference_pr.cpp )
add_executable( dense_inference_pr_additive util.cpp dense_inference_pr_additive.cpp )
add_executable( cross_validation_pr util.cpp cross_validation_pr.cpp )
add_executable( save_unaries util.cpp save_unaries.cpp )

target_link_libraries( save_unaries densecrf ${OpenCV_LIBS})

target_link_libraries( dense_inference_pr_additive densecrf ${OpenCV_LIBS})
target_link_libraries( dense_inference_pr densecrf ${OpenCV_LIBS})
target_link_libraries( cross_validation_pr densecrf ${OpenCV_LIBS})
