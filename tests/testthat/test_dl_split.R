
context("Test calling dlsplit")

test_that("simple test",{
  expect_equal(dl_split("Hans Meier", "Meier Hans"), 0.1)
})
