# PBI - ukol c. 3
# Alena Tesarova (xtesar36)
# 22.12.2020

library(biomaRt)
library(IRanges)
library(GenomicRanges)

# Get human gene
martHuman <- useMart(biomart="ensembl", dataset="hsapiens_gene_ensembl")

# get mouse gene
martMouse <- useMart(biomart="ensembl", dataset="mmusculus_gene_ensembl")

# Get genes of mouse and human
genesHumanBM <- getBM(attributes = c(
	'start_position',
	 'end_position',
	 'strand',
	'external_gene_name', 
	'chromosome_name'
	 ), mart = martHuman)

genesMouseBM <- getBM(attributes = c(
	'start_position', 
	'end_position', 
	'strand',
	'external_gene_name', 
	'chromosome_name'
	), mart = martMouse)

# Find HOX genes
geneInfoHoxHuman <- genesHumanBM[unique(grep("^HOX[[:alpha:]][[:digit:]]+$", genesHumanBM$external_gene_name)),]
geneInfoHoxMouse <- genesMouseBM[unique(grep("^Hox[[:alpha:]][[:digit:]]+$", genesMouseBM$external_gene_name)),]

# Create IRanges for human and mouse
# Add chr + chromoseme name
irhHuman <- IRanges(start=geneInfoHoxHuman[,1], end=geneInfoHoxHuman[,2], names=paste("chr", geneInfoHoxHuman[,5], sep=""))
irhMouse <- IRanges(start=geneInfoHoxMouse[,1], end=geneInfoHoxMouse[,2], names=paste("chr", geneInfoHoxMouse[,5], sep=""))

# Create GRanges for human and mouse
grhHumen <- unique(GRanges(paste("chr", geneInfoHoxHuman[,5], sep=""), irhHuman, strand="*"))
grhMouse <- unique(GRanges(paste("chr", geneInfoHoxMouse[,5], sep=""), irhMouse, strand="*"))

library(ggbio)
# Package that contains info about human and mouse reference genom
data(ideoCyto, package = "biovizBase")

chromozomeNumHuman = unique(names(grhHumen))
seqlengths(grhHumen) <- as.numeric(seqlengths(ideoCyto$hg18)[c(chromozomeNumHuman)])

chromozomeNumMouse = unique(names(grhMouse))
seqlengths(grhMouse) <- as.numeric(seqlengths(ideoCyto$mm10)[c(chromozomeNumMouse)])

# Show graphs
autoplot(grhHumen, layout = "karyogram")
autoplot(grhMouse, layout = "karyogram")
