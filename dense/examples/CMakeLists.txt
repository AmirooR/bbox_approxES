add_executable( dense_inference_pr util.cpp dense_inference_pr.cpp )
add_executable( dense_inference_pr_additive util.cpp dense_inference_pr_additive.cpp )

add_library( probimage probimage.cpp )

add_executable( dense_inference_pr_probimage util.cpp dense_inference_pr_probimage.cpp )
target_link_libraries( dense_inference_pr_probimage densecrf probimage )

add_executable( dense_inference_orig_probimage util.cpp dense_inference_orig_probimage.cpp )
target_link_libraries( dense_inference_orig_probimage densecrf probimage )

add_executable( cross_validation_pr_probimage util.cpp cross_validation_pr_probimage.cpp )
target_link_libraries( cross_validation_pr_probimage densecrf probimage )

add_executable( proposal util.cpp proposal.cpp )
target_link_libraries( proposal densecrf probimage )

add_executable( proposal_additive util.cpp proposal_additive.cpp )
target_link_libraries( proposal_additive densecrf probimage )

add_executable(computeEnergy util.cpp computeEnergy.cpp )
target_link_libraries( computeEnergy densecrf probimage )

target_link_libraries( dense_inference_pr_additive densecrf )
target_link_libraries( dense_inference_pr densecrf )
