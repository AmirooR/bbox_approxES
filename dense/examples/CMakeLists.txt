add_executable( dense_inference_pr util.cpp dense_inference_pr.cpp )
add_executable( dense_inference_pr_additive util.cpp dense_inference_pr_additive.cpp )

add_library( probimage probimage.cpp )

add_executable( dense_inference_pr_probimage util.cpp dense_inference_pr_probimage.cpp )
target_link_libraries( dense_inference_pr_probimage densecrf probimage )

add_executable( dense_inference_orig_probimage util.cpp dense_inference_orig_probimage.cpp )
target_link_libraries( dense_inference_orig_probimage densecrf probimage )

target_link_libraries( dense_inference_pr_additive densecrf )
target_link_libraries( dense_inference_pr densecrf )
