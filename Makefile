all:
	R CMD INSTALL .
	Rscript -e 'tinytest::test_package("geometria")'