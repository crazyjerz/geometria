all:
	Rscript -e "roxygen2::roxygenise()"
	R CMD build .
	Rscript -e "f <- list.files(pattern='geometria_.*\\.tar\\.gz'); system(paste('R CMD check --no-manual', f))"
	R CMD INSTALL .